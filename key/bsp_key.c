/**
  ******************************************************************************
  * @file    bsp_key.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   ����Ӧ��bsp��ɨ��ģʽ��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:���� F103-�Ե� STM32 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
  
#include "bsp_key.h"  
#include "data_help.h"
#include "delay.h"

	/*GPIO����ȫ�ֱ���*/
	uint32_t KEY_R_GPIO_CLK_Init = 0;
	uint32_t KEY_C_GPIO_CLK_Init = 0;
	GPIO_TypeDef *KEY_R_GPIO_PORT_Init = KEY_R0_GPIO_PORT;;
	GPIO_TypeDef *KEY_C1_GPIO_PORT_Init = KEY_C0_GPIO_PORT;//ǰ����ΪGPIOC
	GPIO_TypeDef *KEY_C2_GPIO_PORT_Init = KEY_C2_GPIO_PORT;//������ΪGPIOB
	GPIO_TypeDef * KEY_R_GPIO_PORT[KEY_ROW] ={KEY_R0_GPIO_PORT,KEY_R1_GPIO_PORT,KEY_R2_GPIO_PORT};
	GPIO_TypeDef * KEY_C_GPIO_PORT[KEY_COL] ={KEY_C0_GPIO_PORT,KEY_C1_GPIO_PORT,KEY_C2_GPIO_PORT,KEY_C3_GPIO_PORT,KEY_C4_GPIO_PORT,KEY_C5_GPIO_PORT,KEY_C6_GPIO_PORT};
	uint16_t KEY_R_GPIO_PIN_Init=0;
	uint16_t KEY_C1_GPIO_PIN_Init=0;
	uint16_t KEY_C2_GPIO_PIN_Init=0;
	uint16_t KEY_R_GPIO_PIN[KEY_ROW] ={KEY_R0_GPIO_PIN,KEY_R1_GPIO_PIN,KEY_R2_GPIO_PIN};
	uint16_t KEY_C_GPIO_PIN[KEY_COL] ={KEY_C0_GPIO_PIN,KEY_C1_GPIO_PIN,KEY_C2_GPIO_PIN,KEY_C3_GPIO_PIN,KEY_C4_GPIO_PIN,KEY_C5_GPIO_PIN,KEY_C6_GPIO_PIN};
	
/**
  * @brief  ��ȡ�����õ�������I/O�ڲ���
  * @param  ��
  * @retval ��
  */
void Key_GPIO_Para_Init()
{
	int i=0;
	/*���󰴼� - �޸�����ʱ�ڱ��ļ��ڽ��޸Ĵ˴�*/
	uint32_t KEY_R_GPIO_CLK[KEY_ROW] = {KEY_R0_GPIO_CLK,KEY_R1_GPIO_CLK,KEY_R2_GPIO_CLK};
	uint32_t KEY_C_GPIO_CLK[KEY_COL] = {KEY_C0_GPIO_CLK,KEY_C1_GPIO_CLK,KEY_C2_GPIO_CLK,KEY_C3_GPIO_CLK,KEY_C4_GPIO_CLK,KEY_C5_GPIO_CLK,KEY_C6_GPIO_CLK};
	

	for(i=0;i<KEY_ROW;i++){
		//�����˿ڵ�ʱ��
		KEY_R_GPIO_CLK_Init = KEY_R_GPIO_CLK_Init|KEY_R_GPIO_CLK[i];
		//����������
		KEY_R_GPIO_PIN_Init = KEY_R_GPIO_PIN_Init|KEY_R_GPIO_PIN[i]; 
	}
	
	for(i=0;i<KEY_COL;i++){
		//�����˿ڵ�ʱ��
		KEY_C_GPIO_CLK_Init = KEY_C_GPIO_CLK_Init|KEY_C_GPIO_CLK[i];
		//���������� -- ǰ����ΪGPIOC,������ΪGPIOB
		if(i<2) KEY_C1_GPIO_PIN_Init = KEY_C1_GPIO_PIN_Init|KEY_C_GPIO_PIN[i]; 
		else	KEY_C2_GPIO_PIN_Init = KEY_C2_GPIO_PIN_Init|KEY_C_GPIO_PIN[i]; 
	}
}

/**
  * @brief  �����·������õ���I/O��
  * @param  ��
  * @retval ��
  */
void Key_GPIO_Config(void)
{	
	GPIO_InitTypeDef GPIO_R_InitStructure;
	GPIO_InitTypeDef GPIO_C1_InitStructure;
	GPIO_InitTypeDef GPIO_C2_InitStructure;
	
/*��ȡʵ���·��Ĳ���*/
	Key_GPIO_Para_Init();
	
/*������Ҫ�·��Ĳ���*/
	//����������
	GPIO_R_InitStructure.GPIO_Pin = KEY_R_GPIO_PIN_Init; 
	//�����а���Ϊ�������
	GPIO_R_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_R_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
	
	//���������� -- ǰ����ΪGPIOC,������ΪGPIOB
	GPIO_C1_InitStructure.GPIO_Pin = KEY_C1_GPIO_PIN_Init; 
	GPIO_C2_InitStructure.GPIO_Pin = KEY_C2_GPIO_PIN_Init; 	
	//�����а���Ϊ��������
	GPIO_C1_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_C2_InitStructure.GPIO_Mode = GPIO_Mode_IPU;


/*��ʼ��ʱ�ӺͰ���*/
	//���������˿ڵ�ʱ��
	RCC_APB2PeriphClockCmd(KEY_R_GPIO_CLK_Init|KEY_C_GPIO_CLK_Init,ENABLE);
	
	//ʹ�ýṹ���ʼ������
	GPIO_Init(KEY_R_GPIO_PORT_Init, &GPIO_R_InitStructure);
	GPIO_Init(KEY_C1_GPIO_PORT_Init, &GPIO_C1_InitStructure);
	GPIO_Init(KEY_C2_GPIO_PORT_Init, &GPIO_C2_InitStructure);
}

 /*
 * ��������Key_Scan
 * ����  ������Ƿ��а�������
 * ����  ��GPIOx��x ������ A��B��C��D���� E
 *		     GPIO_Pin������ȡ�Ķ˿�λ 	
 * ���  ��KEY_OFF(û���°���)��KEY_ON�����°�����
 */
uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{			
	/*����Ƿ��а������� */
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON )  
	{	 
		/*�ȴ������ͷ� */
		//while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON);   
		return 	KEY_ON;	 
	}
	else
		return KEY_OFF;
}
/*
 * ��������Get_Cow_Value
 * ����  ����������������ɵ�ֵ�����򰴴ӵ�λ����λ����
 * ����  ��
 * ���  ������ֵ
 */
uint8_t Get_Col_Value()
{
	uint8_t col=0;
	uint8_t i=0;
	
	for(i=0;i<KEY_COL_TMP/*KEY_COL*/;i++){
		col = col | (GPIO_ReadInputDataBit(KEY_C_GPIO_PORT[i],KEY_C_GPIO_PIN[i])<<i);
	}
	
	return col;
}

/*
 * ��������Get_Cow_Index
 * ����  ����ñ�ѡ�е����ߣ�δ������ʱ���ߵ�ֵ1�����º�Ϊ0��
 * ����  ��
 * ���  ��������ţ�����0ʱ�����߱�ѡ��
 */
uint8_t Get_Col_Index(uint8_t col)
{
	if(col!=KEY_ALL_UP){
		Delay(KEY_SHAKE_DELAY);//����
		col = Get_Col_Value();
		if(col!=KEY_ALL_UP){//�жϱ�ѡ�е�����
			switch(col){
				case KEY_COL_1:
					return 1;
				case KEY_COL_2:
					return 2;
				case KEY_COL_3:
					return 3;
				case KEY_COL_4:
					return 4;
				case KEY_COL_5:
					return 5;
				case KEY_COL_6:
					return 6;
				case KEY_COL_7:
					return 7;
				default:
					return 0;
			}
		}
	}
	return 0;
}

uint8_t Get_Row_Index()
{
	uint8_t col=0;
	uint8_t i;
	
	for(i=0;i<KEY_ROW;i++){
		GPIO_WriteBit(KEY_R_GPIO_PORT_Init,KEY_R_GPIO_PIN_Init,Bit_SET);
		GPIO_WriteBit(KEY_R_GPIO_PORT[i],KEY_R_GPIO_PIN[i],Bit_RESET);
		col = Get_Col_Value();
		if(col!=KEY_ALL_UP){
			return (i+1);
		}
	}
	return 0;
}

uint8_t GetKeyNum(void)
{
	uint8_t col;
	uint8_t colIndex=0,rowIndex=0;
	uint8_t keyNum=0;
	
	/*��������ȫ�����0*/
	GPIO_WriteBit(KEY_R_GPIO_PORT_Init,KEY_R_GPIO_PIN_Init,Bit_RESET);
	
	/*��ȡ��ѡ�е��������*/
	col = Get_Col_Value();//����ֵ
	colIndex = Get_Col_Index(col);
	if(colIndex!=0){
		rowIndex = Get_Row_Index();
		if(rowIndex!=0){
			keyNum = (rowIndex<<4) | colIndex;
		}
	}

	return keyNum;
}
/*********************************************END OF FILE**********************/
