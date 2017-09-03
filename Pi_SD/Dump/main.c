// Example program for use with bcm2835 & PFatFS library
// Shows how to interface an SD/Micro SD with SPI Bus
// Print content of file message.txt

// Author: X-death
// Copyright (C) 2017 Ultimate-Consoles

//Pinout Raspberry > SPI SD
//BCM10 (MOSI) > SD MISO
//BCM09 (MISO) > SD MOSI
//BCM11 (SCLK) > SD  SCK
//BCM08 (CE0)  > SD  /CS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <bcm2835.h> // Special GPIO Library for Raspberry Pi

// SD Driver Part

#include "diskio.h"
#include "pff.h"

void die (		/* Stop with dying message */
	FRESULT rc	/* FatFs return value */
)
{
	printf("Failed with rc=%u.\n", rc);
	for (;;) ;
}

void put_rc (FRESULT rc)
{
	const char *p;
	FRESULT i;
	static const char str[] =
		"OK\0DISK_ERR\0NOT_READY\0NO_FILE\0NOT_OPENED\0NOT_ENABLED\0NO_FILE_SYSTEM\0";

	for (p = str, i = 0; i != rc && *p; i++) {
		while(*p++) ;
	}
	printf("rc=%u FR_%S\n", rc, p);
}


// Main Program

int main(void)
{
	
	// PFatFs variable
	
	BYTE res; // byte for Error code
	FATFS fs;			/* File system object */
	
	// Var for dump
	long p1;
	long p2;
	long j;
	UINT w;
	char *Rom_Data;	

	// Custom variable
	
	int retry=0;
	
	
	if (!bcm2835_init())
    {
      printf("bcm2835_init failed. Are you running as root??\n");
      return 1;
    }
	
	
 printf("-= pFatFs test -= \n\n");
 
 /* - Initialize SD Card */
 
 printf("Intialize SD Card ...\n");
 res = disk_initialize();
 printf("res=%d\n",res);
 
 while ( res!=0 && retry!=5)
 {
                printf("Failed SD init will retry 5 times ...\n");
                retry=retry+1;
                res = disk_initialize();
                if (retry == 5){ die(res);}
}
             
 printf("SD initialized sucessfully !\n\n");
 
  /* - Mount a volume and print some info - */
  
  retry=0;
  printf("Mount a volume...\n");
  res = pf_mount(&fs);
  printf("res=%d\n",res);
  
   while ( res!=0 && retry!=5)
 {
                printf("Mount Failed will retry 5 times ...\n");
                retry=retry+1;
                res = disk_initialize();
                if (retry == 5){ die(res);}
}

 printf("Volume mounted sucessfully ! \n");
 printf("FAT type = %u\nBytes/Cluster = %lu\n"
"Number of clusters = %lu\n",
 fs.fs_type, (DWORD)fs.csize * 512,
 (DWORD)fs.n_fatent - 2);
 
 
 
 
 /* Dumping Test */
 
 bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_16);   // 15 Mhz Clock
 
 FILE *Dump;
 res = pf_open("BadApple.bin");
 printf("res=%d\n",res);
 printf("Starting dump... \n");
 printf("Le fichier à une taille de %ld",fs.fsize);
 // res = pf_lseek(p1); // set pointer to offset 0
 Rom_Data =(char*)malloc(fs.fsize); // Malloc buffer with file size
 j=0;


 while ( j < fs.fsize)
 {
   res=pf_read(Rom_Data,fs.fsize,&w); // read the file and send it to Rom_Data buffer
   j += w;
 }
 printf("\n%lu bytes read", j);
 
 //printf("res=%d\n",res);
 Dump=fopen("dump.bin","wb");
 fwrite(Rom_Data,1,fs.fsize,Dump);
 
 
			  
			

   printf("\n\nTest completed with Sucess !!: .\n");	
   
   /* Delay for 2Mhz 8Mo  60s
    * Delay for 4Mhz 8Mo  39s
    * Delay for 8Mhz 8Mo  28s
    * Delay for 16Mhz 8Mo 23s
    * Delay for 16Mhz 4Mo 12s
    * */	
		
 
    
    return 0;
}
