#ifndef	__SOFTWARETIMER_H__
#define __SOFTWARETIMER_H__
#include "main.h"


#define GetTimerTick	HAL_GetTick()

typedef void callback(uint32_t argv);


typedef enum tmrState {
	SOFT_TIMER_STOPPED = 0,  //ֹͣ
	SOFT_TIMER_RUNNING,      //����
	SOFT_TIMER_TIMEOUT       //��ʱ
}tmrState;

typedef enum tmrMode {
	MODE_ONE_SHOT = 0,       //����ģʽ
	MODE_PERIODIC,           //����ģʽ
}tmrMode;

typedef struct softTimer {
	struct softTimer *next;		
	tmrState state;           //״̬
	tmrMode mode;            //ģʽ
	uint32_t match;          //����ʱ��
	uint32_t period;         //��ʱ����
	callback *cb;            //�ص�����ָ��
	uint32_t argv;              //����ָ��
}SoftTimer;

SoftTimer *SoftTimer_Creat(tmrMode mod,uint32_t mat,callback *cb,uint32_t argv);
uint8_t SoftTimer_Start(SoftTimer *timer);
uint8_t SoftTimer_Stop(SoftTimer *timer);
void SoftTimer_Process(void);

#endif

