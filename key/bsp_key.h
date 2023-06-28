#ifndef __KEY_H
#define	__KEY_H


#include "stm32f10x.h"

//  ���Ŷ��� 3r*7c
#define		KEY_ROW	3
#define		KEY_COL	7
#define    KEY_R0_GPIO_CLK     RCC_APB2Periph_GPIOA
#define    KEY_R0_GPIO_PORT    GPIOA			   
#define    KEY_R0_GPIO_PIN		 GPIO_Pin_3

#define    KEY_R1_GPIO_CLK     RCC_APB2Periph_GPIOA
#define    KEY_R1_GPIO_PORT    GPIOA		   
#define    KEY_R1_GPIO_PIN		  GPIO_Pin_2

#define    KEY_R2_GPIO_CLK     RCC_APB2Periph_GPIOA
#define    KEY_R2_GPIO_PORT    GPIOA			   
#define    KEY_R2_GPIO_PIN		 GPIO_Pin_1

#define    KEY_C0_GPIO_CLK     RCC_APB2Periph_GPIOC
#define    KEY_C0_GPIO_PORT    GPIOC		   
#define    KEY_C0_GPIO_PIN		  GPIO_Pin_14

#define    KEY_C1_GPIO_CLK     RCC_APB2Periph_GPIOC
#define    KEY_C1_GPIO_PORT    GPIOC			   
#define    KEY_C1_GPIO_PIN		 GPIO_Pin_13

#define    KEY_C2_GPIO_CLK     RCC_APB2Periph_GPIOB
#define    KEY_C2_GPIO_PORT    GPIOB		   
#define    KEY_C2_GPIO_PIN		  GPIO_Pin_9

#define    KEY_C3_GPIO_CLK     RCC_APB2Periph_GPIOB
#define    KEY_C3_GPIO_PORT    GPIOB			   
#define    KEY_C3_GPIO_PIN		 GPIO_Pin_8

#define    KEY_C4_GPIO_CLK     RCC_APB2Periph_GPIOB
#define    KEY_C4_GPIO_PORT    GPIOB			   
#define    KEY_C4_GPIO_PIN		 GPIO_Pin_5

#define    KEY_C5_GPIO_CLK     RCC_APB2Periph_GPIOB
#define    KEY_C5_GPIO_PORT    GPIOB		   
#define    KEY_C5_GPIO_PIN		  GPIO_Pin_4

#define    KEY_C6_GPIO_CLK     RCC_APB2Periph_GPIOB
#define    KEY_C6_GPIO_PORT    GPIOB			   
#define    KEY_C6_GPIO_PIN		 GPIO_Pin_3
	
//������̲�����
#define		KEY_SHAKE_DELAY 4	//����ʱ��
#if 0
#define		KEY_COL_TMP	KEY_COL			
#define		KEY_ALL_UP	0x7F
#define		KEY_ROW_1		0x7E
#define		KEY_ROW_2		0x7D
#define		KEY_ROW_3		0x7B
#define		KEY_COL_1		0x7E
#define		KEY_COL_2		0x7D
#define		KEY_COL_3		0x7B
#define		KEY_COL_4		0x77
#define		KEY_COL_5		0x6F
#define		KEY_COL_6		0x5F
#define		KEY_COL_7		0x3F
#else
#define		KEY_COL_TMP	5	/*KEY_COL_5/KEY_COL_6��������д������,��ʱ����*/
#define		KEY_ALL_UP	0x1F
#define		KEY_ROW_1		0x7E
#define		KEY_ROW_2		0x7D
#define		KEY_ROW_3		0x7B
#define		KEY_COL_1		0x1E
#define		KEY_COL_2		0x1D
#define		KEY_COL_3		0x1B
#define		KEY_COL_4		0x17
#define		KEY_COL_5		0x0F
#define		KEY_COL_6		0x5F
#define		KEY_COL_7		0x3F
#endif
 /** �������±��ú�
	*  ��������Ϊ�ߵ�ƽ������ KEY_ON=1�� KEY_OFF=0
	*  ����������Ϊ�͵�ƽ���Ѻ����ó�KEY_ON=0 ��KEY_OFF=1 ����
	*/
#define KEY_ON	1
#define KEY_OFF	0

void Key_GPIO_Config(void);
uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);
uint8_t GetKeyNum(void);

#endif /* __KEY_H */

