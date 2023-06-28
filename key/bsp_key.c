/**
  ******************************************************************************
  * @file    bsp_key.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   按键应用bsp（扫描模式）
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火 F103-霸道 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
  
#include "bsp_key.h"  
#include "data_help.h"
#include "delay.h"

	/*GPIO参数全局变量*/
	uint32_t KEY_R_GPIO_CLK_Init = 0;
	uint32_t KEY_C_GPIO_CLK_Init = 0;
	GPIO_TypeDef *KEY_R_GPIO_PORT_Init = KEY_R0_GPIO_PORT;;
	GPIO_TypeDef *KEY_C1_GPIO_PORT_Init = KEY_C0_GPIO_PORT;//前两列为GPIOC
	GPIO_TypeDef *KEY_C2_GPIO_PORT_Init = KEY_C2_GPIO_PORT;//后五列为GPIOB
	GPIO_TypeDef * KEY_R_GPIO_PORT[KEY_ROW] ={KEY_R0_GPIO_PORT,KEY_R1_GPIO_PORT,KEY_R2_GPIO_PORT};
	GPIO_TypeDef * KEY_C_GPIO_PORT[KEY_COL] ={KEY_C0_GPIO_PORT,KEY_C1_GPIO_PORT,KEY_C2_GPIO_PORT,KEY_C3_GPIO_PORT,KEY_C4_GPIO_PORT,KEY_C5_GPIO_PORT,KEY_C6_GPIO_PORT};
	uint16_t KEY_R_GPIO_PIN_Init=0;
	uint16_t KEY_C1_GPIO_PIN_Init=0;
	uint16_t KEY_C2_GPIO_PIN_Init=0;
	uint16_t KEY_R_GPIO_PIN[KEY_ROW] ={KEY_R0_GPIO_PIN,KEY_R1_GPIO_PIN,KEY_R2_GPIO_PIN};
	uint16_t KEY_C_GPIO_PIN[KEY_COL] ={KEY_C0_GPIO_PIN,KEY_C1_GPIO_PIN,KEY_C2_GPIO_PIN,KEY_C3_GPIO_PIN,KEY_C4_GPIO_PIN,KEY_C5_GPIO_PIN,KEY_C6_GPIO_PIN};
	
/**
  * @brief  获取按键用到的所有I/O口参数
  * @param  无
  * @retval 无
  */
void Key_GPIO_Para_Init()
{
	int i=0;
	/*矩阵按键 - 修改引脚时在本文件内仅修改此处*/
	uint32_t KEY_R_GPIO_CLK[KEY_ROW] = {KEY_R0_GPIO_CLK,KEY_R1_GPIO_CLK,KEY_R2_GPIO_CLK};
	uint32_t KEY_C_GPIO_CLK[KEY_COL] = {KEY_C0_GPIO_CLK,KEY_C1_GPIO_CLK,KEY_C2_GPIO_CLK,KEY_C3_GPIO_CLK,KEY_C4_GPIO_CLK,KEY_C5_GPIO_CLK,KEY_C6_GPIO_CLK};
	

	for(i=0;i<KEY_ROW;i++){
		//按键端口的时钟
		KEY_R_GPIO_CLK_Init = KEY_R_GPIO_CLK_Init|KEY_R_GPIO_CLK[i];
		//按键的引脚
		KEY_R_GPIO_PIN_Init = KEY_R_GPIO_PIN_Init|KEY_R_GPIO_PIN[i]; 
	}
	
	for(i=0;i<KEY_COL;i++){
		//按键端口的时钟
		KEY_C_GPIO_CLK_Init = KEY_C_GPIO_CLK_Init|KEY_C_GPIO_CLK[i];
		//按键的引脚 -- 前两列为GPIOC,后五列为GPIOB
		if(i<2) KEY_C1_GPIO_PIN_Init = KEY_C1_GPIO_PIN_Init|KEY_C_GPIO_PIN[i]; 
		else	KEY_C2_GPIO_PIN_Init = KEY_C2_GPIO_PIN_Init|KEY_C_GPIO_PIN[i]; 
	}
}

/**
  * @brief  配置下发按键用到的I/O口
  * @param  无
  * @retval 无
  */
void Key_GPIO_Config(void)
{	
	GPIO_InitTypeDef GPIO_R_InitStructure;
	GPIO_InitTypeDef GPIO_C1_InitStructure;
	GPIO_InitTypeDef GPIO_C2_InitStructure;
	
/*获取实际下发的参数*/
	Key_GPIO_Para_Init();
	
/*配置需要下发的参数*/
	//按键的引脚
	GPIO_R_InitStructure.GPIO_Pin = KEY_R_GPIO_PIN_Init; 
	//设置行按键为推挽输出
	GPIO_R_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_R_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
	
	//按键的引脚 -- 前两列为GPIOC,后五列为GPIOB
	GPIO_C1_InitStructure.GPIO_Pin = KEY_C1_GPIO_PIN_Init; 
	GPIO_C2_InitStructure.GPIO_Pin = KEY_C2_GPIO_PIN_Init; 	
	//设置列按键为上拉输入
	GPIO_C1_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_C2_InitStructure.GPIO_Mode = GPIO_Mode_IPU;


/*初始化时钟和按键*/
	//开启按键端口的时钟
	RCC_APB2PeriphClockCmd(KEY_R_GPIO_CLK_Init|KEY_C_GPIO_CLK_Init,ENABLE);
	
	//使用结构体初始化按键
	GPIO_Init(KEY_R_GPIO_PORT_Init, &GPIO_R_InitStructure);
	GPIO_Init(KEY_C1_GPIO_PORT_Init, &GPIO_C1_InitStructure);
	GPIO_Init(KEY_C2_GPIO_PORT_Init, &GPIO_C2_InitStructure);
}

 /*
 * 函数名：Key_Scan
 * 描述  ：检测是否有按键按下
 * 输入  ：GPIOx：x 可以是 A，B，C，D或者 E
 *		     GPIO_Pin：待读取的端口位 	
 * 输出  ：KEY_OFF(没按下按键)、KEY_ON（按下按键）
 */
uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{			
	/*检测是否有按键按下 */
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON )  
	{	 
		/*等待按键释放 */
		//while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON);   
		return 	KEY_ON;	 
	}
	else
		return KEY_OFF;
}
/*
 * 函数名：Get_Cow_Value
 * 描述  ：读出所有列线组成的值，线序按从低位往高位递增
 * 输入  ：
 * 输出  ：列线值
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
 * 函数名：Get_Cow_Index
 * 描述  ：获得被选中的列线，未被按下时该线的值1，按下后为0；
 * 输入  ：
 * 输出  ：列线序号，返回0时无列线被选中
 */
uint8_t Get_Col_Index(uint8_t col)
{
	if(col!=KEY_ALL_UP){
		Delay(KEY_SHAKE_DELAY);//消抖
		col = Get_Col_Value();
		if(col!=KEY_ALL_UP){//判断被选中的列线
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
	
	/*设置行线全部输出0*/
	GPIO_WriteBit(KEY_R_GPIO_PORT_Init,KEY_R_GPIO_PIN_Init,Bit_RESET);
	
	/*获取被选中的列线序号*/
	col = Get_Col_Value();//读列值
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
