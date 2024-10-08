// ===================================================================================
// TOYOSHIKI Tiny BASIC
// ===================================================================================
// (C) 2012 Tetsuya Suzuki, GNU General Public License
// https://github.com/rutles/ttbasic_arduino
//
//
// Modified by jimmy.su@2024
//    -Modify mem size for CH32V003
//    -Modify UART in/out, enter-key
//    -Add TIME
//RAM:   [========= ]  87.1% (used 1784 bytes from 2048 bytes)
//Flash: [=======   ]  67.0% (used 10976 bytes from 16384 bytes)
//
//    -Add SAVE, BOOT, LOAD
//RAM:   [========= ]  87.9% (used 1800 bytes from 2048 bytes)
//Flash: [=======   ]  70.8% (used 11608 bytes from 16384 bytes)
//
//
#ifndef _TTBASIC_H_
#define _TTBASIC_H_

#include <stdio.h>
#include "myUsartInt.h"
#include "v00xflash.h"

// Define special keys
//#define KEY_ENTER     10      // ENTER key: LF
#define KEY_ENTER     13      // ENTER key: 0X0D
#define KEY_ABORT     27      // ABORT key: ESC

// TOYOSHIKI Tiny BASIC symbols
// TO-DO Rewrite defined values to fit your machine as needed
#define SIZE_LINE     64      // Command line buffer length + NULL
#define SIZE_IBUF     64      // i-code conversion buffer size
#define SIZE_LIST     1024    // List buffer size
#define SIZE_ARRY     5       // Array area size
#define SIZE_GSTK     6       // GOSUB stack size (2*3 2/nest)
#define SIZE_LSTK     15      // FOR stack size (5*3 5/nest)

// Edition string
#define STR_EDITION   "CH32V003t"

// Terminal control
/*
#define c_putch(c)    UART_write(c)
#define c_getch( )    UART_read()
#define c_kbhit( )    UART_available()
*/

#define c_putch(c)    UART_write(c)
#define c_getch( )    getRx()
#define c_kbhit( )    RxCount
// Functions
void basic();

#endif