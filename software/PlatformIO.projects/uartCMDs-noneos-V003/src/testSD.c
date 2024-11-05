/* port of mini-rv32ima to ch32v003 because why not */

#include <ch32v00x.h>
#include <stdio.h>
 
#include "hw_spi.h"
#include "pff.h"
 
// SD CS
#define SD_CS_GPIO GPIOC
#define SD_CS_PIN 0

#define GPIO_CNF_IN_ANALOG   0
#define GPIO_CNF_IN_FLOATING 4
#define GPIO_CNF_IN_PUPD     8
#define GPIO_CNF_OUT_PP      0
#define GPIO_CNF_OUT_OD      4
#define GPIO_CNF_OUT_PP_AF   8
#define GPIO_CNF_OUT_OD_AF   12

/* Configuration Mode enumeration */
/*
typedef enum
{
    GPIO_Mode_AIN = 0x0,
    GPIO_Mode_IN_FLOATING = 0x04,
    GPIO_Mode_IPD = 0x28,
    GPIO_Mode_IPU = 0x48,
    GPIO_Mode_Out_OD = 0x14,
    GPIO_Mode_Out_PP = 0x10,
    GPIO_Mode_AF_OD = 0x1C,
    GPIO_Mode_AF_PP = 0x18
} GPIOMode_TypeDef;
*/

void load_sd_file( uint32_t addr, const char filename[] );
FATFS fs; /* File system object */

//errorHalt
//
// print errors and die
void errorHalt(const char* s)  {
	printf("Error:%s\n",s);
	for (;;);
}

int testSD()
{
	//SystemInit();
	//Enable UART
	
	// Enable GPIOs
	RCC->APB2PCENR |= RCC_APB2Periph_GPIOC;

	// SD Card CS Push-Pull
	SD_CS_GPIO->CFGLR &= ~( 0xf << ( 4 * SD_CS_PIN ) );
	SD_CS_GPIO->CFGLR |= ( GPIO_Speed_50MHz | GPIO_CNF_OUT_PP ) << ( 4 * SD_CS_PIN );
	SD_CS_GPIO->BSHR = ( 1 << SD_CS_PIN );

	// Enable SPI
	SPI_init();
	SPI_begin_8();

	Delay_Ms( 2000 );

	printf("\033[2J\033[1;1H"); // clear terminal
	printf("\n\rrv32ima, compiled %s\n\r", __TIMESTAMP__);

   uint8_t buf[32];
  
  // Initialize SD and file system.
  if (pf_mount(&fs)) errorHalt("pf_mount");
  
  // Open test file.
  if (pf_open("TEST.TXT")) errorHalt("pf_open");
  
  // Dump test file to Serial.
  while (1) {
    UINT nr;
    if (pf_read(buf, sizeof(buf), &nr)) errorHalt("pf_read");
    if (nr == 0) break;
    //Serial.write(buf, nr);
  }
}
/*

// SD card file loading functions
void die( FRESULT rc )
{
	printf( "Failed with rc=%u.\n", rc );
	for ( ;; )
		;
}

void load_sd_file( uint32_t addr, const char filename[] )
{
	FATFS fatfs; // File system object 
	UINT br;
	BYTE buff[64];

	printf( "Mounting volume.\n\r" );
	FRESULT rc = pf_mount( &fatfs );
	if ( rc ) die( rc );

	printf( "Opening file \"%s\"\n\r", filename );
	rc = pf_open( filename );
	if ( rc ) die( rc );

	printf( "Loading image into RAM\n\r" );

	uint32_t total_bytes = 0;
	uint8_t cnt = 0;
	const char spinner[] = "/-\\|";

	for ( ;; )
	{
		rc = pf_read( buff, sizeof( buff ), &br ); // Read a chunk of file 
		if ( rc || !br ) break; // Error or end of file 

		psram_write( addr, buff, br );
		total_bytes += br;
		addr += br;

		if(total_bytes % (16*1024) == 0){
			cnt++;
			printf("%d kb so far...  ", total_bytes/1024);
			putchar(spinner[cnt%4]);
			putchar('\r');
		}

	}
	if ( rc ) die( rc );
	printf("\n\rLoaded %d kilobytes.\n\r", total_bytes/1024);
}
*/