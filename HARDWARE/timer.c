#include "timer.h"
#include "sys.h"
//初始化定时器
void Timer1_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //????

  TIM_TimeBaseStructure.TIM_Period = arr; //?????????????
  TIM_TimeBaseStructure.TIM_Prescaler =(psc-1);//??????
  TIM_TimeBaseStructure.TIM_ClockDivision = 0; //??????
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//??????
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;//??????
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //?????
  TIM_ClearFlag(TIM1, TIM_FLAG_Update);//??????

  TIM_ITConfig(      //?????????TIM??
    TIM1,            //TIM1
    TIM_IT_Update  | //TIM ?????
    TIM_IT_Trigger,  //TIM ????? 
    ENABLE  	     //??
    );
	
  
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//?????0?
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  	   //????0?
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 

  TIM_Cmd(TIM1, ENABLE); 
}

//中断服务函数
void TIM1_UP_IRQHandler(void) 
{ 	    	  	     
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)//?????TIM??????:TIM ??? 
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);//??TIMx???????:TIM ??? 
/***************???????????******************/

	}	     
} 























