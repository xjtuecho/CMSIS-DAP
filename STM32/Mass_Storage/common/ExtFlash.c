/********************* (C) COPYRIGHT 2010 e-Design Co.,Ltd. ********************
 File Name : EXT_Flash.c
 Version   : DS203_SYS Ver 1.5x                                  Author : bure
*******************************************************************************/
#include "ExtFlash.h"
//#include "BIOS.h"

#define OK           0            // 操作完成
#define SEC_ERR      1            // 扇区读写错误
#define TMAX         100000       // 超时限制

u32 Mass_Memory_Size;
u32 Mass_Block_Size;
u32 Mass_Block_Count;
u32 Tout;

/*******************************************************************************
 写FLASH页(256 Bytes)： Mode=0: 从0变1数据改写   Mode=1: 数据重写
*******************************************************************************/
void ExtFlash_PageWR(u8* pBuffer, u32 WriteAddr)
{
  u32  Data1, Data2;
  u16  i, Lenght = 256, Mode = 0; // Mode=0: 从0变1数据改写   Mode=1: 数据重写

  ExtFlash_CS_HIGH();
  ExtFlash_WaitForWriteEnd();

  WriteAddr &= ~0xFF;
  for(i=0; i<64; i++){
    ExtFlash_PageRD((u8*)&Data1, WriteAddr+i*4, 4);
    Data2 =*(u32*)(pBuffer+i*4);
    if((Data1 &(~Data2))!= 0)  Mode = 1; // 判断是否全部满足0->1
  }
  ExtFlash_WriteEnable();
  ExtFlash_CS_LOW();
  (Mode)?(ExtFlash_SendByte(PW)):(ExtFlash_SendByte(PP));
  ExtFlash_SendByte((WriteAddr & 0xFF0000) >> 16);
  ExtFlash_SendByte((WriteAddr & 0xFF00) >> 8);
  ExtFlash_SendByte(WriteAddr & 0xFF);
  while(Lenght--) {         // while there is data to be written on the FLASH
    ExtFlash_SendByte(~(*pBuffer));
    pBuffer++;
  }
  ExtFlash_CS_HIGH();
  ExtFlash_WaitForWriteEnd();
}
/*******************************************************************************
 SPI_FLASH_BufferRead
*******************************************************************************/
void ExtFlash_PageRD(u8* pBuffer, u32 ReadAddr, u16 Lenght)
{
  ExtFlash_CS_HIGH();
  ExtFlash_WaitForWriteEnd();

  ExtFlash_CS_LOW();
  ExtFlash_SendByte(READ);
  ExtFlash_SendByte((ReadAddr & 0xFF0000) >> 16);
  ExtFlash_SendByte((ReadAddr& 0xFF00) >> 8);
  ExtFlash_SendByte(ReadAddr & 0xFF);

  while(Lenght--){ // while there is data to be read
    *pBuffer = ~(ExtFlash_SendByte(Dummy_Byte));
    pBuffer++;
  }
  ExtFlash_CS_HIGH();
}
/*******************************************************************************
 SPI_FLASH_ReadByte
*******************************************************************************/
u8 ExtFlash_ReadByte(void)
{
  return (ExtFlash_SendByte(Dummy_Byte));
}

/*******************************************************************************
 SPI_FLASH_SendByte
*******************************************************************************/
u8 ExtFlash_SendByte(u8 byte)
{
  Tout = 0;
  while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET){
    if(Tout++ > TMAX) return 255;
  }
  SPI_I2S_SendData(SPI3, byte);
  Tout = 0;
  while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET){
    if(Tout++ > TMAX) return 255;
  }
  return SPI_I2S_ReceiveData(SPI3);
}
/*******************************************************************************
 SPI_FLASH_WriteEnable
*******************************************************************************/
void ExtFlash_WriteEnable(void)
{
  ExtFlash_CS_LOW();
  ExtFlash_SendByte(WREN);
  ExtFlash_CS_HIGH();
}
/*******************************************************************************
 SPI_FLASH_WaitForWriteEnd
*******************************************************************************/
void ExtFlash_WaitForWriteEnd(void)
{
  u8 FLASH_Status = 0;

  ExtFlash_CS_LOW();
  ExtFlash_SendByte(RDSR);
  Tout = 0;
  do { FLASH_Status = ExtFlash_SendByte(Dummy_Byte);
       if(Tout++ > TMAX) return;
  } while((FLASH_Status & WIP_Flag) == SET);              // Write in progress
  ExtFlash_CS_HIGH();
}
/*******************************************************************************
 MAL_GetStatus
*******************************************************************************/
void MAL_GetStatus (void)
{
    Mass_Block_Count = 4096;        //FLASH_SIZE/FLASH_PAGE_SIZE;
    Mass_Block_Size  = 512;         //FLASH_PAGE_SIZE;
    Mass_Memory_Size = 0x200000;    //FLASH_SIZE;
}

/*********************************  END OF FILE  ******************************/
