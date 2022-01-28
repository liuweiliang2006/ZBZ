#ifndef _SYSTEM_PRSRAM_H_
#define _SYSTEM_PRSRAM_H_

/**************************************************************************
                                 INCLUDES
***************************************************************************/
#define PROGPC_HEAD_BYTES             256
#define PROGPC_HEAD_WORDS             128//除通道号外前面公用部分


#define STEP_HEAD_BYTES             2
#define STEP_HEAD_WORDS             1
#define STEP_END_CONDITION_BYTES    2
#define STEP_END_CONDITION_WORDS    1

#define MAX_CHAN_PROG               7680
#define MAX_STEP_CNT                200//(7680-40)/(18+5*4)

#define SAVE_BACKUP_CHAN_BYTES      120 
#define SAVE_BACKUP_CHAN_WORDS      60

#define SINGLE_PACKAGE_BYTES        924//(SINGLE_SECTION_BYTES * MAX_SECTION_COUNT + 6)
#define SINGLE_PACKAGE_WORDS        462//(SINGLE_SECTION_WORDS * MAX_SECTION_COUNT + 3)


#define MAX_REC_COMSIZE      30
#define MAX_REC_DATASIZE     1024

#define SAVE_BUFFA_SIZE      500
#define SAVE_BUFFB_SIZE      500

#endif 
