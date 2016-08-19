#include <bcm2835.h>
#include <stdio.h>
#include <unistd.h>
#include "st7735.h"


/* Init script function */
struct st7735_function {
        uint16_t cmd;
        uint16_t data;
};

/* Init script commands */
enum st7735_cmd {
        ST7735_START,
        ST7735_END,
        ST7735_CMD,
        ST7735_DATA,
        ST7735_DELAY
};

static struct st7735_function st7735_cfg_script[] = {
        { ST7735_START, ST7735_START},
        { ST7735_CMD, ST7735_SWRESET},
        { ST7735_DELAY, 150},
        { ST7735_CMD, ST7735_SLPOUT},
        { ST7735_DELAY, 500},
        { ST7735_CMD, ST7735_FRMCTR1},
        { ST7735_DATA, 0x01},
        { ST7735_DATA, 0x2c},
        { ST7735_DATA, 0x2d},
        { ST7735_CMD, ST7735_FRMCTR2},
        { ST7735_DATA, 0x01},
        { ST7735_DATA, 0x2c},
        { ST7735_DATA, 0x2d},
        { ST7735_CMD, ST7735_FRMCTR3},
        { ST7735_DATA, 0x01},
        { ST7735_DATA, 0x2c},
        { ST7735_DATA, 0x2d},
        { ST7735_DATA, 0x01},
        { ST7735_DATA, 0x2c},
        { ST7735_DATA, 0x2d},
        { ST7735_CMD, ST7735_INVCTR},
        { ST7735_DATA, 0x07},
        { ST7735_CMD, ST7735_PWCTR1},
        { ST7735_DATA, 0xa2},
        { ST7735_DATA, 0x02},
        { ST7735_DATA, 0x84},
        { ST7735_CMD, ST7735_PWCTR2},
        { ST7735_DATA, 0xc5},
        { ST7735_CMD, ST7735_PWCTR3},
        { ST7735_DATA, 0x0a},
        { ST7735_DATA, 0x00},
        { ST7735_CMD, ST7735_PWCTR4},
        { ST7735_DATA, 0x8a},
        { ST7735_DATA, 0x2a},
        { ST7735_CMD, ST7735_PWCTR5},
        { ST7735_DATA, 0x8a},
        { ST7735_DATA, 0xee},
        { ST7735_CMD, ST7735_VMCTR1},
        { ST7735_DATA, 0x0e},
        { ST7735_CMD, ST7735_INVOFF},
        { ST7735_CMD, ST7735_MADCTL},
#if ( CONFIG_FB_ST7735_RGB_ORDER_REVERSED == 1 )
        { ST7735_DATA, 0xc0},
#else
        { ST7735_DATA, 0xc8},
#endif
        { ST7735_CMD, ST7735_COLMOD},
        { ST7735_DATA, 0x05},
#if 0 /* set_addr_win will set these, so no need to set them at init */
        { ST7735_CMD, ST7735_CASET},
        { ST7735_DATA, 0x00},
        { ST7735_DATA, 0x00 + ST7735_COLSTART},
        { ST7735_DATA, 0x00},
        { ST7735_DATA, HY18SPI_WIDTH - 1 + ST7735_COLSTART},
        { ST7735_CMD, ST7735_RASET},
        { ST7735_DATA, 0x00},
        { ST7735_DATA, 0x00 + ST7735_ROWSTART},
        { ST7735_DATA, 0x00},
        { ST7735_DATA, HY18SPI_HEIGHT - 1 + ST7735_ROWSTART},
#endif
        { ST7735_CMD, ST7735_GMCTRP1},
        { ST7735_DATA, 0x02},
        { ST7735_DATA, 0x1c},
        { ST7735_DATA, 0x07},
        { ST7735_DATA, 0x12},
        { ST7735_DATA, 0x37},
        { ST7735_DATA, 0x32},
        { ST7735_DATA, 0x29},
        { ST7735_DATA, 0x2d},
        { ST7735_DATA, 0x29},
        { ST7735_DATA, 0x25},
        { ST7735_DATA, 0x2b},
        { ST7735_DATA, 0x39},
        { ST7735_DATA, 0x00},
        { ST7735_DATA, 0x01},
        { ST7735_DATA, 0x03},
        { ST7735_DATA, 0x10},
        { ST7735_CMD, ST7735_GMCTRN1},
        { ST7735_DATA, 0x03},
        { ST7735_DATA, 0x1d},
        { ST7735_DATA, 0x07},
        { ST7735_DATA, 0x06},
        { ST7735_DATA, 0x2e},
        { ST7735_DATA, 0x2c},
        { ST7735_DATA, 0x29},
        { ST7735_DATA, 0x2d},
        { ST7735_DATA, 0x2e},
        { ST7735_DATA, 0x2e},
        { ST7735_DATA, 0x37},
        { ST7735_DATA, 0x3f},
        { ST7735_DATA, 0x00},
        { ST7735_DATA, 0x00},
        { ST7735_DATA, 0x02},
        { ST7735_DATA, 0x10},
///#if 0 /* init_display will turn on the display after clearing it */
        { ST7735_CMD, ST7735_DISPON},
        { ST7735_DELAY, 100},
///#endif
        { ST7735_CMD, ST7735_NORON},
        { ST7735_DELAY, 10},
        { ST7735_END, ST7735_END},
};

// hardware reset
void hy18spi_reset()
{
  bcm2835_gpio_clr(RPI_V2_GPIO_P1_22); //GPIO25 - RESET
  delay(10);
  bcm2835_gpio_set(RPI_V2_GPIO_P1_22); //GPIO25 - RESET
  delay(120);
}

void hy18spi_init_gpio()
{
   // SPI pins have been initialized including GPIO8 - CS
   // A0 and RESET pins set to output
   bcm2835_gpio_fsel(RPI_V2_GPIO_P1_18, BCM2835_GPIO_FSEL_OUTP); // A0 - GPIO24
   bcm2835_gpio_fsel(RPI_V2_GPIO_P1_22, BCM2835_GPIO_FSEL_OUTP); // RESET - GPIO25
   // A0 set high
   bcm2835_gpio_set(RPI_V2_GPIO_P1_18); //GPIO24 - A0
   // RESET set high
   bcm2835_gpio_set(RPI_V2_GPIO_P1_22); //GPIO25 - RESET
   // CS set high
   bcm2835_gpio_set(RPI_V2_GPIO_P1_24); //GPIO8 - CS

}

// gpio deinit
void hy18spi_end_gpio()
{
   // A0 and RESET pins set to input
   bcm2835_gpio_fsel(RPI_V2_GPIO_P1_18, BCM2835_GPIO_FSEL_INPT); // A0 - GPIO24
   bcm2835_gpio_fsel(RPI_V2_GPIO_P1_22, BCM2835_GPIO_FSEL_INPT); // RESET - GPIO25
}

uint8_t hy18spi_transfer_cmd(uint8_t value)
{
   bcm2835_gpio_clr(RPI_V2_GPIO_P1_18); //GPIO24 - A0
   return bcm2835_spi_transfer(value);
}

uint8_t hy18spi_transfer_data(uint8_t value)
{
   bcm2835_gpio_set(RPI_V2_GPIO_P1_18); //GPIO24 - A0
   return bcm2835_spi_transfer(value);
}

void hy18spi_set_addr_window(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
  hy18spi_transfer_cmd(ST7735_CASET);         // column addr set
  hy18spi_transfer_data(0x00);
  hy18spi_transfer_data(x0+ST7735_COLSTART);  // XSTART
  hy18spi_transfer_data(0x00);
  hy18spi_transfer_data(x1+ST7735_COLSTART);  // XEND

  hy18spi_transfer_cmd(ST7735_RASET);         // row addr set
  hy18spi_transfer_data(0x00);
  hy18spi_transfer_data(y0+ST7735_ROWSTART);  // YSTART
  hy18spi_transfer_data(0x00);
  hy18spi_transfer_data(y1+ST7735_ROWSTART);  // YEND
}

void hy18spi_set_rgb(uint16_t color)
{
  uint8_t x, y;
  hy18spi_set_addr_window(0, 0, HY18SPI_WIDTH - 1, HY18SPI_HEIGHT - 1);
  hy18spi_transfer_cmd(ST7735_RAMWR);  // write to RAM
  for (x=0; x < HY18SPI_WIDTH; x++)
  {
    for (y=0; y < HY18SPI_HEIGHT; y++)
    {
      hy18spi_transfer_data(color >> 8);
      hy18spi_transfer_data(color);
    }
  }
  hy18spi_transfer_cmd(ST7735_NOP);
}


void hy18spi_set_pixel(uint16_t x, uint16_t y, uint16_t color)
{
  hy18spi_set_addr_window(x,y,x+1,y+1);
  hy18spi_transfer_cmd(ST7735_RAMWR);  // write to RAM
  hy18spi_transfer_data(color >> 8);
  hy18spi_transfer_data(color);
}

static void hy18spi_run_cfg_script()
{
        int i = 0;
        int end_script = 0;

        do {
                switch (st7735_cfg_script[i].cmd)
                {
                case ST7735_START:
                        break;
                case ST7735_CMD:
                        hy18spi_transfer_cmd(st7735_cfg_script[i].data & 0xff);
                        break;
                case ST7735_DATA:
                        hy18spi_transfer_data(st7735_cfg_script[i].data & 0xff);
                        break;
                case ST7735_DELAY:
                        delay(st7735_cfg_script[i].data);
                        break;
                case ST7735_END:
                        end_script = 1;
                }
                i++;
        } while (!end_script);
}
// hy-1.8 init
void hy18spi_init(void)
{
  hy18spi_reset();
  hy18spi_run_cfg_script();
      // initialize spi
    bcm2835_spi_begin();
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      // The default
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE3);                   // MODE 3
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_8); //< 8 = 32ns = 31.25MHz
    bcm2835_spi_chipSelect(BCM2835_SPI_CS0);                      // The default
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);      // the default

    // initialize gpio
    hy18spi_init_gpio();
}

// hy-1.8 deinit
void hy18spi_end(void)
{
  hy18spi_end_gpio();
}

