#include "wave.h"
#include "delay.h"
#include "sys.h"

#define Trig GPIO_Pin_10//������������pa10
#define Echo GPIO_Pin_9//��������������pa9

void Wave_SRD_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	//��ʼ��������
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStruct.GPIO_Pin=Trig;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin=Echo;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPD;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	//�ⲿ�ж������ŵ����ã��ж���˿�9��ӳ��
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource9);
	//�ⲿ�ж�����
	EXTI_InitStruct.EXTI_Line=EXTI_Line9;
	EXTI_InitStruct.EXTI_LineCmd= ENABLE;
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitStruct);
	//�ж����ȼ�����
	NVIC_InitStruct.NVIC_IRQChannel=EXTI9_5_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_InitStruct);
	
	
}
//�ⲿ�жϷ�����
void EXTI9_5_IRQHandler(void)
{
	delay_us(10);
	if(EXTI_GetITStatus(EXTI_Line9)!=RESET)//Echo��⵽�ߵ�ƽ
	{
		TIM_SetCounter(TIM1,0);//������ʱ��
		TIM_Cmd(TIM1,ENABLE);//ʹ�ܶ�ʱ��
		while(GPIO_ReadInputDataBit(GPIOA,Echo));//�ȴ�Echo���ֵ͵�ƽ
		
		TIM_Cmd(TIM1,DISABLE);//�رն�ʱ��
		
	EXTI_ClearITPendingBit(EXTI_Line9);//����жϱ�־
  }
}
//��������������
//Trig����һ��������10us�ĸߵ�ƽ�ź�

u8 Wave_SRD_Strat(u8 time)
{
	if(time>10)
	{
	GPIO_SetBits(GPIOA,Trig);//����TrigΪ�ߵ�ƽ���
	delay_us(time);
	GPIO_ResetBits(GPIOA,Trig);
	return 0;//��������10us�ߵ�ƽ������0
	}
	else return 1;//Ϊ��������10us�ߵ�ƽ����1
}

