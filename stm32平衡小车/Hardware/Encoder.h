#ifndef __ENCODER_H
#define __ENCODER_H

void Encoder_TIM2_Init(void);
void Encoder_TIM4_Init(void);
void TIM4_IRQHandler(void);
void TIM2_IRQHandler(void);
int16_t TIM2_Encoder_Get(void);
int16_t TIM4_Encoder_Get(void);

#endif
