#include "SoftTimer.h"
#include "string.h"
#include "stdlib.h"

static SoftTimer *SoftTimerHeader;


/*
	回调函数
*/
callback *CallBackFun;

/*
	功能：软件定时器创建
	参数：tmrMode：模式 MODE_ONE_SHOT MODE_PERIODIC
				mat:定时周期
				cb:函数
				argv:参数
	返回值：创建定时器的指针
*/
SoftTimer *SoftTimer_Creat(tmrMode mod,uint32_t mat,callback *cb,uint32_t argv){
	if(SoftTimerHeader==NULL){

 		SoftTimerHeader=(SoftTimer *)malloc(sizeof(SoftTimer));
		SoftTimerHeader->mode=mod;
		SoftTimerHeader->period=mat;
		SoftTimerHeader->match=GetTimerTick+mat;
		SoftTimerHeader->cb=cb;
		SoftTimerHeader->argv=argv;
		SoftTimerHeader->next=NULL;
		return SoftTimerHeader;
	}else{
		SoftTimer *temp=SoftTimerHeader;
		while(temp->next!=NULL){
			temp=temp->next;
		}
		SoftTimer *new_timer=(SoftTimer *)malloc(sizeof(SoftTimer));
		new_timer->mode=mod;
		new_timer->period=mat;
		new_timer->match=GetTimerTick+mat;
		new_timer->cb=cb;
		new_timer->argv=argv;
		new_timer->next=NULL;
		temp->next=new_timer;
		return new_timer;
	}
}
/*
	软件定时器启动
	参数：timer:定时器指针
	返回值：0失败 1成功
				
*/
uint8_t SoftTimer_Start(SoftTimer *timer){
	if(timer==NULL){
		printf("[error] timer not found!\r\n");
		return 0;
	}else{
		timer->state=SOFT_TIMER_RUNNING;
	}
	return 1;
}
/*
	软件定时器停止
	参数：timer:定时器指针
	返回值：0失败 1成功
*/
uint8_t SoftTimer_Stop(SoftTimer *timer){
	if(timer==NULL){
		printf("[error] timer not found!\r\n");
		return 0;
	}else{
		timer->state=SOFT_TIMER_TIMEOUT;
	}
	return 1;
}
/*
	软件定时器删除
*/
uint8_t SoftTimer_Delete(SoftTimer *timer){
	SoftTimer *tempBuffer=SoftTimerHeader;
	while(tempBuffer!=NULL&&tempBuffer->next!=NULL){
		if(tempBuffer->next==timer){
			SoftTimer *buffer=tempBuffer->next;
			tempBuffer->next=buffer->next;
			free(buffer);
			return 1;
		}
	}
	return 0;
}
/*
	定时器处理函数
*/
void SoftTimer_Process(void){
	SoftTimer *processTemp=SoftTimerHeader;
	while(processTemp!=NULL){
		switch(processTemp->state){
			case SOFT_TIMER_STOPPED:
				
				break;
			case SOFT_TIMER_RUNNING:
				if((processTemp->match)<=GetTimerTick){
					processTemp->state=SOFT_TIMER_TIMEOUT;
					CallBackFun=processTemp->cb;
					CallBackFun(processTemp->argv);
				}
				break;
			case SOFT_TIMER_TIMEOUT:
				if(processTemp->mode==MODE_ONE_SHOT){
					processTemp->state=SOFT_TIMER_STOPPED;
				}else{
					processTemp->match=GetTimerTick+processTemp->period;
					processTemp->state=SOFT_TIMER_RUNNING;
				}
				break;
		}
		processTemp=processTemp->next;
	}

}
