/*
 * FLASH.c
 *
 *  Created on: 2018年11月18日
 *      Author: liu
 */




/*
 * Flash.c
 *
 *  Created on: 2018年11月18日
 *      Author: myy12
 */
#include "FLASH.h"



void FlashInit(){
    FCTL2 = FWKEY + FSSEL_2 + FN1;          //默认 SMCLK/3 = 333KHz
}
//擦除函数
void FlashErase(unsigned int Addr){
    unsigned char *FlashPtr;
    FlashPtr = (unsigned char *)Addr;
    FCTL1 = FWKEY + ERASE;                      // Set Erase bit
    FCTL3 = FWKEY;                              // Clear Lock bit
    *FlashPtr = 0;                              // Dummy write to erase Flash segment B
    WaitForEnable();                            //Busy
    FCTL1 = FWKEY;                              // Lock
    FCTL3 = FWKEY + LOCK;                       // Set Lock bit
}
//等待
void WaitForEnable()
{
    while((FCTL3 & BUSY) == BUSY);      //Busy
}
//写入字节
void FlashWriteChar(unsigned int addr,unsigned char Data)
{
    unsigned char *FlashPtr = (unsigned char *)addr;              // Segment A pointer
    FCTL1 = FWKEY + WRT;                        // Set WRT bit for write operation
    FCTL3 = FWKEY;                              // Clear Lock bit
    *FlashPtr = Data;                           // Save Data
    WaitForEnable();                            //Busy
    FCTL1 = FWKEY;                              // Clear WRT bit
    FCTL3 = FWKEY + LOCK;                       // Set LOCK bit
}
//写入字
//void FlashWriteWord(unsigned long addr,unsigned long Data)
//{
//    unsigned long *FlashPtr = (unsigned long *)addr;
//    FCTL1 = FWKEY + WRT;                        // Set WRT bit for write operation
//    FCTL3 = FWKEY;                              // Clear Lock bit
//    *FlashPtr = Data;                           // Save Data
//    WaitForEnable();                            //Busy
//    FCTL1 = FWKEY;                              // Clear WRT bit
//    FCTL3 = FWKEY + LOCK;                       // Set LOCK bit
//}
//读取字 或 字节
unsigned char FlashReadChar(unsigned int Addr)
{
    unsigned char Data;
    unsigned char *FlashPtr = (unsigned char *) Addr;
    Data = *FlashPtr;
    return(Data);
}

unsigned int FlashReadWord(unsigned int Addr)
{
    unsigned int Data;
    unsigned int *FlashPtr = (unsigned int *)Addr;
    Data = *FlashPtr;
    return(Data);
}
/*
//恢复函数
void RecData(){
    unsigned char Model;
    unsigned char i;
    Model=FlashReadChar(InfoA);
    if(Model<4){
        for(i=0;i<11;i++){
            MdO[i]=FlashReadChar(InfoA+i);
        }
    }else if(Model == 4){
        for(i=0;i<128;i++){
            MdT[i]=FlashReadChar(InfoB+i);
        }
    }
}*/
/*
void WriData(){
    unsigned char i, j;
    if(cishu == 0){
        FlashWriteChar(InfoA,keyval);
        FlashWriteChar(InfoA+1,4);}
    if(keyval<4){
        FlashWriteChar(InfoA+2,dianya);
        FlashWriteChar(InfoA+6,4);
        for(i=0;i<4;i++){
            FlashWriteChar(InfoA+i+7,byte[i]);
        }
    }else if(keyval==4){
        for((i=cishu,j=0);i<(cishu+4);i++){
            FlashWriteChar(InfoB+i,byte[j]);
        }
    }
}
*/
