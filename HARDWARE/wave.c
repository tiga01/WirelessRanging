#include "wave.h"
#include "delay.h"
#include "sys.h"

#define Trig GPIO_Pin_10//超声发射引脚pa10
#define Echo GPIO_Pin_9//超声波接受引脚pa9

void Wave_SRD_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	//初始化超声波
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStruct.GPIO_Pin=Trig;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin=Echo;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPD;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	//外部中断与引脚的配置，中断与端口9的映射
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource9);
	//外部中断配置
	EXTI_InitStruct.EXTI_Line=EXTI_Line9;
	EXTI_InitStruct.EXTI_LineCmd= ENABLE;
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitStruct);
	//中断优先级管理
	NVIC_InitStruct.NVIC_IRQChannel=EXTI9_5_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_InitStruct);
	
	
}
//外部中断服务函数
void EXTI9_5_IRQHandler(void)
{
	delay_us(10);
	if(EXTI_GetITStatus(EXTI_Line9)!=RESET)//Echo检测到高电平
	{
		TIM_SetCounter(TIM1,0);//开启定时器
		TIM_Cmd(TIM1,ENABLE);//使能定时器
		while(GPIO_ReadInputDataBit(GPIOA,Echo));//等待Echo出现低电平
		
		TIM_Cmd(TIM1,DISABLE);//关闭定时器
		
	EXTI_ClearITPendingBit(EXTI_Line9);//清除中断标志
  }
}
//超声波工作条件
//Trig产生一个不低于10us的高电平信号

u8 Wave_SRD_Strat(u8 time)
{
	if(time>10)
	{
	GPIO_SetBits(GPIOA,Trig);//设置Trig为高电平输出
	delay_us(time);
	GPIO_ResetBits(GPIOA,Trig);
	return 0;//产生高于10us高电平，返回0
	}
	else return 1;//为产生高于10us高电平返回1
}

