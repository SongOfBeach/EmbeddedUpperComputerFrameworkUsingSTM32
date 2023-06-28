/**
  ******************************************************************************
  * @file    drv_ssh1106.c 
  * @author  Waveshare Team
  * @version 
  * @date    13-October-2014
  * @brief   This file includes the OLED driver for ssh1106 display moudle
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, WAVESHARE SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "bsp_oled.h"
#include "drv_sh1106.h"
#include "Fonts.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ssh1106_CMD    0
#define ssh1106_DAT    1

#define ssh1106_WIDTH    128
#define ssh1106_HEIGHT   64


/* Private macro -------------------------------------------------------------*/

#if !defined(SH1106) && !defined(ssh1106)
	#warning Please select first the target OLED device(SH1106 or ssh1106) in your application!
	#define ssh1106  //define ssh1106 by default	
#endif

#if defined(ssh1106)
#define __SET_COL_START_ADDR() 	do { \
									ssh1106_write_byte(0x00, ssh1106_CMD); \
									ssh1106_write_byte(0x10, ssh1106_CMD); \
								} while(false)
#elif defined(SH1106)
#define __SET_COL_START_ADDR() 	do { \
									ssh1106_write_byte(0x02, ssh1106_CMD); \
									ssh1106_write_byte(0x10, ssh1106_CMD); \
								} while(false)
#endif								
/* Private variables ---------------------------------------------------------*/
static uint8_t s_chDispalyBuffer[128][8];

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/**
  * @brief  Writes an byte to the display data ram or the command register
  *         
  * @param  chData: Data to be writen to the display data ram or the command register
  * @param chCmd:  
  *                           0: Writes to the command register
  *                           1: Writes to the display data ram
  * @retval None
**/
static void ssh1106_write_byte(uint8_t chData, uint8_t chCmd) 
{
#ifdef INTERFACE_4WIRE_SPI
	
	__ssh1106_CS_CLR();
	
	if (chCmd) {
		__ssh1106_DC_SET();
	} else {
		__ssh1106_DC_CLR();
	}	
	__ssh1106_WRITE_BYTE(chData);
	
	__ssh1106_DC_SET();
	__ssh1106_CS_SET();
	
#elif defined(INTERFACE_3WIRE_SPI)
	
	uint8_t i;
	uint16_t hwData = 0;
	
	if (chCmd) {
		hwData = (uint16_t)chData | 0x0100;
	} else {
		hwData = (uint16_t)chData & ~0x0100;
	}
	
	__ssh1106_CS_CLR();
	for(i = 0; i < 9; i ++) {
		__ssh1106_CLK_CLR();
		
		if(hwData & 0x0100) {
			__ssh1106_DIN_SET();
		} else {
			__ssh1106_DIN_CLR();
		}
		__ssh1106_CLK_SET();
		hwData <<= 1;
	}
	__ssh1106_CS_SET();
	
#elif defined(INTERFACE_IIC)
	
	iic_start();
	iic_write_byte(0x78);
	iic_wait_for_ack();
	if (chCmd) {
		iic_write_byte(0x40);
		iic_wait_for_ack();
	} else {
		iic_write_byte(0x00);
		iic_wait_for_ack();
	}
	iic_write_byte(chData);
	iic_wait_for_ack();
	
	iic_stop();
	
#endif
}   	  

/**
  * @brief  OLED turns on 
  *         
  * @param  None
  *         
  * @retval None
**/ 
void ssh1106_display_on(void)
{
	ssh1106_write_byte(0x8D, ssh1106_CMD);  
	ssh1106_write_byte(0x14, ssh1106_CMD);  
	ssh1106_write_byte(0xAF, ssh1106_CMD);  
}
   
/**
  * @brief  OLED turns off
  *         
  * @param  None
  *         
  * @retval  None
**/
void ssh1106_display_off(void)
{
	ssh1106_write_byte(0x8D, ssh1106_CMD);  
	ssh1106_write_byte(0x10, ssh1106_CMD); 
	ssh1106_write_byte(0xAE, ssh1106_CMD);  
}

/**
  * @brief  Refreshs the graphic ram
  *         
  * @param  None
  *         
  * @retval  None
**/

void ssh1106_refresh_gram(void)
{
	uint8_t i, j;
	
	for (i = 0; i < 8; i ++) {  
		ssh1106_write_byte(0xB0 + i, ssh1106_CMD);    
		__SET_COL_START_ADDR();      
		for (j = 0; j < 128; j ++) {
			ssh1106_write_byte(s_chDispalyBuffer[j][i], ssh1106_DAT); 
		}
	}   
}


/**
  * @brief   Clears the screen
  *         
  * @param  None
  *         
  * @retval  None
**/

void ssh1106_clear_screen(uint8_t chFill)  
{ 
	uint8_t i, j;
	
	for (i = 0; i < 8; i ++) {
		ssh1106_write_byte(0xB0 + i, ssh1106_CMD);
		__SET_COL_START_ADDR();
		for (j = 0; j < 128; j ++) {
			s_chDispalyBuffer[j][i] = chFill;
		}
	}
	
	ssh1106_refresh_gram();
}

/**
  * @brief  Draws a piont on the screen
  *         
  * @param  chXpos: Specifies the X position
  * @param  chYpos: Specifies the Y position
  * @param  chPoint: 0: the point turns off    1: the piont turns on 
  *         
  * @retval None
**/

void ssh1106_draw_point(uint8_t chXpos, uint8_t chYpos, uint8_t chPoint)
{
	uint8_t chPos, chBx, chTemp = 0;
	
	if (chXpos > 127 || chYpos > 63) {
		return;
	}
	chPos = 7 - chYpos / 8; // 
	chBx = chYpos % 8;
	chTemp = 1 << (7 - chBx);
	
	if (chPoint) {
		s_chDispalyBuffer[chXpos][chPos] |= chTemp;
		
	} else {
		s_chDispalyBuffer[chXpos][chPos] &= ~chTemp;
	}
}
	  
/**
  * @brief  Fills a rectangle
  *         
  * @param  chXpos1: Specifies the X position 1 (X top left position)
  * @param  chYpos1: Specifies the Y position 1 (Y top left position)
  * @param  chXpos2: Specifies the X position 2 (X bottom right position)
  * @param  chYpos3: Specifies the Y position 2 (Y bottom right position)
  *         
  * @retval 
**/

void ssh1106_fill_screen(uint8_t chXpos1, uint8_t chYpos1, uint8_t chXpos2, uint8_t chYpos2, uint8_t chDot)  
{  
	uint8_t chXpos, chYpos; 
	
	for (chXpos = chXpos1; chXpos <= chXpos2; chXpos ++) {
		for (chYpos = chYpos1; chYpos <= chYpos2; chYpos ++) {
			ssh1106_draw_point(chXpos, chYpos, chDot);
		}
	}	
	
	ssh1106_refresh_gram();
}


/**
  * @brief Displays one character at the specified position    
  *         
  * @param  chXpos: Specifies the X position
  * @param  chYpos: Specifies the Y position
  * @param  chSize: 
  * @param  chMode
  * @retval 
**/
void ssh1106_display_char(uint8_t chXpos, uint8_t chYpos, uint8_t chChr, uint8_t chSize, uint8_t chMode)
{      	
	uint8_t i, j;
	uint8_t chTemp, chYpos0 = chYpos;
	
	chChr = chChr - ' ';				   
    for (i = 0; i < chSize; i ++) {   
		if (chSize == 12) {
			if (chMode) {
				chTemp = c_chFont1206[chChr][i];
			} else {
				chTemp = ~c_chFont1206[chChr][i];
			}
		} else {
			if (chMode) {
				chTemp = c_chFont1608[chChr][i];
			} else {
				chTemp = ~c_chFont1608[chChr][i];
			}
		}
		
			for (j = 0; j < 8; j ++) {
				if (chTemp & 0x80) {
					ssh1106_draw_point(chXpos, chYpos, 1);
				} else {
					ssh1106_draw_point(chXpos, chYpos, 0);
				}
				chTemp <<= 1;
				chYpos ++;
				
				if ((chYpos - chYpos0) == chSize) {
					chYpos = chYpos0;
					chXpos ++;
					break;
				}
			}  	 
    } 
}

void ssh1106_display_cn_font(uint8_t chXpos, uint8_t chYpos, uint16_t chChr, uint8_t chSize, uint8_t chMode)
{ 

	uint8_t i, j;
	uint8_t chTemp;
	uint8_t chYpos0 = chYpos;
	struct typFNT_GB16 chFont;
	
	chFont = Get_Font_From_Lib(chChr);
	for (i = 0; i < chSize*2; i ++) {
		if (chSize == 16) {//一个汉字16*16
			if (chMode) {
				chTemp = chFont.Msk[i];
			} else {
				chTemp = ~(chFont.Msk[i]);
			}
		} 
		
		for (j = 0; j < 8; j ++) {
			if (chTemp & 0x80) {
				ssh1106_draw_point(chXpos, chYpos, 1);
			} else {
				ssh1106_draw_point(chXpos, chYpos, 0);
			}
			chTemp <<= 1;
			chYpos ++;
			
			if ((chYpos - chYpos0) == chSize) {
				chYpos = chYpos0;
				chXpos ++;
				break;
			}
		}  	 
	}

}

static uint32_t pow(uint8_t m, uint8_t n)
{
	uint32_t result = 1;	 
	while(n --) result *= m;    
	return result;
}	


void ssh1106_display_num(uint8_t chXpos, uint8_t chYpos, uint32_t chNum, uint8_t chLen, uint8_t chSize)
{         	
	uint8_t i;
	uint8_t chTemp, chShow = 0;
	
	for(i = 0; i < chLen; i ++) {
		chTemp = (chNum / pow(10, chLen - i - 1)) % 10;
		if(chShow == 0 && i < (chLen - 1)) {
			if(chTemp == 0) {
				ssh1106_display_char(chXpos + (chSize / 2) * i, chYpos, ' ', chSize, 1);
				continue;
			} else {
				chShow = 1;
			}	 
		}
	 	ssh1106_display_char(chXpos + (chSize / 2) * i, chYpos, chTemp + '0', chSize, 1); 
	}
} 


/**
  * @brief  Displays a string on the screen
  *         
  * @param  chXpos: Specifies the X position
  * @param  chYpos: Specifies the Y position
  * @param  pchString: Pointer to a string to display on the screen 
  *         
  * @retval  None
**/
void ssh1106_display_string(uint8_t chXpos, uint8_t chYpos, /*const uint8_t*/char *pchString, uint8_t chSize, uint8_t chMode)
{
	//char *pchString_next;
    while (*pchString != '\0') {  
			//	pchString_next = pchString+1;
        if (chXpos > (ssh1106_WIDTH - chSize / 2)) {
				chXpos = 0;
				chYpos += chSize;
				if (chYpos > (ssh1106_HEIGHT - chSize)) {
					chYpos = chXpos = 0;
					ssh1106_clear_screen(0x00);
				}
			}
#if 0
			ssh1106_display_char(chXpos, chYpos, *pchString, chSize, chMode);
			chXpos += chSize / 2;
			pchString ++;
#else
			if((*pchString)>>7){/*汉字*/
					ssh1106_display_cn_font(chXpos, chYpos, *(uint16_t*)pchString, chSize, chMode);
					chXpos += chSize;
					pchString +=2;
			}else{
					ssh1106_display_char(chXpos, chYpos, *pchString, chSize, chMode);
					chXpos += chSize / 2;
					pchString ++;
			}
#endif
    }
}

void ssh1106_draw_1616char(uint8_t chXpos, uint8_t chYpos, uint8_t chChar)
{
	uint8_t i, j;
	uint8_t chTemp = 0, chYpos0 = chYpos, chMode = 0;

	for (i = 0; i < 32; i ++) {
		chTemp = c_chFont1612[chChar - 0x30][i];
		for (j = 0; j < 8; j ++) {
			chMode = chTemp & 0x80? 1 : 0; 
			ssh1106_draw_point(chXpos, chYpos, chMode);
			chTemp <<= 1;
			chYpos ++;
			if ((chYpos - chYpos0) == 16) {
				chYpos = chYpos0;
				chXpos ++;
				break;
			}
		}
	}
}

void ssh1106_draw_3216char(uint8_t chXpos, uint8_t chYpos, uint8_t chChar)
{
	uint8_t i, j;
	uint8_t chTemp = 0, chYpos0 = chYpos, chMode = 0;

	for (i = 0; i < 64; i ++) {
		chTemp = c_chFont3216[chChar - 0x30][i];
		for (j = 0; j < 8; j ++) {
			chMode = chTemp & 0x80? 1 : 0; 
			ssh1106_draw_point(chXpos, chYpos, chMode);
			chTemp <<= 1;
			chYpos ++;
			if ((chYpos - chYpos0) == 32) {
				chYpos = chYpos0;
				chXpos ++;
				break;
			}
		}
	}
}

void ssh1106_draw_bitmap(uint8_t chXpos, uint8_t chYpos, const uint8_t *pchBmp, uint8_t chWidth, uint8_t chHeight)
{
	uint16_t i, j, byteWidth = (chWidth + 7) / 8;
	
    for(j = 0; j < chHeight; j ++){
        for(i = 0; i < chWidth; i ++ ) {
            if(*(pchBmp + j * byteWidth + i / 8) & (128 >> (i & 7))) {
                ssh1106_draw_point(chXpos + i, chYpos + j, 1);
            }
        }
    }
}


/**
  * @brief  ssh1106 initialization
  *         
  * @param  None
  *         
  * @retval None
**/
void ssh1106_init(void)
{		
#ifdef INTERFACE_4WIRE_SPI	  
	__ssh1106_CS_SET();   //CS set
	__ssh1106_DC_CLR();   //D/C reset
	__ssh1106_RES_SET();  //RES set


#elif defined(INTERFACE_3WIRE_SPI)	
	__ssh1106_CS_CLR();   //CS reset
	__ssh1106_DC_CLR();   //D/C reset
	__ssh1106_RES_SET();  //RES set
	
#elif defined(INTERFACE_IIC)	  
	__ssh1106_CS_CLR();   //CS reset
	__ssh1106_DC_CLR();   //D/C reset
	__ssh1106_RES_SET();  //RES set

#endif

	ssh1106_write_byte(0xAE, ssh1106_CMD);//--turn off oled panel
	ssh1106_write_byte(0x00, ssh1106_CMD);//---set low column address
	ssh1106_write_byte(0x10, ssh1106_CMD);//---set high column address
	ssh1106_write_byte(0x40, ssh1106_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	ssh1106_write_byte(0x81, ssh1106_CMD);//--set contrast control register
	ssh1106_write_byte(0xCF, ssh1106_CMD);// Set SEG Output Current Brightness
	ssh1106_write_byte(0xA1, ssh1106_CMD);//--Set SEG/Column Mapping     
	ssh1106_write_byte(0xC0, ssh1106_CMD);//Set COM/Row Scan Direction   
	ssh1106_write_byte(0xA6, ssh1106_CMD);//--set normal display
	ssh1106_write_byte(0xA8, ssh1106_CMD);//--set multiplex ratio(1 to 64)
	ssh1106_write_byte(0x3f, ssh1106_CMD);//--1/64 duty

#if 0
	ssh1106_write_byte(0xAD, ssh1106_CMD);//--set charge pump enable
	ssh1106_write_byte(0x8B, ssh1106_CMD);//--0x8B 内供VCC
	ssh1106_write_byte(0x33, ssh1106_CMD);//set VPP 9V
#endif
	
	ssh1106_write_byte(0xD3, ssh1106_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	ssh1106_write_byte(0x00, ssh1106_CMD);//-not offset
	ssh1106_write_byte(0xd5, ssh1106_CMD);//--set display clock divide ratio/oscillator frequency
	ssh1106_write_byte(0x80, ssh1106_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	ssh1106_write_byte(0xD9, ssh1106_CMD);//--set pre-charge period
	ssh1106_write_byte(0xF1, ssh1106_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	ssh1106_write_byte(0xDA, ssh1106_CMD);//--set com pins hardware configuration
	ssh1106_write_byte(0x12, ssh1106_CMD);
	ssh1106_write_byte(0xDB, ssh1106_CMD);//--set vcomh
	ssh1106_write_byte(0x40, ssh1106_CMD);//Set VCOM Deselect Level
	ssh1106_write_byte(0x20, ssh1106_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	ssh1106_write_byte(0x02, ssh1106_CMD);//
	ssh1106_write_byte(0x8D, ssh1106_CMD);//--set Charge Pump enable/disable
	ssh1106_write_byte(0x14, ssh1106_CMD);//--set(0x10) disable
	ssh1106_write_byte(0xA4, ssh1106_CMD);// Disable Entire Display On (0xa4/0xa5)
	ssh1106_write_byte(0xA6, ssh1106_CMD);// Disable Inverse Display On (0xa6/a7) 
	ssh1106_write_byte(0xAF, ssh1106_CMD);//--turn on oled panel
	
	ssh1106_clear_screen(0x00);
}

void test_sh1106_init(void)
{

#ifdef INTERFACE_4WIRE_SPI	  
	__ssh1106_CS_SET();   //CS set
	__ssh1106_DC_CLR();   //D/C reset
	__ssh1106_RES_SET();  //RES set
	delay_ms(100);
	__ssh1106_RES_CLR();
	delay_ms(100);
	__ssh1106_RES_SET();
	delay_ms(100);
#elif defined(INTERFACE_3WIRE_SPI)	
	__ssh1106_CS_CLR();   //CS reset
	__ssh1106_DC_CLR();   //D/C reset
	__ssh1106_RES_SET();  //RES set
	
#elif defined(INTERFACE_IIC)	  
	__ssh1106_CS_CLR();   //CS reset
	__ssh1106_DC_CLR();   //D/C reset
	__ssh1106_RES_SET();  //RES set

#endif

	ssh1106_write_byte(0xAE, ssh1106_CMD);//--turn off oled panel
	
	ssh1106_write_byte(0x00, ssh1106_CMD);//---set low column address
	ssh1106_write_byte(0x10, ssh1106_CMD);//---set high column address
	
	ssh1106_write_byte(0x40, ssh1106_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	
	ssh1106_write_byte(0xB0, ssh1106_CMD);//--set page address
	
	ssh1106_write_byte(0x81, ssh1106_CMD);//--set contrast control register
	ssh1106_write_byte(0xFF, ssh1106_CMD);//--128
	
	ssh1106_write_byte(0xA1, ssh1106_CMD);//--Set segment Mapping     
	   
	ssh1106_write_byte(0xA6, ssh1106_CMD);//--set normal display
	
	ssh1106_write_byte(0xA8, ssh1106_CMD);//--set multiplex ratio(1 to 64)
	ssh1106_write_byte(0x3F, ssh1106_CMD);//--1/64 duty

#if 1
	ssh1106_write_byte(0x8d, ssh1106_CMD);//--set DC-DC enable
	ssh1106_write_byte(0x14, ssh1106_CMD);
#else
	ssh1106_write_byte(0xAD, ssh1106_CMD);//--set charge pump enable
	ssh1106_write_byte(0x8B, ssh1106_CMD);//--0x8B 内供VCC
	ssh1106_write_byte(0x33, ssh1106_CMD);//set VPP 9V
#endif
	ssh1106_write_byte(0xC8, ssh1106_CMD);//Set COM/Row Scan Direction
	
	ssh1106_write_byte(0xD3, ssh1106_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	ssh1106_write_byte(0x00, ssh1106_CMD);//-not offset
	
	ssh1106_write_byte(0xD5, ssh1106_CMD);//--set display clock divide ratio/oscillator frequency
	ssh1106_write_byte(0x80, ssh1106_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	
	ssh1106_write_byte(0xD9, ssh1106_CMD);//--set pre-charge period
	ssh1106_write_byte(0x1F, ssh1106_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	
	ssh1106_write_byte(0xDA, ssh1106_CMD);//--set com pins hardware configuration
	ssh1106_write_byte(0x12, ssh1106_CMD);
	
	ssh1106_write_byte(0xDB, ssh1106_CMD);//--set vcomh
	ssh1106_write_byte(0x40, ssh1106_CMD);//Set VCOM Deselect Level
 
	ssh1106_write_byte(0xAF, ssh1106_CMD);//--turn on oled panel
	
	ssh1106_clear_screen(0x00);
}
/*-------------------------------END OF FILE-------------------------------*/

