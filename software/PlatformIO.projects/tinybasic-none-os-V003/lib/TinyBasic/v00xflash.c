// TTBASIC CH32V00x flash utilities
//
// v00xflash.c
//
//    V002/3 16K Flash, 0x0800 0000 ~ 0x0800 4000, 15k addr= 0x0800 3C00 ~ 0x0800 4000
//    V004/5 32K Flash, 0x0800 0000 ~ 0x0800 8000, 31k addr= 0x0800 7C00 ~ 0x0000 8000
//    V006   62K Flash, 0x0800 0000 ~ 0x0800 F800, 61k addr= 0x0000 F400 ~ 0x0000 F800
/*********************************************************************
 * DBGMCU_GetCHIPID
 *    CH32V006K8U6-0x006006x0
 *    CH32V006E8R6-0x006106x0
 *    CH32V006F8U6-0x006206x0
 *    CH32V006F8P6-0x006306x0
 *
 *    CH32M007G8R6-0x007008x0
 *    CH32V007E8R6-0x007106x0
 *    CH32V007F8U6-0x007206x0
 *
 *    CH32V005E6R6-0x005006x0
 *    CH32V005F6U6-0x005106x0
 *    CH32V005F6P6-0x005206x0
 *    CH32V005D6U6-0x005306x0
 *
 *    CH32V002F4P6-0x002006x0
 *    CH32V002F4U6-0x002106x0
 *    CH32V002A4M6-0x002206x0
 *    CH32V002D4U6-0x002306x0
 *    CH32V002J4M6-0x002406x0
 *
 *    CH32V004F6P1-0x004006x0
 *    CH32V004F6U1-0x00410600
 *
 *    CH32V003F4P6-0x003005x0
 *    CH32V003F4U6-0x003105x0
 *    CH32V003A4M6-0x003205x0
 *    CH32V003J4M6-0x003305x0
 */

#include <ch32v00x_dbgmcu.h>
#include <debug.h>
/*
//16-1
#define PAGE_WRITE_START_ADDR_15K ((uint32_t)0x08003C00)// Start from 15K
//32-1
#define PAGE_WRITE_START_ADDR_31K ((uint32_t)0x08007C00)// Start from 31K
//62-1
#define PAGE_WRITE_START_ADDR_61K ((uint32_t)0x0800F400)// Start from 61K
*/
#define FLASH_PAGE_SIZE1K 1024

uint16_t getChipFlashSize() {
    /*
    uint32_t iSize;
    uint32_t chip = (DBGMCU_GetCHIPID() >> 20);  // right shift 20bit
    switch (chip) {
        case 0x006:  // 62K
        case 0x007:
            iSize = 0xF800;
            break;
        case 0x004:  // 32K
        case 0x005:
            iSize = 0x8000;
            break;
        case 0x002:  // 16K
        case 0x003:
        default:  // 16K
            iSize = 0x4000;
            break;
    }
    return iSize; //bytes
    */
   //return ((*(uint32_t *)0x1FFFF7E0)&0x00FF)*1024;//R16_ESIG_FLACAP 0x1FFFF7E0 闪存容量寄存器   
   return ((*(uint16_t *)0x1FFFF7E0))<<10;//R16_ESIG_FLACAP 0x1FFFF7E0 闪存容量寄存器 * 1024
}

volatile uint32_t Address = 0x0;
volatile uint32_t end_adr = 0x0;
volatile uint16_t Data = 0xAAAA;
volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;
void flash_save(uint8_t *adr) {
    unsigned int i;
    //
    FLASH_Unlock();  // ロックを外す。
    // フラッフュの消去は1kbyteで行う。
    FLASHStatus = FLASH_ErasePage(FLASH_BASE + getChipFlashSize() - 1024);
    // 書き込みする。　２バイトづつなのだ。
    Address = FLASH_BASE + getChipFlashSize() - 1024;
    end_adr = FLASH_BASE + getChipFlashSize();
    i = 0;
    while ((Address < end_adr) && (FLASHStatus == FLASH_COMPLETE)) {
        Data = (adr[i + 1] << 8) + adr[i];
        FLASHStatus = FLASH_ProgramHalfWord(Address, Data);
        Address = Address + 2;
        i = i + 2;
    }
    FLASH_Lock();
}

//
// プログラムをロードする関数。。。読み出します。
void flash_load(uint8_t *adr) {
    unsigned int i;  // ループカウンタ
    uint8_t *Address;
    Address = (uint8_t *)(FLASH_BASE + getChipFlashSize() - 1024);
    for (i = 0; i < 1024; i++) {  // 領域のサイズだけ繰り返す
        *adr = *Address;          // 読み込み
        ++Address;
        ++adr;
    }
}

// BOOTの目印を返す　関数
uint8_t flash_boot() {
    uint8_t *boot;
    boot = (uint8_t *)(FLASH_BASE + getChipFlashSize() - 1);
    // 領域番号０の末尾の値を持ち帰る
    return (*boot);
}