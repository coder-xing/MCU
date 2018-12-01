/*
 * Flash.h
 *
 *  Created on: 2018Äê11ÔÂ18ÈÕ
 *      Author: myy12
 */
#include <msp430f169.h>
#ifndef FLASH_H_
#define FLASH_H_
void FlashInit();
void FlashErase(unsigned int Addr);
void WaitForEnable();
void FlashWriteChar(unsigned int addr,unsigned char Data);
//void FlashWriteWord(unsigned long addr,unsigned long Data);
unsigned char FlashReadChar(unsigned int Addr);
/*nsigned int FlashReadWord(unsigned int Addr);
#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long*/
#define InfoA   (0x1080)
#define InfoB   (0x1000)
//unsigned char MdO[11]={0};
//unsigned char MdT[128]={0};

#endif /* FLASH_H_ */
