#include "stm32f10x.h"                  // Device header
#include "PWM.h"

/**
  * 函    数：直流电机初始化
  * 参    数：无
  * 返 回 值：无
  */
void Motor_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//开启时钟
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;//初始化GPIO--PB12、PB13、PB14、PB15为推挽输出
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_12 |GPIO_Pin_13 |GPIO_Pin_14 |GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);	
	PWM_Init();													//初始化直流电机的底层PWM
}

/**
  * 函    数：直流电机设置速度
  * 参    数：Speed 要设置的速度，范围：-100~100
  * 返 回 值：无
  */
void Motor_SetLeftSpeed(int8_t Speed)
{
	if (Speed > 0)							
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_13);	
		GPIO_ResetBits(GPIOB, GPIO_Pin_12);	
		PWM_SetCompare4(Speed);				
	}
	else if(Speed == 0)						
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_13);	
		GPIO_SetBits(GPIOB, GPIO_Pin_12);	
		PWM_SetCompare4(Speed);										
	}
	else
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_13);	
		GPIO_SetBits(GPIOB, GPIO_Pin_12);	
		PWM_SetCompare4(-Speed);			
	}
}

void Motor_SetRightSpeed(int8_t Speed)
{
	if (Speed > 0)							
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_14);	
		GPIO_ResetBits(GPIOB, GPIO_Pin_15);	
		PWM_SetCompare1(Speed);				
	}
	else if(Speed == 0)						
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_14);	
		GPIO_SetBits(GPIOB, GPIO_Pin_15);	
		PWM_SetCompare1(Speed);				
	}
	else
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_14);	
		GPIO_SetBits(GPIOB, GPIO_Pin_15);	
		PWM_SetCompare1(-Speed);			
	}
}


//void Motor_SetLeftSpeed(int8_t Speed)
//{
//		GPIO_SetBits(GPIOB, GPIO_Pin_13);	
//		GPIO_ResetBits(GPIOB, GPIO_Pin_12);	
//		PWM_SetCompare4(Speed);				
//}

//void Motor_SetRightSpeed(int8_t Speed)
//{
//		GPIO_SetBits(GPIOB, GPIO_Pin_15);	
//		GPIO_ResetBits(GPIOB, GPIO_Pin_14);	
//		PWM_SetCompare1(Speed);				
//}
