#include "stm32f10x.h"                  // Device header



void Encoder_TIM2_Init(void)
{
//	GPIO_InitTypeDef GPIO_InitStruct;
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
//	TIM_ICInitTypeDef TIM_ICInitStruct;
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//����ʱ��
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
//	
//	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;//��ʼ��GPIO--PA0��PA1
//	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0 |GPIO_Pin_1;
//	GPIO_Init(GPIOA,&GPIO_InitStruct);
//	
//	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);//��ʼ����ʱ����
//	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
//	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
//	TIM_TimeBaseInitStruct.TIM_Period=65535;
//	TIM_TimeBaseInitStruct.TIM_Prescaler=0;
//	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
//	
//	TIM_EncoderInterfaceConfig(TIM2,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);//���ñ�����ģʽ
//	
//	TIM_ICStructInit(&TIM_ICInitStruct);//��ʼ�����벶��
//	TIM_ICInitStruct.TIM_ICFilter=10;
//	TIM_ICInit(TIM2,&TIM_ICInitStruct);
//	
//	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);//������������жϱ�־λ
//	
//	TIM_SetCounter(TIM2,0);//���㶨ʱ������ֵ
//	
//	TIM_Cmd(TIM2,ENABLE);//������ʱ��
/*����ʱ��*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);			//����TIM2��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);			//����GPIOA��ʱ��
	
	/*GPIO��ʼ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);							//��PA0��PA1���ų�ʼ��Ϊ��������
	
	/*ʱ����Ԫ��ʼ��*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;				//����ṹ�����
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //ʱ�ӷ�Ƶ��ѡ�񲻷�Ƶ���˲������������˲���ʱ�ӣ���Ӱ��ʱ����Ԫ����
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //������ģʽ��ѡ�����ϼ���
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;               //�������ڣ���ARR��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;                //Ԥ��Ƶ������PSC��ֵ
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;            //�ظ����������߼���ʱ���Ż��õ�
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);             //���ṹ���������TIM_TimeBaseInit������TIM2��ʱ����Ԫ
	
	/*���벶���ʼ��*/
	TIM_ICInitTypeDef TIM_ICInitStructure;							//����ṹ�����
	TIM_ICStructInit(&TIM_ICInitStructure);							//�ṹ���ʼ�������ṹ��û��������ֵ
																	//�����ִ�д˺��������ṹ�����г�Ա����һ��Ĭ��ֵ
																	//����ṹ���ֵ��ȷ��������
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;				//ѡ�����ö�ʱ��ͨ��1
	TIM_ICInitStructure.TIM_ICFilter = 0xF;							//�����˲������������Թ����źŶ���
	TIM_ICInit(TIM2, &TIM_ICInitStructure);							//���ṹ���������TIM_ICInit������TIM3�����벶��ͨ��
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;				//ѡ�����ö�ʱ��ͨ��2
	TIM_ICInitStructure.TIM_ICFilter = 0xF;							//�����˲������������Թ����źŶ���
	TIM_ICInit(TIM2, &TIM_ICInitStructure);							//���ṹ���������TIM_ICInit������TIM3�����벶��ͨ��
	
	/*�������ӿ�����*/
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
																	//���ñ�����ģʽ�Լ���������ͨ���Ƿ���
																	//ע���ʱ������Rising��Falling�Ѿ������������غ��½����ˣ����Ǵ����Ƿ���
																	//�˺������������벶���ʼ��֮����У��������벶������ûḲ�Ǵ˺����Ĳ�������
//																	
//																		/*�ж��������*/
//	TIM_ClearFlag(TIM2, TIM_FLAG_Update);						//�����ʱ�����±�־λ
//																//TIM_TimeBaseInit����ĩβ���ֶ������˸����¼�
//																//��������˱�־λ�������жϺ󣬻����̽���һ���ж�
//																//�������������⣬������˱�־λҲ��
//	
//	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);					//����TIM2�ĸ����ж�
//	
//	/*NVIC�жϷ���*/
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);				//����NVICΪ����2
//																//����ռ���ȼ���Χ��0~3����Ӧ���ȼ���Χ��0~3
//																//�˷������������������н������һ��
//																//���ж���жϣ����԰Ѵ˴������main�����ڣ�whileѭ��֮ǰ
//																//�����ö�����÷���Ĵ��룬���ִ�е����ûḲ����ִ�е�����
//	
//	/*NVIC����*/
//	NVIC_InitTypeDef NVIC_InitStructure;						//����ṹ�����
//	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;				//ѡ������NVIC��TIM2��
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//ָ��NVIC��·ʹ��
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//ָ��NVIC��·����ռ���ȼ�Ϊ2
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			//ָ��NVIC��·����Ӧ���ȼ�Ϊ1
//	NVIC_Init(&NVIC_InitStructure);								//���ṹ���������NVIC_Init������NVIC����
//																	
//	
	/*TIMʹ��*/
	TIM_Cmd(TIM2, ENABLE);			//ʹ��TIM2����ʱ����ʼ����
}


void Encoder_TIM4_Init(void)
{
//	GPIO_InitTypeDef GPIO_InitStruct;
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
//	TIM_ICInitTypeDef TIM_ICInitStruct;
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
//	
//	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;
//	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6 |GPIO_Pin_7;
//	GPIO_Init(GPIOB,&GPIO_InitStruct);
//	
//	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
//	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
//	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
//	TIM_TimeBaseInitStruct.TIM_Period=65535;
//	TIM_TimeBaseInitStruct.TIM_Prescaler=0;
//	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStruct);
//	
//	TIM_EncoderInterfaceConfig(TIM4,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
//	
//	TIM_ICStructInit(&TIM_ICInitStruct);
//	TIM_ICInitStruct.TIM_ICFilter=10;
//	TIM_ICInit(TIM4,&TIM_ICInitStruct);
//	
//	TIM_ClearFlag(TIM4,TIM_FLAG_Update);
//	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
//	
//	TIM_SetCounter(TIM4,0);

//	TIM_Cmd(TIM4,ENABLE);


	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);			//����TIM4��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);			//����GPIOB��ʱ��
	
	/*GPIO��ʼ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);							//��PB6��PB7���ų�ʼ��Ϊ��������
	
	/*ʱ����Ԫ��ʼ��*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;				//����ṹ�����
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //ʱ�ӷ�Ƶ��ѡ�񲻷�Ƶ���˲������������˲���ʱ�ӣ���Ӱ��ʱ����Ԫ����
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //������ģʽ��ѡ�����ϼ���
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;               //�������ڣ���ARR��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;                //Ԥ��Ƶ������PSC��ֵ
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;            //�ظ����������߼���ʱ���Ż��õ�
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);             //���ṹ���������TIM_TimeBaseInit������TIM2��ʱ����Ԫ
	
	/*���벶���ʼ��*/
	TIM_ICInitTypeDef TIM_ICInitStructure;							//����ṹ�����
	TIM_ICStructInit(&TIM_ICInitStructure);							//�ṹ���ʼ�������ṹ��û��������ֵ
																	//�����ִ�д˺��������ṹ�����г�Ա����һ��Ĭ��ֵ
																	//����ṹ���ֵ��ȷ��������
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;				//ѡ�����ö�ʱ��ͨ��1
	TIM_ICInitStructure.TIM_ICFilter = 0xF;							//�����˲������������Թ����źŶ���
	TIM_ICInit(TIM4, &TIM_ICInitStructure);							//���ṹ���������TIM_ICInit������TIM3�����벶��ͨ��
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;				//ѡ�����ö�ʱ��ͨ��2
	TIM_ICInitStructure.TIM_ICFilter = 0xF;							//�����˲������������Թ����źŶ���
	TIM_ICInit(TIM4, &TIM_ICInitStructure);							//���ṹ���������TIM_ICInit������TIM3�����벶��ͨ��
	
	/*�������ӿ�����*/
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
																	//���ñ�����ģʽ�Լ���������ͨ���Ƿ���
																	//ע���ʱ������Rising��Falling�Ѿ������������غ��½����ˣ����Ǵ����Ƿ���
																	//�˺������������벶���ʼ��֮����У��������벶������ûḲ�Ǵ˺����Ĳ�������
																	
//																	
//																		/*�ж��������*/
//	TIM_ClearFlag(TIM4, TIM_FLAG_Update);						//�����ʱ�����±�־λ
//																//TIM_TimeBaseInit����ĩβ���ֶ������˸����¼�
//																//��������˱�־λ�������жϺ󣬻����̽���һ���ж�
//																//�������������⣬������˱�־λҲ��
//	
//	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);					//����TIM2�ĸ����ж�
//	
//	/*NVIC�жϷ���*/
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);				//����NVICΪ����2
//																//����ռ���ȼ���Χ��0~3����Ӧ���ȼ���Χ��0~3
//																//�˷������������������н������һ��
//																//���ж���жϣ����԰Ѵ˴������main�����ڣ�whileѭ��֮ǰ
//																//�����ö�����÷���Ĵ��룬���ִ�е����ûḲ����ִ�е�����
//	
//	/*NVIC����*/
//	NVIC_InitTypeDef NVIC_InitStructure;						//����ṹ�����
//	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;				//ѡ������NVIC��TIM2��
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//ָ��NVIC��·ʹ��
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//ָ��NVIC��·����ռ���ȼ�Ϊ2
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			//ָ��NVIC��·����Ӧ���ȼ�Ϊ1
//	NVIC_Init(&NVIC_InitStructure);								//���ṹ���������NVIC_Init������NVIC����

	/*TIMʹ��*/
	TIM_Cmd(TIM4, ENABLE);			//ʹ��TIM2����ʱ����ʼ����
}








/**
  * ��    ������ȡ������������ֵ
  * ��    ������
  * �� �� ֵ�����ϴ˵��ô˺����󣬱�����������ֵ
  */

int16_t TIM2_Encoder_Get(void)
{
	int16_t Temp;
	Temp = TIM_GetCounter(TIM2);
	TIM_SetCounter(TIM2, 0);
	return Temp;
}

int16_t TIM4_Encoder_Get(void)
{
	int16_t Temp;
	Temp = TIM_GetCounter(TIM4);
	TIM_SetCounter(TIM4, 0);
	return Temp;
}

