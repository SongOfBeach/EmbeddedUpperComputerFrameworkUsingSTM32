#ifndef __DATA_HELP_H
#define __DATA_HELP_H
#include "stm32f10x.h"
#include <stdio.h>	
#include <stdint.h>	
#include <stdbool.h>

#define DEBUG_INFO DEBUG_NO_INFO	/*打印调试信息到串口*/
#define	DEBUG_ALL_INFO				4
#define DEBUG_FAILURE_INFO			1
#define DEBUG_NO_INFO				0
#define TRACE(level,message)   if(DEBUG_INFO >= level) printf message
	
#define MAX_USART_REV_LEN	32

#define BIT(__N)         ((uint8_t)(1 << __N)) 

#define enable_irq()   __enable_irq();
#define disable_irq()  __disable_irq(); 


//! \brief The safe ATOM code section macro
#define SAFE_ATOM_CODE(...) {\
        disable_irq();\
        __VA_ARGS__;\
        enable_irq();\
    }

#define YES		0
#define NO		1

/*---------------------------- 按键值 ---------------------------------------*/
#define KEY_UP			0x22
#define KEY_DOWN		0x12
#define KEY_LEFT		0x13
#define KEY_RIGHT		0x11
#define KEY_YES			0x23
#define KEY_NO			0x21
#define	KEY_NUM0		0x24
#define	KEY_NUM1		0x35	
#define	KEY_NUM2		0x34
#define	KEY_NUM3		0x33
#define	KEY_NUM4		0x32
#define	KEY_NUM5		0x31
#define	KEY_NUM6		0x01
#define	KEY_NUM7		0x02
#define	KEY_NUM8		0x03
#define	KEY_NUM9		0x04
#define KEY_DOT			0X25
/*---------------------------------------------------------------------------*/
		
		
/*----------------------------- 绘图 ----------------------------------------*/
#define	OLED_ROW_PIXEL		64	//行像素点
#define	OLED_COL_PIXEL		128 //列像素点

/*菜单页绘图*/
#define MENU_FONT_SIZE		16	//字号 12:12*6	16:16*8
#define MENU_ROW_NUM			4		//菜单页的行数
#define MENU_COL_NUM			1		//菜单页的列数(不要大于2)
#define MENU_ROW_PIXEL_OFFSET		(uint8_t)((OLED_ROW_PIXEL - MENU_ROW_NUM*MENU_FONT_SIZE)/2)		//行像素偏移
#define MENU_COL_PIXEL_OFFSET		(uint8_t)(MENU_FONT_SIZE/2-1)	//列像素偏移
#define MENU_ROW_PIXEL_SPACE		(uint8_t)((OLED_ROW_PIXEL-MENU_FONT_SIZE*MENU_ROW_NUM)/(MENU_ROW_NUM+1)+MENU_FONT_SIZE)		//行像素间距
#define MENU_COL_PIXEL_SPACE		(uint8_t)(OLED_COL_PIXEL/MENU_COL_NUM)		//列像素间距

/*IP设置页绘图*/
#define PAGE_IP_FONT_SIZE		12	//字号
#define PAGE_IP_ROW_PIXEL_SPACE		(uint8_t)(OLED_ROW_PIXEL/PAGE_IP_FONT_SIZE)		//行像素间距
#define PAGE_IP_COL_PIXEL_SPACE		(uint8_t)(OLED_COL_PIXEL/PAGE_IP_FONT_SIZE/2)		//列像素间距
/*---------------------------------------------------------------------------*/
		
typedef struct {
    unsigned Bit0:1;
    unsigned Bit1:1;
    unsigned Bit2:1;
    unsigned Bit3:1;
    unsigned Bit4:1;
    unsigned Bit5:1;
    unsigned Bit6:1;
    unsigned Bit7:1;
}byte_bit_t;

typedef union{
	byte_bit_t tBit;
	uint8_t chByte;
}bit_operate_t;


	
#endif /*__DATA_HELP_H*/
