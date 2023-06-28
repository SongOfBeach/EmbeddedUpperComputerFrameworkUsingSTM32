/**
  ******************************************************************************
  * @file    ssh1106.h
  * @author  Waveshare Team
  * @version 
  * @date    13-October-2014
  * @brief   This file contains all the functions prototypes for the ssh1106 OLED firmware driver.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _ssh1106_H_
#define _ssh1106_H_

/* Includes ------------------------------------------------------------------*/
#include "data_help.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */

extern void ssh1106_clear_screen(uint8_t chFill);
extern void ssh1106_refresh_gram(void);
extern void ssh1106_draw_point(uint8_t chXpos, uint8_t chYpos, uint8_t chPoint);
extern void ssh1106_fill_screen(uint8_t chXpos1, uint8_t chYpos1, uint8_t chXpos2, uint8_t chYpos2, uint8_t chDot);
extern void ssh1106_display_char(uint8_t chXpos, uint8_t chYpos, uint8_t chChr, uint8_t chSize, uint8_t chMode);
extern void ssh1106_display_num(uint8_t chXpos, uint8_t chYpos, uint32_t chNum, uint8_t chLen,uint8_t chSize);
extern void ssh1106_display_string(uint8_t chXpos, uint8_t chYpos, /*const uint8_t*/char *pchString, uint8_t chSize, uint8_t chMode);
extern void ssh1106_draw_1616char(uint8_t chXpos, uint8_t chYpos, uint8_t chChar);
extern void ssh1106_draw_3216char(uint8_t chXpos, uint8_t chYpos, uint8_t chChar);
extern void ssh1106_draw_bitmap(uint8_t chXpos, uint8_t chYpos, const uint8_t *pchBmp, uint8_t chWidth, uint8_t chHeight);

extern void ssh1106_init(void);
extern void test_sh1106_init(void);
#endif

/*-------------------------------END OF FILE-------------------------------*/


