/*
 * auth.c - PPP authentication and phase control.
 *
 * Copyright (c) 1993-2002 Paul Mackerras. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. The name(s) of the authors of this software must not be used to
 *    endorse or promote products derived from this software without
 *    prior written permission.
 *
 * 3. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *    "This product includes software developed by Paul Mackerras
 *     <paulus@samba.org>".
 *
 * THE AUTHORS OF THIS SOFTWARE DISCLAIM ALL WARRANTIES WITH REGARD TO
 * THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS, IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY
 * SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN
 * AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING
 * OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Derived from main.c, which is:
 *
 * Copyright (c) 1984-2000 Carnegie Mellon University. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The name "Carnegie Mellon University" must not be used to
 *    endorse or promote products derived from this software without
 *    prior written permission. For permission or any legal
 *    details, please contact
 *      Office of Technology Transfer
 *      Carnegie Mellon University
 *      5000 Forbes Avenue
 *      Pittsburgh, PA  15213-3890
 *      (412) 268-4387, fax: (412) 268-7395
 *      tech-transfer@andrew.cmu.edu
 *
 * 4. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *    "This product includes software developed by Computing Services
 *     at Carnegie Mellon University (http://www.cmu.edu/computing/)."
 *
 * CARNEGIE MELLON UNIVERSITY DISCLAIMS ALL WARRANTIES WITH REGARD TO
 * THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS, IN NO EVENT SHALL CARNEGIE MELLON UNIVERSITY BE LIABLE
 * FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN
 * AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING
 * OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "netif/ppp/ppp_opts.h"
#if PPP_SUPPORT /* don't build if not configured for use in lwipopts.h */


#include "netif/ppp/ppp_impl.h"

#include "netif/ppp/fsm.h"
#include "netif/ppp/lcp.h"
#if CCP_SUPPORT
#include "netif/ppp/ccp.h"
#endif /* CCP_SUPPORT */
#if ECP_SUPPORT
#include "netif/ppp/ecp.h"
#endif /* ECP_SUPPORT */
#include "netif/ppp/ipcp.h"
#if PAP_SUPPORT
#include "netif/ppp/upap.h"
#endif /* PAP_SUPPORT */
#if CHAP_SUPPORT
#include "netif/ppp/chap-new.h"
#endif /* CHAP_SUPPORT */
#if EAP_SUPPORT
#include "netif/ppp/eap.h"
#endif /* EAP_SUPPORT */
#if CBCP_SUPPORT
#include "netif/ppp/cbcp.h"
#endif





#ifdef HAVE_MULTILINK
/* Hook for plugin to hear when an interface joins a multilink bundle */
void (*multilink_join_hook) (void) = NULL;
#endif

#if PPP_NOTIFY
/* A notifier for when the peer has authenticated itself,
   and we are proceeding to the network phase. */
struct notifier* auth_up_notifier = NULL;

/* A notifier for when the link goes down. */
struct notifier* link_down_notifier = NULL;
#endif /* PPP_NOTIFY */

/*
 * Option variables.
 */
/* Prototypes for procedures local to this file. */

static void network_phase(ppp_pcb* pcb);
#if PPP_IDLETIMELIMIT
static void check_idle(void* arg);
#endif /* PPP_IDLETIMELIMIT */
#if PPP_MAXCONNECT
static void connect_time_expired(void* arg);
#endif /* PPP_MAXCONNECT */

#ifdef MAXOCTETS
static void check_maxoctets (void*);
#endif

#if PPP_OPTIONS
/*
 * Authentication-related options.
 */
option_t auth_options[] = {
    {
        "auth", o_bool, &auth_required,
        "Require authentication from peer", OPT_PRIO | 1
    },
    {
        "noauth", o_bool, &auth_required,
        "Don't require peer to authenticate", OPT_PRIOSUB | OPT_PRIV,
        &allow_any_ip
    },
    {
        "require-pap", o_bool, &lcp_wantoptions[0].neg_upap,
        "Require PAP authentication from peer",
        OPT_PRIOSUB | 1, &auth_required
    },
    {
        "+pap", o_bool, &lcp_wantoptions[0].neg_upap,
        "Require PAP authentication from peer",
        OPT_ALIAS | OPT_PRIOSUB | 1, &auth_required
    },
    {
        "require-chap", o_bool, &auth_required,
        "Require CHAP authentication from peer",
        OPT_PRIOSUB | OPT_A2OR | MDTYPE_MD5,
        &lcp_wantoptions[0].chap_mdtype
    },
    {
        "+chap", o_bool, &auth_required,
        "Require CHAP authentication from peer",
        OPT_ALIAS | OPT_PRIOSUB | OPT_A2OR | MDTYPE_MD5,
        &lcp_wantoptions[0].chap_mdtype
    },
#if MSCHAP_SUPPORT
    {
        "require-mschap", o_bool, &auth_required,
        "Require MS-CHAP authentication from peer",
        OPT_PRIOSUB | OPT_A2OR | MDTYPE_MICROSOFT,
        &lcp_wantoptions[0].chap_mdtype
    },
    {
        "+mschap", o_bool, &auth_required,
        "Require MS-CHAP authentication from peer",
        OPT_ALIAS | OPT_PRIOSUB | OPT_A2OR | MDTYPE_MICROSOFT,
        &lcp_wantoptions[0].chap_mdtype
    },
    {
        "require-mschap-v2", o_bool, &auth_required,
        "Require MS-CHAPv2 authentication from peer",
        OPT_PRIOSUB | OPT_A2OR | MDTYPE_MICROSOFT_V2,
        &lcp_wantoptions[0].chap_mdtype
    },
    {
        "+mschap-v2", o_bool, &auth_required,
        "Require MS-CHAPv2 authentication from peer",
        OPT_ALIAS | OPT_PRIOSUB | OPT_A2OR | MDTYPE_MICROSOFT_V2,
        &lcp_wantoptions[0].chap_mdtype
    },
#endif /* MSCHAP_SUPPORT */
#if MSCHAP_SUPPORT
#endif /* MSCHAP_SUPPORT*/
#if EAP_SUPPORT
    {
        "require-eap", o_bool, &lcp_wantoptions[0].neg_eap,
        "Require EAP authentication from peer", OPT_PRIOSUB | 1,
        &auth_required
    },
#endif /* EAP_SUPPORT */
    {
        "name", o_string, our_name,
        "Set local name for authentication",
        OPT_PRIO | OPT_PRIV | OPT_STATIC, NULL, MAXNAMELEN
    },

    {
        "+ua", o_special, (void*)setupapfile,
        "Get PAP user and password from file",
        OPT_PRIO | OPT_A2STRVAL, &uafname
    },


    {
        "usehostname", o_bool, &usehostname,
        "Must use hostname for authentication", 1
    },

    {
        "remotename", o_string, remote_name,
        "Set remote name for authentication", OPT_PRIO | OPT_STATIC,
        &explicit_remote, MAXNAMELEN
    },

    {
        "login", o_bool, &uselogin,
        "Use system password database for PAP", OPT_A2COPY | 1,
        &session_mgmt
    },
    {
        "enable-session", o_bool, &session_mgmt,
        "Enable session accounting for remote peers", OPT_PRIV | 1
    },

    {
        "papcrypt", o_bool, &cryptpap,
        "PAP passwords are encrypted", 1
    },

    {
        "privgroup", o_special, (void*)privgroup,
        "Allow group members to use privileged options", OPT_PRIV | OPT_A2LIST
    },

    {
        "allow-ip", o_special, (void*)set_noauth_addr,
        "Set IP address(es) which can be used without authentication",
        OPT_PRIV | OPT_A2LIST
    },

    {
        "remotenumber", o_string, remote_number,
        "Set remote telephone number for authentication", OPT_PRIO | OPT_STATIC,
        NULL, MAXNAMELEN
    },

    {
        "allow-number", o_special, (void*)set_permitted_number,
        "Set telephone number(s) which are allowed to connect",
        OPT_PRIV | OPT_A2LIST
    },

    { NULL }
};
#endif /* PPP_OPTIONS */


/*
 * An Open on LCP has requested a change from Dead to Establish phase.
 */
void link_required(ppp_pcb* pcb)
{
    LWIP_UNUSED_ARG(pcb);
}


/*
 * LCP has terminated the link; go to the Dead phase and take the
 * physical layer down.
 */
void link_terminated(ppp_pcb* pcb)
{
    if (pcb->phase == PPP_PHASE_DEAD
#ifdef HAVE_MULTILINK
            || pcb->phase == PPP_PHASE_MASTER
#endif /* HAVE_MULTILINK */
       )
        return;
    new_phase(pcb, PPP_PHASE_DISCONNECT);


    if (!doing_multilink) {
        ppp_notice("Connection terminated.");
#if PPP_STATS_SUPPORT
        print_link_stats();
#endif /* PPP_STATS_SUPPORT */
    }
    else
        ppp_notice("Link terminated.");

    lcp_lowerdown(pcb);

    ppp_link_terminated(pcb);
}

/*
 * LCP has gone down; it will either die or try to re-establish.
 */
void link_down(ppp_pcb* pcb)
{
#if PPP_NOTIFY
    notify(link_down_notifier, 0);
#endif /* PPP_NOTIFY */

    if (!doing_multilink) {
        upper_layers_down(pcb);
        if (pcb->phase != PPP_PHASE_DEAD
#ifdef HAVE_MULTILINK
                && pcb->phase != PPP_PHASE_MASTER
#endif /* HAVE_MULTILINK */
           )
            new_phase(pcb, PPP_PHASE_ESTABLISH);
    }
    /* XXX if doing_multilink, should do something to stop
       network-layer traffic on the link */
}

void upper_layers_down(ppp_pcb* pcb)
{
    int i;
    const struct protent* protp;

    for (i = 0; (protp = protocols[i]) != NULL; ++i) {
        if (protp->protocol != PPP_LCP && protp->lowerdown != NULL)
            (*protp->lowerdown)(pcb);
        if (protp->protocol < 0xC000 && protp->close != NULL)
            (*protp->close)(pcb, "LCP down");
    }
    pcb->num_np_open = 0;
    pcb->num_np_up = 0;
}

/*
 * The link is established.
 * Proceed to the Dead, Authenticate or Network phase as appropriate.
 */
void link_established(ppp_pcb* pcb)
{
#if PPP_AUTH_SUPPORT
    int auth;
#if PPP_SERVER
#if PAP_SUPPORT
    lcp_options* wo = &pcb->lcp_wantoptions;
#endif /* PAP_SUPPORT */
    lcp_options* go = &pcb->lcp_gotoptions;
#endif /* PPP_SERVER */
    lcp_options* ho = &pcb->lcp_hisoptions;
#endif /* PPP_AUTH_SUPPORT */
    int i;
    const struct protent* protp;

    /*
     * Tell higher-level protocols that LCP is up.
     */
    if (!doing_multilink) {
        for (i = 0; (protp = protocols[i]) != NULL; ++i)
            if (protp->protocol != PPP_LCP
                    && protp->lowerup != NULL)
                (*protp->lowerup)(pcb);
    }

#if PPP_AUTH_SUPPORT
#if PPP_SERVER
#if PPP_ALLOWED_ADDRS
    if (!auth_required && noauth_addrs != NULL)
        set_allowed_addrs(unit, NULL, NULL);
#endif /* PPP_ALLOWED_ADDRS */

    if (pcb->settings.auth_required && !(0
#if PAP_SUPPORT
                                         || go->neg_upap
#endif /* PAP_SUPPORT */
#if CHAP_SUPPORT
                                         || go->neg_chap
#endif /* CHAP_SUPPORT */
#if EAP_SUPPORT
                                         || go->neg_eap
#endif /* EAP_SUPPORT */
                                        )) {

#if PPP_ALLOWED_ADDRS
        /*
         * We wanted the peer to authenticate itself, and it refused:
         * if we have some address(es) it can use without auth, fine,
         * otherwise treat it as though it authenticated with PAP using
         * a username of "" and a password of "".  If that's not OK,
         * boot it out.
         */
        if (noauth_addrs != NULL) {
            set_allowed_addrs(unit, NULL, NULL);
        }
        else
#endif /* PPP_ALLOWED_ADDRS */
            if (!pcb->settings.null_login
#if PAP_SUPPORT
                    || !wo->neg_upap
#endif /* PAP_SUPPORT */
               ) {
                ppp_warn("peer refused to authenticate: terminating link");
                pcb->err_code = PPPERR_AUTHFAIL;
                lcp_close(pcb, "peer refused to authenticate");
                return;
            }
    }
#endif /* PPP_SERVER */

    new_phase(pcb, PPP_PHASE_AUTHENTICATE);
    auth = 0;
#if PPP_SERVER
#if EAP_SUPPORT
    if (go->neg_eap) {
        eap_authpeer(pcb, PPP_OUR_NAME);
        auth |= EAP_PEER;
    }
    else
#endif /* EAP_SUPPORT */
#if CHAP_SUPPORT
        if (go->neg_chap) {
            chap_auth_peer(pcb, PPP_OUR_NAME, CHAP_DIGEST(go->chap_mdtype));
            auth |= CHAP_PEER;
        }
        else
#endif /* CHAP_SUPPORT */
#if PAP_SUPPORT
            if (go->neg_upap) {
                upap_authpeer(pcb);
                auth |= PAP_PEER;
            }
            else
#endif /* PAP_SUPPORT */
            {}
#endif /* PPP_SERVER */

#if EAP_SUPPORT
    if (ho->neg_eap) {
        eap_authwithpeer(pcb, pcb->settings.user);
        auth |= EAP_WITHPEER;
    }
    else
#endif /* EAP_SUPPORT */
#if CHAP_SUPPORT
        if (ho->neg_chap) {
            chap_auth_with_peer(pcb, pcb->settings.user, CHAP_DIGEST(ho->chap_mdtype));
            auth |= CHAP_WITHPEER;
        }
        else
#endif /* CHAP_SUPPORT */
#if PAP_SUPPORT
            if (ho->neg_upap) {
                upap_authwithpeer(pcb, pcb->settings.user, pcb->settings.passwd);
                auth |= PAP_WITHPEER;
            }
            else
#endif /* PAP_SUPPORT */
            {}

    pcb->auth_pending = auth;
    pcb->auth_done = 0;

    if (!auth)
#endif /* PPP_AUTH_SUPPORT */
        network_phase(pcb);
}

/*
 * Proceed to the network phase.
 */
static void network_phase(ppp_pcb* pcb)
{
#if CBCP_SUPPORT
    ppp_pcb* pcb = &ppp_pcb_list[unit];
#endif


#if PPP_NOTIFY
    /*
     * If the peer had to authenticate, notify it now.
     */
    if (0
#if CHAP_SUPPORT
            || go->neg_chap
#endif /* CHAP_SUPPORT */
#if PAP_SUPPORT
            || go->neg_upap
#endif /* PAP_SUPPORT */
#if EAP_SUPPORT
            || go->neg_eap
#endif /* EAP_SUPPORT */
       ) {
        notify(auth_up_notifier, 0);
    }
#endif /* PPP_NOTIFY */

#if CBCP_SUPPORT
    /*
     * If we negotiated callback, do it now.
     */
    if (go->neg_cbcp) {
        new_phase(pcb, PPP_PHASE_CALLBACK);
        (*cbcp_protent.open)(pcb);
        return;
    }
#endif

#if PPP_OPTIONS
    /*
     * Process extra options from the secrets file
     */
    if (extra_options) {
        options_from_list(extra_options, 1);
        free_wordlist(extra_options);
        extra_options = 0;
    }
#endif /* PPP_OPTIONS */
    start_networks(pcb);
}

void start_networks(ppp_pcb* pcb)
{
#if CCP_SUPPORT || ECP_SUPPORT
    int i;
    const struct protent* protp;
#endif /* CCP_SUPPORT || ECP_SUPPORT */

    new_phase(pcb, PPP_PHASE_NETWORK);

#ifdef HAVE_MULTILINK
    if (multilink) {
        if (mp_join_bundle()) {
            if (multilink_join_hook)
                (*multilink_join_hook)();
            if (updetach && !nodetach)
                detach();
            return;
        }
    }
#endif /* HAVE_MULTILINK */

#ifdef PPP_FILTER
    if (!demand)
        set_filters(&pass_filter, &active_filter);
#endif
#if CCP_SUPPORT || ECP_SUPPORT
    /* Start CCP and ECP */
    for (i = 0; (protp = protocols[i]) != NULL; ++i)
        if (
            (0
#if ECP_SUPPORT
             || protp->protocol == PPP_ECP
#endif /* ECP_SUPPORT */
#if CCP_SUPPORT
             || protp->protocol == PPP_CCP
#endif /* CCP_SUPPORT */
            )
            && protp->open != NULL)
            (*protp->open)(pcb);
#endif /* CCP_SUPPORT || ECP_SUPPORT */

    /*
     * Bring up other network protocols iff encryption is not required.
     */
    if (1
#if ECP_SUPPORT
            && !ecp_gotoptions[unit].required
#endif /* ECP_SUPPORT */
#if MPPE_SUPPORT
            && !pcb->ccp_gotoptions.mppe
#endif /* MPPE_SUPPORT */
       )
        continue_networks(pcb);
}

void continue_networks(ppp_pcb* pcb)
{
    int i;
    const struct protent* protp;

    /*
     * Start the "real" network protocols.
     */
    for (i = 0; (protp = protocols[i]) != NULL; ++i)
        if (protp->protocol < 0xC000
#if CCP_SUPPORT
                && protp->protocol != PPP_CCP
#endif /* CCP_SUPPORT */
#if ECP_SUPPORT
                && protp->protocol != PPP_ECP
#endif /* ECP_SUPPORT */
                && protp->open != NULL) {
            (*protp->open)(pcb);
            ++pcb->num_np_open;
        }

    if (pcb->num_np_open == 0)
        /* nothing to do */
        lcp_close(pcb, "No network protocols running");
}

#if PPP_AUTH_SUPPORT
#if PPP_SERVER
/*
 * auth_check_passwd - Check the user name and passwd against configuration.
 *
 * returns:
 *      0: Authentication failed.
 *      1: Authentication succeeded.
 * In either case, msg points to an appropriate message and msglen to the message len.
 */
int auth_check_passwd(ppp_pcb* pcb, char* auser, int userlen, char* apasswd, int passwdlen, const char** msg, int* msglen)
{
    int secretuserlen;
    int secretpasswdlen;

    if (pcb->settings.user && pcb->settings.passwd) {
        secretuserlen = (int)strlen(pcb->settings.user);
        secretpasswdlen = (int)strlen(pcb->settings.passwd);
        if (secretuserlen == userlen
                && secretpasswdlen == passwdlen
                && !memcmp(auser, pcb->settings.user, userlen)
                && !memcmp(apasswd, pcb->settings.passwd, passwdlen) ) {
            *msg = "Login ok";
            *msglen = sizeof("Login ok") - 1;
            return 1;
        }
    }

    *msg = "Login incorrect";
    *msglen = sizeof("Login incorrect") - 1;
    return 0;
}

/*
 * The peer has failed to authenticate himself using `protocol'.
 */
void auth_peer_fail(ppp_pcb* pcb, int protocol)
{
    LWIP_UNUSED_ARG(protocol);
    /*
     * Authentication failure: take the link down
     */
    pcb->err_code = PPPERR_AUTHFAIL;
    lcp_close(pcb, "Authentication failed");
}

/*
 * The peer has been successfully authenticated using `protocol'.
 */
void auth_peer_success(ppp_pcb* pcb, int protocol, int prot_flavor, const char* name, int namelen)
{
    int bit;
#ifndef HAVE_MULTILINK
    LWIP_UNUSED_ARG(name);
    LWIP_UNUSED_ARG(namelen);
#endif /* HAVE_MULTILINK */

    switch (protocol) {
#if CHAP_SUPPORT
        case PPP_CHAP:
            bit = CHAP_PEER;
            switch (prot_flavor) {
                case CHAP_MD5:
                    bit |= CHAP_MD5_PEER;
                    break;
#if MSCHAP_SUPPORT
                case CHAP_MICROSOFT:
                    bit |= CHAP_MS_PEER;
                    break;
                case CHAP_MICROSOFT_V2:
                    bit |= CHAP_MS2_PEER;
                    break;
#endif /* MSCHAP_SUPPORT */
                default:
                    break;
            }
            break;
#endif /* CHAP_SUPPORT */
#if PAP_SUPPORT
        case PPP_PAP:
            bit = PAP_PEER;
            break;
#endif /* PAP_SUPPORT */
#if EAP_SUPPORT
        case PPP_EAP:
            bit = EAP_PEER;
            break;
#endif /* EAP_SUPPORT */
        default:
            ppp_warn("auth_peer_success: unknown protocol %x", protocol);
            return;
    }

#ifdef HAVE_MULTILINK
    /*
     * Save the authenticated name of the peer for later.
     */
    if (namelen > (int)sizeof(pcb->peer_authname) - 1)
        namelen = (int)sizeof(pcb->peer_authname) - 1;
    MEMCPY(pcb->peer_authname, name, namelen);
    pcb->peer_authname[namelen] = 0;
#endif /* HAVE_MULTILINK */

    /* Save the authentication method for later. */
    pcb->auth_done |= bit;

    /*
     * If there is no more authentication still to be done,
     * proceed to the network (or callback) phase.
     */
    if ((pcb->auth_pending &= ~bit) == 0)
        network_phase(pcb);
}
#endif /* PPP_SERVER */

/*
 * We have failed to authenticate ourselves to the peer using `protocol'.
 */
void auth_withpeer_fail(ppp_pcb* pcb, int protocol)
{
    LWIP_UNUSED_ARG(protocol);
    /*
     * We've failed to authenticate ourselves to our peer.
     *
     * Some servers keep sending CHAP challenges, but there
     * is no point in persisting without any way to get updated
     * authentication secrets.
     *
     * He'll probably take the link down, and there's not much
     * we can do except wait for that.
     */
    pcb->err_code = PPPERR_AUTHFAIL;
    lcp_close(pcb, "Failed to authenticate ourselves to peer");
}

/*
 * We have successfully authenticated ourselves with the peer using `protocol'.
 */
void auth_withpeer_success(ppp_pcb* pcb, int protocol, int prot_flavor)
{
    int bit;
    const char* prot = "";

    switch (protocol) {
#if CHAP_SUPPORT
        case PPP_CHAP:
            bit = CHAP_WITHPEER;
            prot = "CHAP";
            switch (prot_flavor) {
                case CHAP_MD5:
                    bit |= CHAP_MD5_WITHPEER;
                    break;
#if MSCHAP_SUPPORT
                case CHAP_MICROSOFT:
                    bit |= CHAP_MS_WITHPEER;
                    break;
                case CHAP_MICROSOFT_V2:
                    bit |= CHAP_MS2_WITHPEER;
                    break;
#endif /* MSCHAP_SUPPORT */
                default:
                    break;
            }
            break;
#endif /* CHAP_SUPPORT */
#if PAP_SUPPORT
        case PPP_PAP:
            bit = PAP_WITHPEER;
            prot = "PAP";
            break;
#endif /* PAP_SUPPORT */
#if EAP_SUPPORT
        case PPP_EAP:
            bit = EAP_WITHPEER;
            prot = "EAP";
            break;
#endif /* EAP_SUPPORT */
        default:
            ppp_warn("auth_withpeer_success: unknown protocol %x", protocol);
            bit = 0;
            /* no break */
    }

    ppp_notice("%s authentication succeeded", prot);

    /* Save the authentication method for later. */
    pcb->auth_done |= bit;

    /*
     * If there is no more authentication still being done,
     * proceed to the network (or callback) phase.
     */
    if ((pcb->auth_pending &= ~bit) == 0)
        network_phase(pcb);
}
#endif /* PPP_AUTH_SUPPORT */


/*
 * np_up - a network protocol has come up.
 */
void np_up(ppp_pcb* pcb, int proto)
{
#if PPP_IDLETIMELIMIT
    int tlim;
#endif /* PPP_IDLETIMELIMIT */
    LWIP_UNUSED_ARG(proto);

    if (pcb->num_np_up == 0) {
        /*
         * At this point we consider that the link has come up successfully.
         */
        new_phase(pcb, PPP_PHASE_RUNNING);

#if PPP_IDLETIMELIMIT
        tlim = pcb->settings.idle_time_limit;
        if (tlim > 0)
            TIMEOUT(check_idle, (void*)pcb, tlim);
#endif /* PPP_IDLETIMELIMIT */

#if PPP_MAXCONNECT
        /*
         * Set a timeout to close the connection once the maximum
         * connect time has expired.
         */
        if (pcb->settings.maxconnect > 0)
            TIMEOUT(connect_time_expired, (void*)pcb, pcb->settings.maxconnect);
#endif /* PPP_MAXCONNECT */

#ifdef MAXOCTETS
        if (maxoctets > 0)
            TIMEOUT(check_maxoctets, NULL, maxoctets_timeout);
#endif

    }
    ++pcb->num_np_up;
}

/*
 * np_down - a network protocol has gone down.
 */
void np_down(ppp_pcb* pcb, int proto)
{
    LWIP_UNUSED_ARG(proto);
    if (--pcb->num_np_up == 0) {
#if PPP_IDLETIMELIMIT
        UNTIMEOUT(check_idle, (void*)pcb);
#endif /* PPP_IDLETIMELIMIT */
#if PPP_MAXCONNECT
        UNTIMEOUT(connect_time_expired, NULL);
#endif /* PPP_MAXCONNECT */
#ifdef MAXOCTETS
        UNTIMEOUT(check_maxoctets, NULL);
#endif
        new_phase(pcb, PPP_PHASE_NETWORK);
    }
}

/*
 * np_finished - a network protocol has finished using the link.
 */
void np_finished(ppp_pcb* pcb, int proto)
{
    LWIP_UNUSED_ARG(proto);
    if (--pcb->num_np_open <= 0) {
        /* no further use for the link: shut up shop. */
        lcp_close(pcb, "No network protocols running");
    }
}

#ifdef MAXOCTETS
static void
check_maxoctets(arg)
void* arg;
{
#if PPP_STATS_SUPPORT
    unsigned int used;

    update_link_stats(ifunit);
    link_stats_valid = 0;

    switch(maxoctets_dir) {
        case PPP_OCTETS_DIRECTION_IN:
            used = link_stats.bytes_in;
            break;
        case PPP_OCTETS_DIRECTION_OUT:
            used = link_stats.bytes_out;
            break;
        case PPP_OCTETS_DIRECTION_MAXOVERAL:
        case PPP_OCTETS_DIRECTION_MAXSESSION:
            used = (link_stats.bytes_in > link_stats.bytes_out) ? link_stats.bytes_in : link_stats.bytes_out;
            break;
        default:
            used = link_stats.bytes_in + link_stats.bytes_out;
            break;
    }
    if (used > maxoctets) {
        ppp_notice("Traffic limit reached. Limit: %u Used: %u", maxoctets, used);
        status = EXIT_TRAFFIC_LIMIT;
        lcp_close(pcb, "Traffic limit");
    }
    else {
        TIMEOUT(check_maxoctets, NULL, maxoctets_timeout);
    }
#endif /* PPP_STATS_SUPPORT */
}
#endif /* MAXOCTETS */

#if PPP_IDLETIMELIMIT
/*
 * check_idle - check whether the link has been idle for long
 * enough that we can shut it down.
 */
static void check_idle(void* arg)
{
    ppp_pcb* pcb = (ppp_pcb*)arg;
    struct ppp_idle idle;
    time_t itime;
    int tlim;

    if (!get_idle_time(pcb, &idle))
        return;
    itime = LWIP_MIN(idle.xmit_idle, idle.recv_idle);
    tlim = pcb->settings.idle_time_limit - itime;
    if (tlim <= 0) {
        /* link is idle: shut it down. */
        ppp_notice("Terminating connection due to lack of activity.");
        pcb->err_code = PPPERR_IDLETIMEOUT;
        lcp_close(pcb, "Link inactive");
    }
    else {
        TIMEOUT(check_idle, (void*)pcb, tlim);
    }
}
#endif /* PPP_IDLETIMELIMIT */

#if PPP_MAXCONNECT
/*
 * connect_time_expired - log a message and close the connection.
 */
static void connect_time_expired(void* arg)
{
    ppp_pcb* pcb = (ppp_pcb*)arg;
    ppp_info("Connect time expired");
    pcb->err_code = PPPERR_CONNECTTIME;
    lcp_close(pcb, "Connect time expired"); /* Close connection */
}
#endif /* PPP_MAXCONNECT */

#if PPP_OPTIONS
/*
 * auth_check_options - called to check authentication options.
 */
void
auth_check_options()
{
    lcp_options* wo = &lcp_wantoptions[0];
    int can_auth;
    int lacks_ip;

    /* Default our_name to hostname, and user to our_name */
    if (our_name[0] == 0 || usehostname)
        strlcpy(our_name, hostname, sizeof(our_name));
    /* If a blank username was explicitly given as an option, trust
       the user and don't use our_name */
    if (ppp_settings.user[0] == 0 && !explicit_user)
        strlcpy(ppp_settings.user, our_name, sizeof(ppp_settings.user));

    /*
     * If we have a default route, require the peer to authenticate
     * unless the noauth option was given or the real user is root.
     */
    if (!auth_required && !allow_any_ip && have_route_to(0) && !privileged) {
        auth_required = 1;
        default_auth = 1;
    }

#if CHAP_SUPPORT
    /* If we selected any CHAP flavors, we should probably negotiate it. :-) */
    if (wo->chap_mdtype)
        wo->neg_chap = 1;
#endif /* CHAP_SUPPORT */

    /* If authentication is required, ask peer for CHAP, PAP, or EAP. */
    if (auth_required) {
        allow_any_ip = 0;
        if (1
#if CHAP_SUPPORT
                && !wo->neg_chap
#endif /* CHAP_SUPPORT */
#if PAP_SUPPORT
                && !wo->neg_upap
#endif /* PAP_SUPPORT */
#if EAP_SUPPORT
                && !wo->neg_eap
#endif /* EAP_SUPPORT */
           ) {
#if CHAP_SUPPORT
            wo->neg_chap = CHAP_MDTYPE_SUPPORTED != MDTYPE_NONE;
            wo->chap_mdtype = CHAP_MDTYPE_SUPPORTED;
#endif /* CHAP_SUPPORT */
#if PAP_SUPPORT
            wo->neg_upap = 1;
#endif /* PAP_SUPPORT */
#if EAP_SUPPORT
            wo->neg_eap = 1;
#endif /* EAP_SUPPORT */
        }
    }
    else {
#if CHAP_SUPPORT
        wo->neg_chap = 0;
        wo->chap_mdtype = MDTYPE_NONE;
#endif /* CHAP_SUPPORT */
#if PAP_SUPPORT
        wo->neg_upap = 0;
#endif /* PAP_SUPPORT */
#if EAP_SUPPORT
        wo->neg_eap = 0;
#endif /* EAP_SUPPORT */
    }

    /*
     * Check whether we have appropriate secrets to use
     * to authenticate the peer.  Note that EAP can authenticate by way
     * of a CHAP-like exchanges as well as SRP.
     */
    lacks_ip = 0;
#if PAP_SUPPORT
    can_auth = wo->neg_upap && (uselogin || have_pap_secret(&lacks_ip));
#else
    can_auth = 0;
#endif /* PAP_SUPPORT */
    if (!can_auth && (0
#if CHAP_SUPPORT
                      || wo->neg_chap
#endif /* CHAP_SUPPORT */
#if EAP_SUPPORT
                      || wo->neg_eap
#endif /* EAP_SUPPORT */
                     )) {
#if CHAP_SUPPORT
        can_auth = have_chap_secret((explicit_remote ? remote_name : NULL),
                                    our_name, 1, &lacks_ip);
#else
        can_auth = 0;
#endif
    }
    if (!can_auth
#if EAP_SUPPORT
            && wo->neg_eap
#endif /* EAP_SUPPORT */
       ) {
        can_auth = have_srp_secret((explicit_remote ? remote_name : NULL),
                                   our_name, 1, &lacks_ip);
    }

    if (auth_required && !can_auth && noauth_addrs == NULL) {
        if (default_auth) {
            option_error(
                "By default the remote system is required to authenticate itself");
            option_error(
                "(because this system has a default route to the internet)");
        }
        else if (explicit_remote)
            option_error(
                "The remote system (%s) is required to authenticate itself",
                remote_name);
        else
            option_error(
                "The remote system is required to authenticate itself");
        option_error(
            "but I couldn't find any suitable secret (password) for it to use to do so.");
        if (lacks_ip)
            option_error(
                "(None of the available passwords would let it use an IP address.)");

        exit(1);
    }

    /*
     * Early check for remote number authorization.
     */
    if (!auth_number()) {
        ppp_warn("calling number %q is not authorized", remote_number);
        exit(EXIT_CNID_AUTH_FAILED);
    }
}
#endif /* PPP_OPTIONS */


#if PPP_AUTH_SUPPORT
/*
 * get_secret - open the CHAP secret file and return the secret
 * for authenticating the given client on the given server.
 * (We could be either client or server).
 */
int get_secret(ppp_pcb* pcb, const char* client, const char* server, char* secret, int* secret_len, int am_server)
{
    int len;
    LWIP_UNUSED_ARG(server);
    LWIP_UNUSED_ARG(am_server);

    if (!client || !client[0] || !pcb->settings.user || !pcb->settings.passwd || strcmp(client, pcb->settings.user)) {
        return 0;
    }

    len = (int)strlen(pcb->settings.passwd);
    if (len > MAXSECRETLEN) {
        ppp_error("Secret for %s on %s is too long", client, server);
        len = MAXSECRETLEN;
    }

    MEMCPY(secret, pcb->settings.passwd, len);
    *secret_len = len;
    return 1;

}
#endif /* PPP_AUTH_SUPPORT */



#endif /* PPP_SUPPORT */
