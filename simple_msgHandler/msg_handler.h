#ifndef __MSG_HANDLER__
#define __MSG_HANDLER__
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <rtthread.h>
#include "board.h"


#define USER_MSG_MAX_NUM    8


#define MSG_FREE            0

typedef struct{
    rt_mutex_t msgLock;
    uint8_t msgData[USER_MSG_MAX_NUM];
}MsgHandler_t;

int msg_send(MsgHandler_t *its,uint8_t msg);
uint8_t msg_get(MsgHandler_t *its);
void init_msg(MsgHandler_t *its,char *msgName);
void clear_msg(MsgHandler_t *its);
void msg_urgentSend(MsgHandler_t *its,uint8_t msg);




#endif

