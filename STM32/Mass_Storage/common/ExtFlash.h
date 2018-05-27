/********************* (C) COPYRIGHT 2010 e-Design Co.,Ltd. ********************
 File Name : EXT_Flash.h  
 Version   : DS203_SYS Ver 1.5x                                 Author : bure
*******************************************************************************/
#ifndef __EXT_FLASH_H
#define __EXT_FLASH_H

#include "hw_config.h"

#define EXT_FLASH_PageSize 256

#define WREN       0x06  // Write enable instruction 
#define READ       0x03  // Read from Memory instruction 
#define RDSR       0x05  // Read Status Register instruction  
#define PP         0x02  // Write to Memory instruction 
#define PE         0xDB  // Page Erase instruction 
#define PW         0x0A  // Page write instruction 
#define DP         0xB9  // Deep power-down instruction 
#define RDP        0xAB  // Release from deep power-down instruction 

#define WIP_Flag   0x01  // Write In Progress (WIP) flag 
#define Dummy_Byte 0xA5

#define ExtFlash_CS_LOW()     GPIO_ResetBits(GPIOB, GPIO_Pin_7)
#define ExtFlash_CS_HIGH()    GPIO_SetBits(GPIOB, GPIO_Pin_7)

void ExtFlash_PageWR(u8* pBuffer, u32 WriteAddr);
void ExtFlash_PageRD(u8* pBuffer, u32 ReadAddr, u16 Lenght);
void MAL_GetStatus (void);
u8   ExtFlash_ReadByte(void);
u8   ExtFlash_SendByte(u8 byte);
void ExtFlash_WriteEnable(void);
void ExtFlash_WaitForWriteEnd(void);

#endif

/*********************************  END OF FILE  ******************************/
