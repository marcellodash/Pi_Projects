#ifndef __ST7735_H
#define __ST7735_H

/*
 Defines for ST7735 LCD controller
borrowed from ST7735 frame buffer driver in
drivers/video/st7735
*/

/* ST7735 Commands */
#define ST7735_NOP	0x0
#define ST7735_SWRESET	0x01
#define ST7735_RDDID	0x04
#define ST7735_RDDST	0x09
#define ST7735_SLPIN	0x10
#define ST7735_SLPOUT	0x11
#define ST7735_PTLON	0x12
#define ST7735_NORON	0x13
#define ST7735_INVOFF	0x20
#define ST7735_INVON	0x21
#define ST7735_DISPOFF	0x28
#define ST7735_DISPON	0x29
#define ST7735_CASET	0x2A
#define ST7735_RASET	0x2B
#define ST7735_RAMWR	0x2C
#define ST7735_RAMRD	0x2E
#define ST7735_COLMOD	0x3A
#define ST7735_MADCTL	0x36
#define ST7735_FRMCTR1	0xB1
#define ST7735_FRMCTR2	0xB2
#define ST7735_FRMCTR3	0xB3
#define ST7735_INVCTR	0xB4
#define ST7735_DISSET5	0xB6
#define ST7735_PWCTR1	0xC0
#define ST7735_PWCTR2	0xC1
#define ST7735_PWCTR3	0xC2
#define ST7735_PWCTR4	0xC3
#define ST7735_PWCTR5	0xC4
#define ST7735_VMCTR1	0xC5
#define ST7735_RDID1	0xDA
#define ST7735_RDID2	0xDB
#define ST7735_RDID3	0xDC
#define ST7735_RDID4	0xDD
#define ST7735_GMCTRP1	0xE0
#define ST7735_GMCTRN1	0xE1
#define ST7735_PWCTR6	0xFC

// forwad declarations
void hy18spi_init_gpio(void);
void hy18spi_end_gpio(void);
void hy18spi_reset();
void hy18spi_init(void);
void hy18spi_end(void);
uint8_t hy18spi_transfer_cmd(uint8_t value);
uint8_t hy18spi_transfer_data(uint8_t value);
void hy18spi_probe();
void hy18spi_set_rgb(uint16_t color);
void hy18spi_set_pixel(uint16_t x, uint16_t y, uint16_t color);

#define HY18SPI_WIDTH 128
#define HY18SPI_HEIGHT 160

// configuration - comment to disable
#define CONFIG_FB_ST7735_PANEL_TYPE_RED_TAB 1
#define CONFIG_FB_ST7735_RGB_ORDER_REVERSED 1

#if ( CONFIG_FB_ST7735_PANEL_TYPE_RED_TAB == 1 )
# define ST7735_COLSTART 0
# define ST7735_ROWSTART 0
#else
# define ST7735_COLSTART 2
# define ST7735_ROWSTART 1
#endif


#endif
