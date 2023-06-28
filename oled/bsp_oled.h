/**
  ******************************************************************************
  * @file    xxx.h
  * @author  Waveshare Team
  * @version 
  * @date    xx-xx-2014
  * @brief   xxxxx.
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
#ifndef _BSP_OLED_H_
#define _BSP_OLED_H_
/* Includes ------------------------------------------------------------------*/
#include "data_help.h"
#include "stm32f10x.h" 

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#include "delay.h"
#include "bsp_spi.h"
#include "drv_sh1106.h"
#include "Fonts.h"
/*------------------------------------------------------------------------------------------------------*/
//ssh1106
#define SH1106
//#define ssh1106

//#define INTERFACE_3WIRE_SPI   //3-wire SPI 
#define INTERFACE_4WIRE_SPI     //4-wire SPI 
//#define INTERFACE_IIC         //I2C

#define USR_SPI_x								SPI1
#define RCC_APBxPeriph_SPIx			RCC_APB2Periph_SPI1
#define ssh1106_CS_PIN          GPIO_Pin_4
#define ssh1106_CS_GPIO         GPIOA
#define ssh1106_CLK_PIN         GPIO_Pin_5
#define ssh1106_CLK_GPIO        GPIOA
#define ssh1106_DIN_PIN         GPIO_Pin_7
#define ssh1106_DIN_GPIO        GPIOA
#define ssh1106_RES_PIN         GPIO_Pin_7
#define ssh1106_RES_GPIO        GPIOB
#define ssh1106_DC_PIN          GPIO_Pin_6
#define ssh1106_DC_GPIO         GPIOA 

#define __ssh1106_CS_SET()      GPIO_WriteBit(ssh1106_CS_GPIO, ssh1106_CS_PIN, Bit_SET)
#define __ssh1106_CS_CLR()      GPIO_WriteBit(ssh1106_CS_GPIO, ssh1106_CS_PIN, Bit_RESET)

#define __ssh1106_RES_SET()     GPIO_WriteBit(ssh1106_RES_GPIO, ssh1106_RES_PIN, Bit_SET)
#define __ssh1106_RES_CLR()     GPIO_WriteBit(ssh1106_RES_GPIO, ssh1106_RES_PIN, Bit_RESET)

#define __ssh1106_DC_SET()      GPIO_WriteBit(ssh1106_DC_GPIO, ssh1106_DC_PIN, Bit_SET)
#define __ssh1106_DC_CLR()      GPIO_WriteBit(ssh1106_DC_GPIO, ssh1106_DC_PIN, Bit_RESET)

#define __ssh1106_CLK_SET()     GPIO_WriteBit(ssh1106_CLK_GPIO, ssh1106_CLK_PIN, Bit_SET)
#define __ssh1106_CLK_CLR()     GPIO_WriteBit(ssh1106_CLK_GPIO, ssh1106_CLK_PIN, Bit_RESET)

#define __ssh1106_DIN_SET()     GPIO_WriteBit(ssh1106_DIN_GPIO, ssh1106_DIN_PIN, Bit_SET)
#define __ssh1106_DIN_CLR()     GPIO_WriteBit(ssh1106_DIN_GPIO, ssh1106_DIN_PIN, Bit_RESET)

#define __ssh1106_WRITE_BYTE(DATA) spi_read_write_byte(USR_SPI_x, DATA)
/*------------------------------------------------------------------------------------------------------*/
//I2C	-- Î´ÒÆÖ²¹ý
#include "bsp_i2c.h"

#define IIC_SCL_PIN         GPIO_Pin_13
#define IIC_SDA_PIN         GPIO_Pin_15

#define IIC_SCL_GPIO        GPIOB
#define IIC_SDA_GPIO        GPIOB

#define __IIC_SCL_SET()     GPIO_WriteBit(IIC_SCL_GPIO, IIC_SCL_PIN, Bit_SET)
#define __IIC_SCL_CLR()     GPIO_WriteBit(IIC_SCL_GPIO, IIC_SCL_PIN, Bit_RESET)

#define __IIC_SDA_SET()		GPIO_WriteBit(IIC_SDA_GPIO, IIC_SDA_PIN, Bit_SET)
#define __IIC_SDA_CLR()     GPIO_WriteBit(IIC_SDA_GPIO, IIC_SDA_PIN, Bit_RESET)

#define __IIC_SDA_IN()     	do { \
								GPIO_InitTypeDef tGPIO; \
								tGPIO.GPIO_Pin = IIC_SDA_PIN; \
								tGPIO.GPIO_Speed = GPIO_Speed_50MHz; \
								tGPIO.GPIO_Mode = GPIO_Mode_IPU; \
								GPIO_Init(IIC_SDA_GPIO, &tGPIO); \
							}while(0)				

#define __IIC_SDA_OUT() 	do { \
								GPIO_InitTypeDef tGPIO; \
								tGPIO.GPIO_Pin = IIC_SDA_PIN; \
								tGPIO.GPIO_Speed = GPIO_Speed_50MHz; \
								tGPIO.GPIO_Mode = GPIO_Mode_Out_PP; \
								GPIO_Init(IIC_SDA_GPIO, &tGPIO); \
							}while(0)   


#define __IIC_SDA_READ()    GPIO_ReadInputDataBit(IIC_SDA_GPIO, IIC_SDA_PIN)
/*------------------------------------------------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

extern void OLED_Config(void);



#endif

/*-------------------------------END OF FILE-------------------------------*/
