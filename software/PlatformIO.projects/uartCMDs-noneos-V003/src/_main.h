/* 
 * _main.h
 *
 *    CH32V003 module enable/disable header for none-os sdk
 *
 *    by jimmy.su
 */
#ifndef _MAIN_H
#define _MAIN_H

//ALL DISABLED    Flash: [======    ]  60.1% (used 9840 bytes from 16384 bytes)

//ALL  ENABLED    Flash: [==========]  98.6% (used 16152 bytes from 16384 bytes)

//--------------------------------bytes----percent/16K
//ENABLE ANSI TITLE 10108-9840      268   =1.6%
#define _ENABLE_ANSITITLE 
//ENABLE NEO/LED BLINK
#define _ENABLE_BLINK
//ENABL SLEEP code:10572-10108      464   =2.8%
#define _ENABLE_SLEEP
//ENABLE timer code:11136-10572     564   =3.4%
#define _ENABLE_TIMER
//ENABLE jog code:11420-11136       284   =1.7%
#define _ENABLE_JOG
//ENABLE Neopixel code:11896-11420  476   =2.9%
#define _ENABLE_NEO
//ENABLE PLAY code:12528-11896      632   =3.9%
#define _ENABLE_PLAY
//ENABLE I2C  code:13080-12568      512   =3.1%
#define _ENABLE_I2C   
//ENABLE I2C SLAVE code:13428-13180 248   =1.4%
//#define _ENABLE_I2C_SLAVE 
//ENABLE OLED code:13616-12568     1048   =6.4%
//#define _ENABLE_OLED
//ENABLE SDFS  code:15504-13080    2424   =14.8%
//#define _ENABLE_SDFS 
//ENABLE EE  code:16152-13616      2536   =15.5%
#define _ENABLE_EE

#endif