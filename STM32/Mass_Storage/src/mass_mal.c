/**
  ******************************************************************************
  * @file    mass_mal.c
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   Medium Access Layer interface
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


/* Includes ------------------------------------------------------------------*/
#include "hw_config.h"
#include "mass_mal.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

uint32_t Mass_Memory_Size[2];
uint32_t Mass_Block_Size[2];
uint32_t Mass_Block_Count[2];
uint8_t * const flashMassStart = (uint8_t *)MASS_FLASH_START;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : MAL_Init
* Description    : Initializes the Media on the STM32
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint16_t MAL_Init(uint8_t lun)
{
	if (0 == lun)
	{
		Mass_Block_Count[0] = MASS_SECTOR_NUM;
		Mass_Block_Size[0]  = MASS_SECTOR_SIZE;
		Mass_Memory_Size[0] = (Mass_Block_Count[0] * Mass_Block_Size[0]);
		return MAL_OK;
	}

	return MAL_FAIL;
}
/*******************************************************************************
* Function Name  : MAL_Write
* Description    : Write sectors
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint16_t MAL_Write(uint8_t lun, uint32_t offset, uint8_t *buf, uint32_t len)
{
	// len = 0x200
	uint32_t i = 0;
	uint16_t stat = MAL_OK;
	uint32_t progAddr = ((MASS_FLASH_START + offset) & (~(FLASH_PAGE_SIZE-1)));

	FLASH_Unlock();

	if (0 == lun)
	{
		if(0 == offset % FLASH_PAGE_SIZE)
		{
			for(i=0; i<MASS_SECTOR_SIZE; i++)
			{
				buf[MASS_SECTOR_SIZE+i] = flashMassStart[offset+MASS_SECTOR_SIZE+i];
			}
		}
		else
		{
			for(i=0; i<MASS_SECTOR_SIZE; i++)
			{
				buf[MASS_SECTOR_SIZE+i] = buf[i];
			}
			for(i=0; i<MASS_SECTOR_SIZE; i++)
			{
				buf[i] = flashMassStart[offset-MASS_SECTOR_SIZE+i];
			}
		}

		if(FLASH_COMPLETE == FLASH_ErasePage(progAddr))
		{
			for(i=0; i<FLASH_PAGE_SIZE; i+=2)
			{
				if(FLASH_COMPLETE != FLASH_ProgramHalfWord(progAddr + i,
					(((uint16_t)buf[i+1])<<8) | buf[i]))
				{
					stat = MAL_FAIL;
					break;
				}
			}
		}
		else
		{
			stat = MAL_FAIL;
		}
	}
	else
	{
		stat = MAL_FAIL;
	}

	FLASH_Lock();

	return stat;
}

/*******************************************************************************
* Function Name  : MAL_Read
* Description    : Read sectors
* Input          : None
* Output         : None
* Return         : Buffer pointer
*******************************************************************************/
uint16_t MAL_Read(uint8_t lun, uint32_t offset, uint8_t *buf, uint32_t len)
{
	uint32_t i = 0;

	if (0 == lun)
	{
		for(i=0; i<MASS_SECTOR_SIZE; i++)
		{
			buf[i] = flashMassStart[offset+i];
		}

		return MAL_OK;
	}

	return MAL_FAIL;
}

/*******************************************************************************
* Function Name  : MAL_GetStatus
* Description    : Get status
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint16_t MAL_GetStatus (uint8_t lun)
{
	if(0 == lun)
	{
		Mass_Block_Count[0] = MASS_SECTOR_NUM;
		Mass_Block_Size[0]  = MASS_SECTOR_SIZE;
		Mass_Memory_Size[0] = (Mass_Block_Count[0] * Mass_Block_Size[0]);
		return MAL_OK;
	}

	return MAL_FAIL;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

