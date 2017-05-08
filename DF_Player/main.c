// Example program for use with DFPlayer Mini
// Shows how to interface DFPlayer Mini audio module
//
// Author: X-death
// Copyright (C) 2017 Ultimate-Consoles

//Pinout Raspberry > DFPlayer

//Pin1 3v3 Power  > PIN 1  VCC Input Voltage
//Pin9 Ground     > PIN 10 Power GND 
//Pin8 Serial TXD > PIN 2  UART Serial Input RX

//Pinout DFPlayer > Audio OUT

//Pin4 DAC_R > Audio Right
//Pin5 DAC_L > Audio Left

#include <stdio.h>

#include <wiringPi.h>     // Special GPIO Library
#include <wiringSerial.h> // Serial core for WiringPi

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

void DF_Write_Command(unsigned char Command,unsigned char data,unsigned long delay)
{
  serialPutchar(fd,START); // Byte Start
  serialPutchar(fd,VERSION);
  serialPutchar(fd,0x06);
  serialPutchar(fd,Command); // Send Custom Command
  serialPutchar(fd,0x00);
  serialPutchar(fd,0x00);  // data High Byte
  serialPutchar(fd,data); // data Low Byte
  serialPutchar(fd,STOP); // Byte Stop
  delayMicroseconds(delay);
}

void DF_Reset(void)
{
  DF_Write_Command(0x0C,0,100); // Reset DF Module
}

void DF_SourceDevice(void)
{
  DF_Write_Command(0x09,0x02,200); // Select SD_Card as source device
}

void DF_SetVolume(unsigned char volume) // Change Volume ( 0 - 30 )
{
  if (volume > 30) volume = 30;
  DF_Write_Command(0x06,volume,10); 
}

void DF_PlayTrack(unsigned char Track) // Play Specified Track
{
  DF_Write_Command(0x03,Track,10); 
}

void DF_StopTrack(void) // Stop current Track
{
  DF_Write_Command(0x16,0,20); 
}

void DF_PauseTrack(void) // Pause current Track
{
  DF_Write_Command(PAUS,0,20); 
}

void DF_ResumeTrack(void) // Resume current Track
{
  DF_Write_Command(PLAY,0,20); 
}

int main(int argc, char **argv)
{   
    printf("=- DF Mini Audio Player -=\n\n");

    // Init Wiring Pi lib

    if (wiringPiSetup() == -1)
    {
       printf("Unable to start wiringPi \n");
       return 1;
     }
    printf("WiringPi started sucessfully..\n");

    // Init serial communication

    if((fd = serialOpen ("/dev/ttyAMA0",9600)) < 0)
    {
       printf("Unable to open serial device \n");
       return 1;
    }
    printf("Serial COM opened sucessfully..\n\n");

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
          DF_Write_Command(0x03,Number,10); 
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
    serialClose(fd);
    return 0;
}
