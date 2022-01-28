#include "main.h"
#include "lwip/opt.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/timeouts.h"
#include "netif/ethernet.h"
#include "netif/etharp.h"
#include "lwip/ethip6.h"
#include "eth_bsp_config.h"
#include "HAL_eth.h"
#include "ethernetif.h"
#include <string.h>
#include "GY_CPLD.h"

// Network interface name
#define IFNAME0 'm'
#define IFNAME1 'm'

struct ethernetif {
    struct eth_addr* ethaddr;
// Add whatever per-interface state that is needed here.
};
static void arp_timer(void* arg);
void ethernetif_input(void* pParams);
xSemaphoreHandle s_xSemaphore = NULL;


sys_sem_t tx_sem = NULL;
sys_mbox_t eth_tx_mb = NULL;
void ETH_NVIC_Config(void);
static void low_level_init(struct netif* netif)
{
	  uint8_t MAC_ADD=0;
    ETH_BSP_Config();

    if (EthStatus & ETH_LINK_FLAG) {
        netif->flags |= NETIF_FLAG_LINK_UP;                                     // Set netif link flag
    }

#if LWIP_ARP || LWIP_ETHERNET

    // set MAC hardware address length
    netif->hwaddr_len = ETH_HWADDR_LEN;

		MAC_ADD=(READSTATE_ADDR+10);
    // set MAC hardware address
    netif->hwaddr[0] = MAC_ADD;
    netif->hwaddr[1] = MAC_ADDR1;
    netif->hwaddr[2] = MAC_ADDR2;
    netif->hwaddr[3] = MAC_ADDR3;
    netif->hwaddr[4] = MAC_ADDR4;
    netif->hwaddr[5] = MAC_ADDR5;

    /* initialize MAC address in ethernet MAC */
    ETH_MACAddressConfig(ETH_MAC_Address0, netif->hwaddr);

    // maximum transfer unit
    netif->mtu = NETIF_MTU;

#if LWIP_ARP
    netif->flags |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;
#else
    netif->flags |= NETIF_FLAG_BROADCAST;
#endif

#endif

    s_xSemaphore = xSemaphoreCreateCounting(40, 0);

    if(sys_sem_new(&tx_sem, 0) == ERR_OK)
		{
		
		}
      //  printf("sys_sem_new ok\n");

    if(sys_mbox_new(&eth_tx_mb, 50) == ERR_OK)
		{
		
		}
//        printf("sys_mbox_new ok\n");

    sys_thread_new("ETHIN",
                   ethernetif_input,   // Task entry function
                   netif,               // Task entry function parameters
                   NETIF_IN_TASK_STACK_SIZE, // Task stack size
                   NETIF_IN_TASK_PRIORITY);  // The priority of the task

    /* Initialize Tx Descriptors list: Chain Mode */
    ETH_DMATxDescChainInit(DMATxDscrTab, &Tx_Buff[0][0], ETH_TX_BUF_NUM);
    /* Initialize Rx Descriptors list: Chain Mode  */
    ETH_DMARxDescChainInit(DMARxDscrTab, &Rx_Buff[0][0], ETH_RX_BUF_NUM);

    ETH_NVIC_Config();

    ETH_Start();
}

static err_t low_level_output(struct netif* netif, struct pbuf* p)
{
    static sys_sem_t ousem = NULL;
    if(ousem == NULL) {
        sys_sem_new(&ousem, 0);
        sys_sem_signal(&ousem);
    }

    err_t errval;
    struct pbuf* q;

    uint8_t* buffer = (uint8_t*)(DMATxDescToSet->BUF1ADDR);

    __IO ETH_DMADESCTypeDef* DmaTxDesc;
    uint32_t framelength = 0;
    uint32_t bufferoffset = 0;
    uint32_t byteslefttocopy = 0;
    uint32_t payloadoffset = 0;
    DmaTxDesc = DMATxDescToSet;
    bufferoffset = 0;

    if((DmaTxDesc->CS & ETH_DMA_TDES_OWN)) {
        errval = ERR_USE;
        goto error;
    }
    sys_sem_wait(&ousem);
    // copy frame from pbufs to driver buffers
    for(q = p; q != NULL; q = q->next) {
        // Get bytes in current lwIP buffer
        byteslefttocopy = q->len;
        payloadoffset = 0;

        // Check if the length of data to copy is bigger than Tx buffer size
        while( (byteslefttocopy + bufferoffset) > ETH_TX_BUF_SIZE ) {
            // Copy data to Tx buffer
            memcpy( (uint8_t*)((uint8_t*)buffer + bufferoffset),
                    (uint8_t*)((uint8_t*)q->payload + payloadoffset),
                    (ETH_TX_BUF_SIZE - bufferoffset));

            // Point to next descriptor
            DmaTxDesc = (ETH_DMADESCTypeDef*)(DmaTxDesc->BUF2NDADDR);

            // Check if the buffer is available
            if((DmaTxDesc->CS & ETH_DMA_TDES_OWN)) {
                errval = ERR_USE;
                goto error;
            }

            buffer = (uint8_t*)(DmaTxDesc->BUF1ADDR);

            byteslefttocopy = byteslefttocopy - (ETH_TX_BUF_SIZE - bufferoffset);
            payloadoffset = payloadoffset + (ETH_TX_BUF_SIZE - bufferoffset);
            framelength = framelength + (ETH_TX_BUF_SIZE - bufferoffset);
            bufferoffset = 0;
        }

        // Copy the remaining bytes
        memcpy( (uint8_t*)((uint8_t*)buffer + bufferoffset),
                (uint8_t*)((uint8_t*)q->payload + payloadoffset),
                byteslefttocopy);
        bufferoffset = bufferoffset + byteslefttocopy;
        framelength = framelength + byteslefttocopy;
    }

    // Prepare transmit descriptors to give to DMA
    ETH_Prepare_Transmit_Descriptors(framelength);

    errval = ERR_OK;

error:
    if (ETH->DMASR & ETH_DMASR_TUS) {
        // Clear TUS ETHERNET DMA flag
        ETH->DMASR = ETH_DMASR_TUS;

        // Resume DMA transmission
        ETH->DMATPDR = 0;
    }

    sys_sem_signal(&ousem);

    return errval;
}


static struct pbuf* low_level_input(struct netif* netif)
{
    struct pbuf* p = NULL;
    struct pbuf* q = NULL;
    uint16_t len = 0;
    FrameTypeDef frame;
    uint8_t* buffer;
    __IO ETH_DMADESCTypeDef* dmarxdesc;
    uint32_t bufferoffset = 0;
    uint32_t payloadoffset = 0;
    uint32_t byteslefttocopy = 0;
    uint32_t i = 0;

    if (!ETH_CheckFrameReceived())
        return p;

    // get received frame
    frame = ETH_Get_Received_Frame();

    // Obtain the size of the packet and put it into the "len" variable.
    len = frame.len;
    buffer = (u8*)frame.buf;

    // PRINT_INFO("receive frame %d len buffer : %s\n", len, buffer);
    if (len > 0) {
        /* We allocate a pbuf chain of pbufs from the Lwip buffer pool */
        p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);
    }

    if (p != NULL) {
        dmarxdesc = frame.ptrDesc;
        bufferoffset = 0;

        for(q = p; q != NULL; q = q->next) {
            byteslefttocopy = q->len;
            payloadoffset = 0;

            while( (byteslefttocopy + bufferoffset) > ETH_RX_BUF_SIZE ) {
                // Copy data to pbuf
                memcpy( (uint8_t*)((uint8_t*)q->payload + payloadoffset),
                        (uint8_t*)((uint8_t*)buffer + bufferoffset),
                        (ETH_RX_BUF_SIZE - bufferoffset));

                // Point to next descriptor
                dmarxdesc = (ETH_DMADESCTypeDef*)(dmarxdesc->BUF2NDADDR);
                buffer = (uint8_t*)(dmarxdesc->BUF1ADDR);

                byteslefttocopy = byteslefttocopy - (ETH_RX_BUF_SIZE - bufferoffset);
                payloadoffset = payloadoffset + (ETH_RX_BUF_SIZE - bufferoffset);
                bufferoffset = 0;
            }

            // Copy remaining data in pbuf
            memcpy( (uint8_t*)((uint8_t*)q->payload + payloadoffset),
                    (uint8_t*)((uint8_t*)buffer + bufferoffset),
                    byteslefttocopy);
            bufferoffset = bufferoffset + byteslefttocopy;
        }
    }

    // Release descriptors to DMA
    // Point to first descriptor
    dmarxdesc = frame.ptrDesc;

    // Set Own bit in Rx descriptors: gives the buffers back to DMA
    for (i = 0; i < DMA_RX_FRAME_infos->cnt; i++) {
        dmarxdesc->CS |= ETH_DMA_RDES_OWN;
        dmarxdesc = (ETH_DMADESCTypeDef*)(dmarxdesc->BUF2NDADDR);
    }

    // Clear Segment_Count
    DMA_RX_FRAME_infos->cnt = 0;

    // When Rx Buffer unavailable flag is set: clear it and resume reception */
    if ((ETH->DMASR & ETH_DMASR_RBUS) != (uint32_t)RESET) {
        /* Clear RBUS ETHERNET DMA flag */
        ETH->DMASR = ETH_DMASR_RBUS;
        /* Resume DMA reception */
        ETH->DMARPDR = 0;
    }

    return p;
}

void ethernetif_input(void* pParams)
{
    struct netif* netif;
//    err_t err;
    struct pbuf* p = NULL;
    netif = (struct netif*) pParams;

    while(1) {
        if(xSemaphoreTake( s_xSemaphore, portMAX_DELAY ) == pdTRUE) {
            /* move received packet into a new pbuf */
            taskENTER_CRITICAL();
TRY_GET_NEXT_FRAGMENT:
            p = low_level_input(netif);
            taskEXIT_CRITICAL();
            /* points to packet payload, which starts with an Ethernet header */
            if(p != NULL) {
                taskENTER_CRITICAL();
                /* full packet send to tcpip_thread to process */
                if (netif->input(p, netif) != ERR_OK) {
                    LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: IP input error\n"));
                    pbuf_free(p);
                    p = NULL;
                }
                else {
                    xSemaphoreTake( s_xSemaphore, 0);
                    goto TRY_GET_NEXT_FRAGMENT;
                }
                taskEXIT_CRITICAL();
            }
        }
    }
}


#if !LWIP_ARP

static err_t low_level_output_arp_off(struct netif* netif, struct pbuf* q, const ip4_addr_t* ipaddr)
{
    err_t errval;
    errval = ERR_OK;

    return errval;
}
#endif // LWIP_ARP

err_t ethernetif_init(struct netif* netif)
{
    struct ethernetif* ethernetif;

    LWIP_ASSERT("netif != NULL", (netif != NULL));

    ethernetif = mem_malloc(sizeof(struct ethernetif));

    if (ethernetif == NULL) {
        // PRINT_ERR("ethernetif_init: out of memory\n");
        return ERR_MEM;
    }

    LWIP_ASSERT("netif != NULL", (netif != NULL));

#if LWIP_NETIF_HOSTNAME
    // Initialize interface hostname
    netif->hostname = "lwip";
#endif // LWIP_NETIF_HOSTNAME
    netif->state = ethernetif;
    netif->name[0] = IFNAME0;
    netif->name[1] = IFNAME1;

#if LWIP_IPV4
#if LWIP_ARP || LWIP_ETHERNET
#if LWIP_ARP
    netif->output = etharp_output;
#else
    netif->output = low_level_output_arp_off;
#endif // LWIP_ARP
#endif // LWIP_ARP || LWIP_ETHERNET
#endif // LWIP_IPV4

#if LWIP_IPV6
    netif->output_ip6 = ethip6_output;
#endif // LWIP_IPV6

    netif->linkoutput = low_level_output;

    // initialize the hardware
    low_level_init(netif);
    ethernetif->ethaddr = (struct eth_addr*) & (netif->hwaddr[0]);

    return ERR_OK;
}

static void arp_timer(void* arg)
{
    etharp_tmr();
    sys_timeout(ARP_TMR_INTERVAL, arp_timer, NULL);
}

/* USER CODE BEGIN 6 */

#if LWIP_NETIF_LINK_CALLBACK

void ethernetif_update_config(struct netif* netif)
{
    __IO uint32_t tickstart = 0;
    uint32_t regvalue = 0;

    if(netif_is_link_up(netif)) {
        // Restart the auto-negotiation
        if(heth.Init.AutoNegotiation != ETH_AUTONEGOTIATION_DISABLE) {
            // Enable Auto-Negotiation
            HAL_ETH_WritePHYRegister(&heth, PHY_BCR, PHY_AUTONEGOTIATION);

            // Get tick
            tickstart = HAL_GetTick();

            // Wait until the auto-negotiation will be completed
            do {
                HAL_ETH_ReadPHYRegister(&heth, PHY_BSR, &regvalue);

                // Check for the Timeout ( 1s )
                if((HAL_GetTick() - tickstart ) > 1000) {
                    // In case of timeout
                    goto error;
                }
            } while (((regvalue & PHY_AUTONEGO_COMPLETE) != PHY_AUTONEGO_COMPLETE));

            // Read the result of the auto-negotiation
            HAL_ETH_ReadPHYRegister(&heth, PHY_SR, &regvalue);

            if((regvalue & PHY_DUPLEX_STATUS) != (uint32_t)RESET) {
                heth.Init.DuplexMode = ETH_MODE_FULLDUPLEX;
            }
            else {
                heth.Init.DuplexMode = ETH_MODE_HALFDUPLEX;
            }

            if(regvalue & PHY_SPEED_STATUS) {
                /* Set Ethernet speed to 10M following the auto-negotiation */
                heth.Init.Speed = ETH_SPEED_10M;
            }
            else {
                /* Set Ethernet speed to 100M following the auto-negotiation */
                heth.Init.Speed = ETH_SPEED_100M;
            }
        }
        else {    // AutoNegotiation Disable
error :
            // Check parameters

            // Set MAC Speed and Duplex Mode to PHY
            ETH_WritePHYRegister(&heth, PHY_BCR,
                                 ((uint16_t)(heth.Init.DuplexMode >> 3) |
                                  (uint16_t)(heth.Init.Speed >> 1)));
        }

        // ETHERNET MAC Re-Configuration
        HAL_ETH_ConfigMAC(&heth, (ETH_MACInitTypeDef*) NULL);

        // Restart MAC interface
        HAL_ETH_Start(&heth);
    }
    else {
        // Stop MAC interface
        HAL_ETH_Stop(&heth);
    }

    ethernetif_notify_conn_changed(netif);
}
#endif
