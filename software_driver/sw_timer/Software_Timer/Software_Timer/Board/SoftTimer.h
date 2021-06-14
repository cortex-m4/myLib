#ifndef	__SOFTWARETIMER_H__
#define __SOFTWARETIMER_H__
#include "main.h"


#define GetTimerTick	HAL_GetTick()

typedef void callback(uint32_t argv);


typedef enum tmrState {
	SOFT_TIMER_STOPPED = 0,  //停止
	SOFT_TIMER_RUNNING,      //运行
	SOFT_TIMER_TIMEOUT       //超时
}tmrState;

typedef enum tmrMode {
	MODE_ONE_SHOT = 0,       //单次模式
	MODE_PERIODIC,           //周期模式
}tmrMode;

typedef struct softTimer {
	struct softTimer *next;		
	tmrState state;           //状态
	tmrMode mode;            //模式
	uint32_t match;          //到期时间
	uint32_t period;         //定时周期
	callback *cb;            //回调函数指针
	uint32_t argv;              //参数指针
}SoftTimer;

SoftTimer *SoftTimer_Creat(tmrMode mod,uint32_t mat,callback *cb,uint32_t argv);
uint8_t SoftTimer_Start(SoftTimer *timer);
uint8_t SoftTimer_Stop(SoftTimer *timer);
void SoftTimer_Process(void);

#endif

