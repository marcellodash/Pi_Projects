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

void die (		/* Stop with dying message */
	FRESULT rc	/* FatFs return value */
)
{
	printf("Failed with rc=%u.\n", rc);
	for (;;) ;
}


// Main Program

int main(void)
{
	
	// PFatFs variable
	
	BYTE res; // byte for Error code
	UINT br, i;
	BYTE buff[64];
	FATFS fs;			/* File system object */

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
 
 /* - Read message.txt file */
 
 	printf("\nOpen a test file (message.txt).\n");
	res = pf_open("message.txt");
	printf("res=%d\n",res);	
	
	 /* - Print the content */
	
		printf("\nFile content is : .\n");
	for (;;) {
		res = pf_read(buff, sizeof(buff), &br);	/* Read a chunk of file */
		if (res || !br) break;			/* Error or end of file */
		for (i = 0; i < br; i++)		/* Type the data */
			printf("%c",buff[i]);
	}

   printf("\n\nTest completed with Sucess !!: .\n");		
		
 
    
    return 0;
}
