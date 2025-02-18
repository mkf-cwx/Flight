#include "usart.h"


int fputc(int ch, FILE *stream)
{
    /* �����жϴ����Ƿ������ */
    while((USART1->SR & 0X40) == 0);

    /* ���ڷ�����ɣ������ַ����� */
    USART1->DR = (uint8_t) ch;

    return ch;
}

/*******************************************************************************
* �� �� ��         : USART1_Init
* ��������		   : USART1��ʼ������
* ��    ��         : bound:������
* ��    ��         : ��
*******************************************************************************/ 
void USART1_Init(uint32_t bound)
{
   //GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
 	//GPIOA9����ΪUSART1 
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); 
	
	/*  ����GPIO��ģʽ��IO�� */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;// Tx			   // �������PA9
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;	  // ���ù���  
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	// �����������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; // ����
	GPIO_Init(GPIOA,&GPIO_InitStructure);  /* ��ʼ����������IO */

	// USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;// ����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;// �ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;// һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;// ����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;// ��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Tx;	// �շ�ģʽ
	USART_Init(USART1, &USART_InitStructure); // ��ʼ������1
	
	USART_Cmd(USART1, ENABLE);  // ʹ�ܴ���1 
	
}

