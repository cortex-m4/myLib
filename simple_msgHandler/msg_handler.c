#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <rtthread.h>
#include "board.h"


#include "msg_handler.h"

int msg_send(MsgHandler_t *its,uint8_t msg){
    uint8_t i;
    uint8_t temp_msg=msg;
    rt_mutex_take(its->msgLock, 500);
    for(i = 0;i<USER_MSG_MAX_NUM;i++)
    {
        if(its->msgData[i] == MSG_FREE)
        {
        	its->msgData[i] = temp_msg;
        	break;
        }
    }
    rt_mutex_release(its->msgLock);
    if(i==USER_MSG_MAX_NUM){
        return -1;
    }else{
        return 0;
    }
}
void msg_urgentSend(MsgHandler_t *its,uint8_t msg){
	uint8_t i;
	for(i=USER_MSG_MAX_NUM-1;i>1;i--){
		its->msgData[i]=its->msgData[i-1];
	}
    its->msgData[0] = msg;
}

uint8_t msg_get(MsgHandler_t *its){
	uint8_t i;
	uint8_t temp_msg=MSG_FREE;
    rt_mutex_take(its->msgLock, 500);
    
    temp_msg = its->msgData[0];
	its->msgData[0] = MSG_FREE;
    for(i=0;i<(USER_MSG_MAX_NUM-1);i++){
        its->msgData[i]=its->msgData[i+1];
        its->msgData[i+1]=MSG_FREE;
    }
    rt_mutex_release(its->msgLock);
	return temp_msg;

}

void clear_msg(MsgHandler_t *its){
    uint8_t i;
    for(i=0;i<USER_MSG_MAX_NUM;i++){
        its->msgData[i] = MSG_FREE;
    }
}

void init_msg(MsgHandler_t *its,char *msgName){
    uint8_t i;
    its->msgLock= rt_mutex_create(msgName, RT_IPC_FLAG_FIFO);
    rt_enter_critical();
    for(i=0;i<USER_MSG_MAX_NUM;i++){
        its->msgData[i] = MSG_FREE;
    }
    rt_exit_critical();
}



