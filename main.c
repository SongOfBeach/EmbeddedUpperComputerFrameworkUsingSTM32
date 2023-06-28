#include "stm32f10x.h"
#include "string.h"
#include "stdbool.h"

#include "data_help.h"
#include "bsp_usart.h"
#include "bsp_led.h"
#include "bsp_key.h"
#include "bsp_oled.h"

#include "menu.h"

extern struct menustate myMenuState;
extern struct menu mainMenu;

void test_main(uint32_t mode,uint8_t *buf)
{
	int cnt=0;
	uint8_t ch;
	
	if(mode==0)/*����TEST*/
	{
		//LED1_ON;	// ��
		//delay_soft();
		//LED1_OFF;	// ��
		//delay_soft();
		cnt = Usart_RevArray(DEBUG_USARTx,buf);
		Usart_SendArray(DEBUG_USARTx,buf,cnt);
		delay_soft();	
		memset(buf,0,MAX_USART_REV_LEN);
	}
	else if(mode== 1)/*����TEST*/
	{
		ch = GetKeyNum();
		if(ch!=0){
			Usart_SendByte( DEBUG_USARTx,ch);
			delay_soft();
			delay_soft();
		}else{
			cnt = Usart_RevArray(DEBUG_USARTx,buf);
			if(cnt!=0){
				Usart_SendArray(DEBUG_USARTx,buf,cnt);
				//delay_soft();
				memset(buf,0,MAX_USART_REV_LEN);
			}
		}
	}
	else if(mode== 2)/*oled��ʾ�� TEST*/
	{
		ssh1106_clear_screen(0x00);;
		ssh1106_display_string(18, 0, "1.3inch OLED", 16, 1);
		ssh1106_display_string(0, 16, "This is a demo for ssh1106 OLED moudle!", 16, 1);
		ssh1106_display_string(10, 44, "test by zzh!", 9, 1);
		ssh1106_refresh_gram();
		delay_ms(1000);
		ssh1106_clear_screen(0x00);

		ssh1106_draw_bitmap(0, 2, &c_chSingal816[0], 16, 8);
		ssh1106_draw_bitmap(24, 2, &c_chBluetooth88[0], 8, 8);
		ssh1106_draw_bitmap(40, 2, &c_chMsg816[0], 16, 8);
		ssh1106_draw_bitmap(64, 2, &c_chGPRS88[0], 8, 8);
		ssh1106_draw_bitmap(90, 2, &c_chAlarm88[0], 8, 8);
		ssh1106_draw_bitmap(112, 2, &c_chBat816[0], 16, 8);
		
		ssh1106_draw_3216char(0,16, '2');
		ssh1106_draw_3216char(16,16, '3');
		ssh1106_draw_3216char(32,16, ':');
		ssh1106_draw_3216char(48,16, '5');
		ssh1106_draw_3216char(64,16, '6');
		ssh1106_draw_1616char(80,32, ':');
		ssh1106_draw_1616char(96,32, '4');
		ssh1106_draw_1616char(112,32, '7');
		ssh1106_draw_bitmap(87, 16, &c_chBmp4016[0], 40, 16);

		ssh1106_display_string(0, 52, "MUSIC", 12, 0);
		ssh1106_display_string(52, 52, "MENU", 12, 0);
		ssh1106_display_string(98, 52, "PHONE", 12, 0);
		
		ssh1106_refresh_gram();

		while (1) {

		}
	}
}

void init_Peripheral(void)
{
	/* LED �˿ڳ�ʼ�� -- ����PB11�ı仯��ѹ*/
	//LED_GPIO_Config();

	/*��ʼ��USART ����ģʽΪ 115200 8-N-1���жϽ���*/
	USART_Config();

	/* �����˿ڳ�ʼ�� */
	Key_GPIO_Config();

	/* OLED �˿ڳ�ʼ��*/
	OLED_Config();
}

void init_Parameter()
{
	myMenuState.top = 0;	
	myMenuState.MAX_ROWS = MENU_ROW_NUM*MENU_COL_NUM;
	myMenuState.last_top = 0;
	myMenuState.currentItem = 0;
	myMenuState.last_currentItem = 0;
	myMenuState.currentMenu = &mainMenu;
	
	ssh1106_clear_screen(0x00);
	draw_string_RankMiddle(11, "�������", 16, 1);
	draw_string_RankMiddle(37, "Ӣ��", 16, 1);
	ssh1106_refresh_gram();
	delay_ms(1000);
	ssh1106_refresh_gram();

	draw_menu_Page(&myMenuState);
}

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{	
	uint8_t buf[MAX_USART_REV_LEN];
	uint8_t ch;
	uint32_t cnt=0;

	init_Peripheral();/*��ʼ������*/
	init_Parameter();/*��ʼ������*/
	
	while(1)
	{	
		//test_main(2,buf);
		ch = GetKeyNum();
		if(ch!=0){/*��ȡ����*/
			if(!display_menu_changed_by_key(ch,&myMenuState)){
				TRACE(DEBUG_FAILURE_INFO,("display_changed_by_key No such key num =%d\\n\r",ch));
			}else
				draw_menu_Page(&myMenuState);
			delay_soft();
		}else{	/*�Ӵ��ڽ�������*/
			cnt = Usart_RevArray(DEBUG_USARTx,buf);
			if(cnt!=0){
				Usart_SendArray(DEBUG_USARTx,buf,cnt);
				delay_soft();
				memset(buf,0,MAX_USART_REV_LEN);
			}
		}
	}	
}
/*********************************************END OF FILE**********************/
