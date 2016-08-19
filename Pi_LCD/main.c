/*
Here is my modified version of Pawel Wodnicki HY-1.8 SPI LCD for Raspberry Pi
it comes with custom LCD function for driving LCD easily

// Original Author: Pawel Wodnicki
// Copyright (C) 2013 Pawel Wodnicki
// Additional LCD Function : X-death 2016 (C) Ultimate-Consoles

/*
 PCB Silskreen is wrong : china powaa it's not an I²C but an SPI slave !

 SDA -> MOSI
 A0 -> D/C

HY-1.8 J1        Raspberry Pi P1      bcm2835 lib pin
1 - GND          - P1.6 GND
2 - VCC          - P1.2 5V Power
6 - RESET        - P1.22 GPIO25        RPI_V2_GPIO_P1_22
7 - A0           - P1.18 GPIO24        RPI_V2_GPIO_P1_18
8 - SDA          - P1.19 GPIO10 MOSI   RPI_V2_GPIO_P1_19
9 - SCK          - P1.23 GPIO11 SCLK   RPI_V2_GPIO_P1_23
10 - CS          - P1.24 GPIO8 CE0     RPI_V2_GPIO_P1_24
15 - LED+         - P1.4 5V Power
16 - LED-         - P1.9 GND

*/

#include <bcm2835.h>
#include <stdio.h>
#include <unistd.h>

#include "st7735.h"
#include "LCD.h"


int main(int argc, char **argv)
{
    // intialize bcm2835 library
    if (!bcm2835_init())
        return 1;

    // initialize display and SPI
    hy18spi_init();


    // set black background
    //hy18spi_set_rgb(0x0000FF);
    // lcd_paint_screen (BLUE_16);
    // lcd_set_pixel(64,80,WHITE_16);
    // lcd_invert_display();
    //  lcd_print_char(64,80,WHITE_16,0x61,cour10_font);

    // Display BMP function

lcd_Display_BMP("tails.bmp");
//lcd_Display_BMP("Sephiroth.bmp");

      /*
    // deinitialize display
    hy18spi_end();

    // deintialize spi
    bcm2835_spi_end();

    // deintialize bcm2835 library
    bcm2835_close();
    */

    return 0;
}

