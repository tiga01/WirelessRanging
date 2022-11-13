/*
STM32F103C8T6
iicͨ��0.96��OLED��ʾ������ʾ��Ӣ�ġ����֡�ͼƬ
18B20�¶ȴ�����
HC-SR04������������

OLED���߶���:
					VCC--3.3V/5V
					GND--GND
					SCL--PB8
					SDA--PB9
18B20���߶���:
					DQ--PA5 
HC-SR04���������߶���:
					Trig--PB7
					Echo--PB6
*/

#include "sys.h"  
#include "delay.h"
#include "oled_iic.h"
#include "stdio.h"
#include "wave.h"
#include "timer.h"
#include "usart.h"	  
#include "ds18b20.h" 

int main(void)
{		
	u8 a=0;	
	u8 t=20;//trig���Ÿߵ�ƽ����ʱ��
	short temperature;    	 		 		
	u8 Distance;
	
	
	Wave_SRD_Init();//��ʼ��������
	Timer1_Init(9998,7199);//��ʱ����ʼ��	

	extern const u8 BMP1[];
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	delay_init();
	OLED_Init();		 	//OLED��Ļ��ʼ��		
		
	
	while(Wave_SRD_Strat(t))
	{
	}
	
	OLED_Clear();	
	
	while(1)
	{		
		//18B20��ȡ�¶�		
		if(a%10==0)			//ÿ100ms��ȡһ��
		{									  
			temperature=DS18B20_Get_Temp();
			if(temperature<0)
			{
				OLED_ShowCH(50,6,"-");			//��ʾ����
				temperature=-temperature;			//תΪ����
			}else{
				OLED_ShowCH(50,6," ");			//ȥ������
			}
			OLED_ShowNum(60,6,temperature/10,2,16);			//��ʾ��������	    
   			OLED_ShowNum(60+28,6,temperature%10,1,16);	//��ʾС������ 		   
		}				   
	 	delay_ms(10);
		a++;
		if(a==20)
		{
			a=0;
		}

		OLED_ShowCH(15,0,"��Ļ��ʾ����");		//������ʾ����

		OLED_ShowCH(0,6,"����:");		//��ʾ���ģ�����
//		OLED_ShowCH(0,3,"Distance: ");	//��ʾӢ��
		OLED_ShowCH(100,6,"cm");		//������ʾ����

	

		//��������10Us�ĸߵ�ƽ��OLED����ʾ�������������		
		Wave_SRD_Strat(t);
		Distance = TIM_GetCounter(TIM1)*340/200.0;
		if(Distance>0)
		{
			OLED_ShowNum(50,6,Distance,6,16);		//��ʾ��������������
			delay_ms(10);
		}

/*		
		OLED_ShowCH(20,1,"��Ļ��ʾ����");		//������ʾ����

		OLED_ShowNum(20,3,314159265,9,0);		//������ʾ����

		OLED_DrawBMP(0,0,128,8,(u8 *)BMP1);		//������ʾͼƬ
*/
	}
}


