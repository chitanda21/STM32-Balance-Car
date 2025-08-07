#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Motor.h"
#include "Encoder.h"
#include "Timer.h"
#include "MPU6050.h"    
#include "inv_mpu.h"    
#include  "PID.h"  
//投降喵，DMP库移植参考这个视频https://www.bilibili.com/video/BV17z421f78T/?spm_id_from=333.337.search-card.all.click&vd_source=beb64341cfd158b1985800d97717b2c7
//不得不舍弃江科大的MPU6050代码了

int16_t Speed_Left;			//定义速度变量
int16_t Speed_Right;		//定义速度变量

float Angle;				//小车前后倾倒角度值

float angle_offset;

float Pitch,Roll,Yaw;								//俯仰角默认跟中值一样，翻滚角，偏航角
int16_t ax,ay,az,gx,gy,gz;							//加速度，陀螺仪角速度

u8 MPU_Get_Gyroscope(short *gx,short *gy,short *gz);
u8 MPU_Get_Accelerometer(short *ax,short *ay,short *az);


int16_t Speed_Average;		//速度，位置


/*宏定义参数*/
#define CENTER_ANGLE		-4	       //机械中值


#define Target_Speed        0	//期望速度（俯仰）。---二次开发接口，用于控制小车前进后退及其速度。




/*定义PID结构体变量*/
PID_t AnglePID = 
{					//直立环（内环）PID结构体变量，定义的时候同时给部分成员赋初值
	.Target = CENTER_ANGLE,	//角度环目标值初值设定为中心角度值
	.Kp = -5.2*0.6,					//比例项权重-5.2*0.6
	.Ki = 0,					//积分项权重
	.Kd = -58*0.6,					//微分项权重-58*0.6
	.OutMax = 100,				//输出限幅的最大值
	.OutMin = -100,				//输出限幅的最小值
};



PID_t SpeedPID = 
{					//速度环（外环）PID结构体变量，定义的时候同时给部分成员赋初值
	.Target = Target_Speed,			//速度环目标值初值设定为0
	.Kp = 0.078,					//比例项权重
	.Ki = 0.078/200,					//积分项权重
	.Kd = 0,					//微分项权重
	.OutMax = 100,				//输出限幅的最大值
	.OutMin = -100,				//输出限幅的最小值
};



int main(void)
{
	Encoder_TIM2_Init();
	Encoder_TIM4_Init();
	Timer3_Init();
	OLED_Init();
	Motor_Init();   
	OLED_Init();
	MPU6050_Init();     
	MPU6050_DMP_Init();
	MPU6050_Int_Init();
	
	while (1)
	{
		MPU6050_DMP_Get_Data(&Pitch,&Roll,&Yaw);				//读取姿态信息(其中偏航角有飘移是正常现象)
////		MPU_Get_Gyroscope(&gx,&gy,&gz);
////		MPU_Get_Accelerometer(&ax,&ay,&az);

//		OLED_ShowSignedNum(1,1,SpeedPID.Out,8);
//		OLED_ShowSignedNum(2,1,AnglePID.Out,8);
//		OLED_ShowSignedNum(3,1,AnglePID.Target,8);
		OLED_ShowSignedNum(4, 1, Roll, 5);

	}
}



//核心部分，pid的控制实现
void EXTI9_5_IRQHandler(void)
{
	/*定义静态变量（默认初值为0，函数退出后保留值和存储空间）*/
	static uint16_t Count1, Count2;		//分别用于角度环和位置环的计次分频
	
	 if(EXTI_GetITStatus(EXTI_Line5) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line5);
	
		Angle = Roll;
				
			/*内环计次分频*/
			/*角度环计次分频*/
			Count1 ++;				//计次自增
			if (Count1 >= 1)		//如果计次1次，则if成立，即if每隔1ms进一次
			{
				Count1 = 0;			//计次清零，便于下次计次
				// 添加角度滤波
				static float filtered_angle = 0;  // 静态变量，保留上一次滤波值
				filtered_angle = 0.85 * filtered_angle + 0.15 * Angle;
				
				/*以下进行角度环PID控制*/
				AnglePID.Actual = filtered_angle;
				PID_Update(&AnglePID);			//调用封装好的函数，一步完成PID计算和更新
		
				Motor_SetRightSpeed(AnglePID.Out);		//角度环的输出值给到电机PWM
				Motor_SetLeftSpeed(AnglePID.Out);		//角度环的输出值给到电机PWM
			}
			
			/*速度环计次分频*/
			Count2 ++;				//计次自增
			if (Count2 >= 5)		//如果计次5次，则if成立，即if每隔5ms进一次
			{
				Count2 = 0;			//计次清零，便于下次计次
				
				// 计算平均速度（添加滤波）
				static int16_t speed_avg_history[3] = {0};
				static uint8_t speed_index = 0;
            
				speed_avg_history[speed_index] = (Speed_Left + Speed_Right)/2;
				speed_index = (speed_index + 1) % 3;
              
				// 三阶移动平均滤波
				Speed_Average = (speed_avg_history[0] + speed_avg_history[1] + speed_avg_history[2]) / 3;
				
				
				/*以下进行位置环PID控制*/
				SpeedPID.Actual = Speed_Average;
				PID_Update(&SpeedPID);		//调用封装好的函数，一步完成PID计算和更新
				AnglePID.Target = CENTER_ANGLE + SpeedPID.Out;
			
//				AnglePID.Target = CENTER_ANGLE ;
		
			}

							
			
	
	}
	else				
	{
		Motor_SetRightSpeed(0);		//不执行PID程序且电机PWM直接设置为0，电机停止
		Motor_SetLeftSpeed(0);
	}	
}





/**
  * 函    数：TIM3中断函数
  * 参    数：无
  * 返 回 值：无
  * 注意事项：此函数为中断函数，无需调用，中断触发后自动执行
  *           函数名为预留的指定名称，可以从启动文件复制
  *           请确保函数名正确，不能有任何差异，否则中断函数将不能进入
  */
void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)		//判断是否是TIM2的更新事件触发的中断
	{
		Speed_Left = TIM2_Encoder_Get();	
		Speed_Right = TIM4_Encoder_Get();	//每隔固定时间段读取一次编码器计数增量值，即为速度值
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);			//清除TIM2更新事件的中断标志位
															//中断标志位必须清除
															//否则中断将连续不断地触发，导致主程序卡死
	}
}























///**
//  * 函    数：TIM2中断函数
//  * 参    数：无
//  * 返 回 值：无
//  * 注意事项：此函数为中断函数，无需调用，中断触发后自动执行
//  *           函数名为预留的指定名称，可以从启动文件复制
//  *           请确保函数名正确，不能有任何差异，否则中断函数将不能进入
//  */
//void TIM2_IRQHandler(void)
//{
//	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)		//判断是否是TIM2的更新事件触发的中断
//	{
//		Actual_Speed_Left = TIM2_Encoder_Get();							//每隔固定时间段读取一次编码器计数增量值，即为实际速度值
//		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);			//清除TIM2更新事件的中断标志位
//															//中断标志位必须清除
//															//否则中断将连续不断地触发，导致主程序卡死
//	}
//}




///**
//  * 函    数：TIM4中断函数
//  * 参    数：无
//  * 返 回 值：无
//  * 注意事项：此函数为中断函数，无需调用，中断触发后自动执行
//  *           函数名为预留的指定名称，可以从启动文件复制
//  *           请确保函数名正确，不能有任何差异，否则中断函数将不能进入
//  */
//void TIM4_IRQHandler(void)
//{
//	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)		//判断是否是TIM2的更新事件触发的中断
//	{
//		Actual_Speed_Right =  TIM4_Encoder_Get();								//每隔固定时间段读取一次编码器计数增量值，即为实际速度值
//		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);			//清除TIM2更新事件的中断标志位
//															//中断标志位必须清除
//															//否则中断将连续不断地触发，导致主程序卡死
//	}
//}

	
