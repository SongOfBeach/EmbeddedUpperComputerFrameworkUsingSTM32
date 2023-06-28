

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _USE_FONTS_H
#define _USE_FONTS_H

#define GB_16_FONT_ZONE_NUM 16

/* Includes ------------------------------------------------------------------*/
#include "bsp_oled.h"
#include <string.h>

/* Exported types ------------------------------------------------------------*/
// ------------------  汉字字模的数据结构定义 ------------------------ //
typedef struct typFNT_GB16                 // 汉字字模数据结构
{
       signed char Index[2];               // 汉字内码索引
       char Msk[32];                       // 点阵码数据,从上往下-从左往右扫描
}*typFNT_GB16_pt,typFNT_GB16_st;
// ------------------------------------------------------------------- //
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern const uint8_t c_chFont1206[95][12];
extern const uint8_t c_chFont1608[95][16];
extern const uint8_t c_chFont1612[11][32];
extern const uint8_t c_chFont3216[11][64];
extern const uint8_t c_chBmp4016[96];
extern const uint8_t c_chSingal816[16];
extern const uint8_t c_chMsg816[16];
extern const uint8_t c_chBluetooth88[8];
extern const uint8_t c_chBat816[16];
extern const uint8_t c_chGPRS88[8];
extern const uint8_t c_chAlarm88[8];

extern struct typFNT_GB16 Get_Font_From_Lib(uint16_t chChr);
#endif

/*-------------------------------END OF FILE-------------------------------*/

