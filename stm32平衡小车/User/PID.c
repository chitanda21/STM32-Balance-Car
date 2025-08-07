//#include "stm32f10x.h"                  // Device header
//#include "PID.h"

///**
//  * 函    数：PID计算及结构体变量值更新
//  * 参    数：PID_t * 指定结构体的地址
//  * 返 回 值：无
//  */
//void PID_Update(PID_t *p)
//{
//	/*获取本次误差和上次误差*/
//	p->Error1 = p->Error0;					//获取上次误差
//	p->Error0 = p->Target - p->Actual;		//获取本次误差，目标值减实际值，即为误差值
//	
//	/*外环误差积分（累加）*/
//	/*如果Ki不为0，才进行误差积分，这样做的目的是便于调试*/
//	/*因为在调试时，我们可能先把Ki设置为0，这时积分项无作用，误差消除不了，误差积分会积累到很大的值*/
//	/*后续一旦Ki不为0，那么因为误差积分已经积累到很大的值了，这就导致积分项疯狂输出，不利于调试*/
//	if (p->Ki != 0)					//如果Ki不为0
//	{
//		p->ErrorInt += p->Error0;	//进行误差积分
//	}
//	else							//否则
//	{
//		p->ErrorInt = 0;			//误差积分直接归0
//	}
//	
//	/*PID计算*/
//	/*使用位置式PID公式，计算得到输出值*/
//	p->Out = p->Kp * p->Error0
//		   + p->Ki * p->ErrorInt
//		   + p->Kd * (p->Error0 - p->Error1);
//	
//	/*输出限幅*/
//	if (p->Out > p->OutMax) {p->Out = p->OutMax;}	//限制输出值最大为结构体指定的OutMax
//	if (p->Out < p->OutMin) {p->Out = p->OutMin;}	//限制输出值最小为结构体指定的OutMin
//}


#include "stm32f10x.h"
#include "PID.h"

// 新增全局常量定义（根据实际需求调整）
#define MAX_INTEGRAL 1000.0f       // 积分项最大限幅值
#define INTEGRAL_LEAKAGE 0.98f     // 积分泄漏系数（0.98表示每次保留98%）

/**
  * 函    数：PID计算及结构体变量值更新（带积分限幅和泄漏）
  * 参    数：PID_t * 指定结构体的地址
  * 返 回 值：无
  */
void PID_Update(PID_t *p)
{
    /* 1. 获取本次误差和上次误差 */
    p->Error1 = p->Error0;                  // 获取上次误差
    p->Error0 = p->Target - p->Actual;      // 获取本次误差
    
    /* 2. 积分项处理（带限幅和泄漏）*/
    if (p->Ki != 0) {
        // 只在误差合理范围内进行积分（防止突发干扰导致积分饱和）
        if (fabs(p->Error0) < 15.0f) {      // 误差绝对值小于15时才积分
            p->ErrorInt += p->Error0;       // 累加误差积分
        }
        
        // 积分限幅（关键防饱和措施）
        if (p->ErrorInt > MAX_INTEGRAL) {
            p->ErrorInt = MAX_INTEGRAL;
        } else if (p->ErrorInt < -MAX_INTEGRAL) {
            p->ErrorInt = -MAX_INTEGRAL;
        }
        
        // 积分泄漏（防止长期累积）
        p->ErrorInt *= INTEGRAL_LEAKAGE;    // 每次减少2%的积分值
    } else {
        p->ErrorInt = 0;                    // Ki=0时清零积分
    }
    
    /* 3. PID计算 */
    p->Out = p->Kp * p->Error0
           + p->Ki * p->ErrorInt
           + p->Kd * (p->Error0 - p->Error1);
    
    /* 4. 输出限幅 */
    if (p->Out > p->OutMax) {
        p->Out = p->OutMax;
    }
    if (p->Out < p->OutMin) {
        p->Out = p->OutMin;
    }
}



