#include<stm32f4xx_rcc.h>
#include<stm32f4xx_tim.h>
#include<stm32f4xx_gpio.h>
#include<stm32f4xx_syscfg.h>

#include "tm_stm32f4_gpio.h"
#include "tm_stm32f4_lis302dl_lis3dsh.h"
#include "tm_stm32f4_pcd8544.h"
#include "tm_stm32f4_spi.h"
#include "misc.h"

#define WIDTH 30
#define LENGTH 20

typedef struct
{
int jest;
}bloczek;

bloczek tab[WIDTH][LENGTH];

int punkty=0;

TM_LIS302DL_LIS3DSH_Device_t Axes;
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TIM3_IRQHandler()//TIMER odpowiedzialny za prędkość snake'a
{


    if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
    TM_LIS302DL_LIS3DSH_ReadAxes(&Axes);
    //if(Axes.X)...
    //if(Axes.Y)...
    }


PCD8544_Clear();
TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TIM2_IRQHandler()//Odpowiedzialny za wyswietlanie i update wyswietlacza (f= 100 Hz)
{
             if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
             {

            	 int i,j;
            	 	for(j=0;j<LENGTH;j++)
            	 	{
            	 		 for(i=0;i<WIDTH;i++)
            	 	 	{
            	 		 PCD8544_GotoXY(i,j);
							if(tab[i][j].jest==1)
							{
            	 			//Funkcja do rysowania, nie zdecydowałem jaka "grubość" węża, i nie mam pojęcia jak animować głowę. Do pracy mam piksele, a żeby głowa była animowana to trzeba ich trochę zużyć, co wpłynie na "wielkość" samego snake'a.
            	 			PCD8544_Refresh();
            	 			}
            	 	    }
            	 	}


            	 	PCD8544_Refresh();
                    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
             }
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int main(void)
{
	SystemInit();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = 209;
	TIM_TimeBaseStructure.TIM_Prescaler = 249;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode =  TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure2;
	TIM_TimeBaseStructure2.TIM_Period = 8399;
	TIM_TimeBaseStructure2.TIM_Prescaler = 4999;
	TIM_TimeBaseStructure2.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure2.TIM_CounterMode =  TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure2);

	TIM_Cmd(TIM2, ENABLE);
	TIM_Cmd(TIM3, ENABLE);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	NVIC_InitTypeDef NVIC_InitStructure2;
	NVIC_InitStructure2.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure2.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure2.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure2.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure2);
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    PCD8544_Init(0x38);

	while(1){}












}
