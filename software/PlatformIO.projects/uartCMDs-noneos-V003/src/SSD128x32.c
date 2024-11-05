/*
 *
 *
 * 
 * 
 */
//Buffered SSD1306
//#include <oled_segment.h>
//unbuffered SSD1306
#include <oled_small.h>
#include <debug.h>
#include "robotoDigit.h"

void oledHello(void) {

   // Setup CH32V003 I2C pins
   //I2C_init();
   
   // Init the OLED display
   OLED_init();
   OLED_fill(255);
   Delay_Ms(500);
   
   // Write text
   OLED_clear();
   OLED_setpos(0, 0); //ROW 0
   OLED_print("0123456789ABCDEF01234"); //21x6 = 126
   OLED_setpos(0, 1); //ROW 1
   OLED_print("----------------");
   OLED_setpos(0, 2); //ROW 2
   OLED_print("Hello world!");
   OLED_setpos(0, 3); //ROW 3
   OLED_print("=====================");
   //OLED_DrawBitmap(100, 0, fontRobotWidth[7], 24 , fontRobotAddr[7], 0);
   //OLED_DrawBitmap(115, 0, fontRobotWidth[8], 24 , fontRobotAddr[8], 0);
   OLED_plotRobot(128-(13*5), 0, "89.64");

/*
   // Write inverted text
   OLED_clear();
   OLED_printS("Inverted text!", true);

   // Draw WCH Logo
   OLED_clear();
   OLED_DrawLogo();

   // Draw single pixels (Buffered version)
   OLED_clear();
   OLED_DrawPixel(0,30,true);
   OLED_DrawPixel(1,31,true);
   OLED_DrawPixel(2,32,true);
   OLED_DrawPixel(3,33,true);
   OLED_DrawPixel(4,34,true);
   OLED_DrawPixel(5,35,true);
   OLED_DrawPixel(6,36,true);
   OLED_DrawPixel(7,37,true);
   OLED_DrawPixel(8,38,true);
   OLED_DrawPixel(9,39,true);
   OLED_DisplayBuffer();
   
   // Draw small bitmap
   OLED_clear();
   OLED_DrawBitmap(128-38, 64-56+1, 38, 56, dust_bmp);
   
   // Fill screen with a pattern (255 for white screen, 0 for black screen)
   OLED_fill(255);
   
   // Draw horizontal line (Buffered version)
   OLED_clear();
   OLED_DrawLine(false, 2, true); 
   OLED_DisplayBuffer();
   
   // Draw vertical line (Buffered version)
   OLED_clear();
   OLED_DrawLine(true, 2, true); 
   OLED_DisplayBuffer();
   
   // Draw multiple lines (Buffered version)
   OLED_clear();
   short lines[] = {0,2,3,5,6,30};
   OLED_DrawMultipleLines(false, lines, 6, true);
   OLED_DisplayBuffer();
   */
}
