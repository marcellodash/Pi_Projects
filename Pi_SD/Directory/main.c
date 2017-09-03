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
#include <string.h>
#include <bcm2835.h> // Special GPIO Library for Raspberry Pi

// SD Driver Part

#include "diskio.h"
#include "pff.h"

static int j=0; // nombre de fichier sucessif à lire

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
	BYTE buff[64];
	FATFS fs;			/* File system object */
	
	// var for directory listing
	
	char *ptr;
	UINT i,s1,s2;
	long p1;
	DIR dir;			/* Directory object */
	FILINFO fno;		/* File information */
	

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
 
 bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_128);   // 2 Mhz Clock
 
 
 /* fl [<path>] - Directory listing */
 
 printf("\nDirectory list is : \n");
 
 
 
 res = pf_opendir(&dir, ptr);
 s1=0;

 
 //res = pf_readdir(&dir, &fno);
		//		printf("%d %d  %s\n", fno.fattrib, fno.fsize, fno.fname);
		//	s1++;
		i=0;
	
		for ( ;;){
				 res = pf_readdir(&dir, &fno);
				 if (!fno.fname[0] | j==16) break; // break if more 16 files
				 if (fno.fattrib & AM_DIR){
				 printf("  <DIR>    %s\n", fno.fname);j++;}
				 else{
			      printf("  <FILE>   %s\n", fno.fname);j++;}	      
			  }
			  
			  
			

   printf("\n\nTest completed with Sucess !!: .\n");		
		
 
    
    return 0;
}
