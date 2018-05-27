/**
  ******************************************************************************
  * @file    stm32f10x_conf.h
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   Library configuration file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F10x_CONF_H
#define __STM32F10x_CONF_H

/* Includes ------------------------------------------------------------------*/
/* Uncomment the line below to enable peripheral header file inclusion */
#include "stm32f10x_adc.h"
#include "stm32f10x_bkp.h"
#include "stm32f10x_can.h"
#include "stm32f10x_crc.h"
#include "stm32f10x_dac.h"
#include "stm32f10x_dbgmcu.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_flash.h"
#include "stm32f10x_fsmc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_i2c.h"
#include "stm32f10x_iwdg.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_rtc.h"
#include "stm32f10x_sdio.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_wwdg.h"
#include "misc.h"   /* High level functions for NVIC and SysTick (add-on to CMSIS functions) */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Uncomment the line below to expanse the "assert_param" macro in the
   Standard Peripheral Library drivers code */
/* #define USE_FULL_ASSERT    1 */


#ifndef M_PI
#define M_PI 3.14159265358979
#endif

#ifndef M_SQRT2
#define M_SQRT2  1.41421356237310
#endif

#ifndef M_SQRT3
#define M_SQRT3  1.73205080756888
#endif

#ifndef BOOL
#define BOOL	int
#endif
#ifndef TRUE
#define TRUE	(~0)
#endif
#ifndef FALSE
#define FALSE	(0)
#endif

#ifndef NULL
#ifdef __cplusplus
#define NULL (0L)
#else
#define NULL ((void*)0)
#endif
#endif

#define MAX(a,b)		((a)>(b)?(a):(b))
#define MIN(a,b)		((a)<(b)?(a):(b))
#define ARRAYSIZE(a)    (sizeof(a)/sizeof(a[0]))
#define LOBYTE(w)       (w&0xFF)
#define HIBYTE(w)       ((w>>8)&0xFF)
#define LOWORD(dw)      (dw&0xFFFF)
#define HIWORD(dw)      ((dw>>16)&0xFFFF)
#define MAKEDATE(y,m,d)	(((y&0x7F)<<9)|((m&0xF)<<5)|(d&0x1F))

/* Exported macro ------------------------------------------------------------*/
#ifdef  USE_FULL_ASSERT

/*******************************************************************************
* Macro Name     : assert_param
* Description    : The assert_param macro is used for function's parameters check.
* Input          : - expr: If expr is false, it calls assert_failed function
*                    which reports the name of the source file and the source
*                    line number of the call that failed.
*                    If expr is true, it returns no value.
* Return         : None
*******************************************************************************/
  #define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
  void assert_failed(uint8_t* file, uint32_t line);
#else
  #define assert_param(expr) ((void)0)
#endif /* USE_FULL_ASSERT */

#endif /* __STM32F10x_CONF_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
