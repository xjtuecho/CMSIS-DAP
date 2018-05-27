/**
  ******************************************************************************
  * @file    hw_config.h
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   Hardware Configuration & Setup
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
#ifndef __HW_CONFIG_H
#define __HW_CONFIG_H

/* Includes ------------------------------------------------------------------*/

//#include "platform_config.h"
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/

#define SN_ADDR 		0x1FFFF7E8
#define SERIAL_NO1      *((uint32_t*)(SN_ADDR+0))
#define SERIAL_NO2      *((uint32_t*)(SN_ADDR+4))
#define SERIAL_NO3      *((uint32_t*)(SN_ADDR+8))

// 64kB+64kB
#define FLASH_START 	0x08000000
#define FLASH_LEN		0x20000
#define FLASH_END		(FLASH_START+FLASH_LEN)

#define FLASH_PAGE_SIZE 	0x0400		// 1kB
#define FLASH_APP_LEN		0x2400

#define MASS_FLASH_START		(FLASH_START+FLASH_APP_LEN)
#define MASS_SECTOR_SIZE 		0x0200		// Windows 512B
#define MASS_SECTOR_NUM			((FLASH_LEN-FLASH_APP_LEN)/MASS_SECTOR_SIZE)

void RCC_Config(void);
void GPIO_Config(void);
void SPI_Config(void);
void Set_USBClock(void);
void Enter_LowPowerMode(void);
void Leave_LowPowerMode(void);
void USB_Interrupts_Config(void);
void USB_Configured_LED(void);
void USB_NotConfigured_LED(void);
void USB_Cable_Config (FunctionalState NewState);
void Get_SerialNum(void);
void MAL_Config(void);

FLASH_Status FLASH_Prog(uint32_t Addr, uint16_t Data);
FLASH_Status FLASH_Erase(uint32_t Addr);


#endif  /*__HW_CONFIG_H*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
