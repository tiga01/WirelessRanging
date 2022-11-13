/*
STM32F103C8T6
iic通信0.96寸OLED显示屏；显示中英文、数字、图片
18B20温度传感器
HC-SR04超声波传感器

OLED接线定义:
					VCC--3.3V/5V
					GND--GND
					SCL--PB8
					SDA--PB9
18B20接线定义:
					DQ--PA5 
HC-SR04超声波接线定义:
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
	u8 t=20;//trig引脚高电平持续时间
	short temperature;    	 		 		
	u8 Distance;
	
	
	Wave_SRD_Init();//初始化超声波
	Timer1_Init(9998,7199);//定时器初始化	

	extern const u8 BMP1[];
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	delay_init();
	OLED_Init();		 	//OLED屏幕初始化		
		
	
	while(Wave_SRD_Strat(t))
	{
	}
	
	OLED_Clear();	
	
	while(1)
	{		
		//18B20读取温度		
		if(a%10==0)			//每100ms读取一次
		{									  
			temperature=DS18B20_Get_Temp();
			if(temperature<0)
			{
				OLED_ShowCH(50,6,"-");			//显示负号
				temperature=-temperature;			//转为正数
			}else{
				OLED_ShowCH(50,6," ");			//去掉负号
			}
			OLED_ShowNum(60,6,temperature/10,2,16);			//显示整数部分	    
   			OLED_ShowNum(60+28,6,temperature%10,1,16);	//显示小数部分 		   
		}				   
	 	delay_ms(10);
		a++;
		if(a==20)
		{
			a=0;
		}

		OLED_ShowCH(15,0,"屏幕显示测试");		//测试显示中文

		OLED_ShowCH(0,6,"距离:");		//显示中文：距离
//		OLED_ShowCH(0,3,"Distance: ");	//显示英文
		OLED_ShowCH(100,6,"cm");		//测试显示中文

	

		//产生高于10Us的高电平与OLED上显示超声波所测距离		
		Wave_SRD_Strat(t);
		Distance = TIM_GetCounter(TIM1)*340/200.0;
		if(Distance>0)
		{
			OLED_ShowNum(50,6,Distance,6,16);		//显示超声波测量距离
			delay_ms(10);
		}

/*		
		OLED_ShowCH(20,1,"屏幕显示测试");		//测试显示中文

		OLED_ShowNum(20,3,314159265,9,0);		//测试显示数字

		OLED_DrawBMP(0,0,128,8,(u8 *)BMP1);		//测试显示图片
*/
	}
}


