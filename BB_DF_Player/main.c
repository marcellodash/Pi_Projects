// Example program for use with DFPlayer Mini
// Shows how to interface DFPlayer Mini audio module
//
// Author: X-death
// Copyright (C) 2017 Ultimate-Consoles

//Pinout Raspberry > DFPlayer

//Pin1 3v3 Power  > PIN 1  VCC Input Voltage
//Pin9 Ground       > PIN 10 Power GND 
//Pin16 BCM 23    > PIN 2  UART Serial Input RX

//Pinout DFPlayer > Audio OUT

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)

// Access from ARM Running Linux

#define BCM2708_PERI_BASE        0x3F000000
#define GPIO_BASE                (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */

// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))

#define GPIO_SET *(gpio+7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR *(gpio+10) // clears bits which are 1 ignores bits which are 0

#define GET_GPIO(g) (*(gpio+13)&(1<<g)) // 0 if LOW, (1<<g) if HIGH

#define GPIO_PULL *(gpio+37) // Pull up/pull down
#define GPIO_PULLCLK0 *(gpio+38) // Pull up/pull down clock

int  mem_fd;
void *gpio_map;

// I/O access
volatile unsigned *gpio;

// Global Variable

static int fd;
static int Menu=0;
static int Number=0;

//DFSerial Communication Command

#define START     0x7E //Start command BYTE
#define VERSION   0xFF //Version command BYTE
#define STOP      0xEF //Stop copmmand BYTE

#define NEXT    0x01 //Next Track
#define PREV    0x02 //Previous Track
#define INCV    0x04 //Increase Volume
#define DECV    0x05 //Decrease Volume
#define MODE    0x08 //Specify playback mode (0/1/2/3) (repeat/folder_repeat/track_repeat/random)
#define PLAY    0x0D //Play Track
#define PAUS    0x0E //Pause Track 

void setup_io();

void wait(unsigned char delay)
{
unsigned long tmp=0; 
for(tmp=0; tmp<31*delay; tmp++) 
{
__asm__("nop");  // 62,5ns
__asm__("nop");  // 62,5ns
__asm__("nop");  // 62,5ns
__asm__("nop");  // 62,5ns
}  
}

void uart_tx_bit_bang (unsigned char val)
{
 unsigned char i=0;
 GPIO_CLR = 1<<23;   // turn TX to logic '0'
 wait(167);// wait 104 탎
   for ( i = 8; i !=0; i--)
     {
         if ( val & 0x01 )  GPIO_SET = 1<<23;  // turn TX to logic '1'
        else  GPIO_CLR = 1<<23; 
       val >>= 1;
        wait(167);// wait 104 탎
     }
  GPIO_SET = 1<<23;   // Stop bit
wait(167);// wait 104 탎
} 

void DF_Write_Command(unsigned char Command,unsigned char data)
{
uart_tx_bit_bang(START); 
uart_tx_bit_bang(VERSION); 
uart_tx_bit_bang(0x06); 
uart_tx_bit_bang(Command); 
uart_tx_bit_bang(0x00); 
uart_tx_bit_bang(0x00); 
uart_tx_bit_bang(data); 
uart_tx_bit_bang(STOP); 
}

void DF_Reset(void)
{
  DF_Write_Command(0x0C,0); // Reset DF Module
  wait(165);// wait 100 탎
}

void DF_SourceDevice(void)
{
  DF_Write_Command(0x09,0x02); // Select SD_Card as source device
wait(165);// wait 200 탎
}

void DF_SetVolume(unsigned char volume) // Change Volume ( 0 - 30 )
{
  if (volume > 30) volume = 30;
  DF_Write_Command(0x06,volume); 
 wait(16);// wait 10 탎
}

void DF_PlayTrack(unsigned char Track) // Play Specified Track
{
  DF_Write_Command(0x03,Track); 
  wait(16);// wait 10 탎
}

void DF_StopTrack(void) // Stop current Track
{
  DF_Write_Command(0x16,0); 
  wait(16);// wait 10 탎
}

void DF_PauseTrack(void) // Pause current Track
{
  DF_Write_Command(PAUS,0); 
}

void DF_ResumeTrack(void) // Resume current Track
{
  DF_Write_Command(PLAY,0); 
 wait(16);// wait 20 탎
}

int main(int argc, char **argv)
{   
    printf("=- DF Mini Audio Player -=\n\n");
  int g,rep;

  // Set up gpi pointer for direct register access
  setup_io();

  // Set GPIO pin 23 to output
  INP_GPIO(23); // must use INP_GPIO before we can use OUT_GPIO
  OUT_GPIO(23);
    DF_Reset();
    DF_SourceDevice();
    DF_SetVolume(0x0F);

while (Menu ==0 )

{
    printf("---Menu---\n\n");
    printf("1. Play Track \n");
    printf("2. Stop Track \n");
    printf("3. Change Volume \n");
    printf("4. Pause/Resume Track \n");
    printf("5. Exit  DF Audio Player \n");

   scanf("%d",&Menu);
     
   switch (Menu)
   {
      case 1: // Play track
          DF_SetVolume(20);
          printf("Select track number :\n");
          scanf("%d",&Number); 
          DF_Write_Command(0x03,Number); 
         wait(16);// wait 10 탎
          printf("1. Playing track number %d \n",Number);
          Menu=0;
          break;

     case 2: // Stop track
           DF_StopTrack();
           Menu=0;
           break;

     case 3: // Change volume
          printf("Enter your volume (1- 30) :\n");
          Number=0;
          scanf("%d",&Number);
          DF_SetVolume(Number);
          printf("Volume is now set to %d \n",Number);
          Number=0;
          Menu=0;
          break;

     case 4: // Change volume
          if ( Number==0) {DF_PauseTrack();  Number=1; printf("Pause \n");}
          else if ( Number==1) {DF_ResumeTrack(); Number=0; printf("Play  \n");}
          Menu=0;
          break;

     case 5: // Exit Audio Player
          printf("Exit DF Audio Player \n");
          DF_StopTrack();
          Menu=1;
          break;  
   }

}
    return 0;
}

//
// Set up a memory regions to access GPIO
//
void setup_io()
{
   /* open /dev/mem */
   if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
      printf("can't open /dev/mem \n");
      exit(-1);
   }

   /* mmap GPIO */
   gpio_map = mmap(
      NULL,             //Any adddress in our space will do
      BLOCK_SIZE,       //Map length
      PROT_READ|PROT_WRITE,// Enable reading & writting to mapped memory
      MAP_SHARED,       //Shared with other processes
      mem_fd,           //File to map
      GPIO_BASE         //Offset to GPIO peripheral
   );

   close(mem_fd); //No need to keep mem_fd open after mmap

   if (gpio_map == MAP_FAILED) {
      printf("mmap error %d\n", (int)gpio_map);//errno also set!
      exit(-1);
   }

   // Always use volatile pointer!
   gpio = (volatile unsigned *)gpio_map;


} // setup_io