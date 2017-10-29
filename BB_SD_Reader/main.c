// Example program for read a microSD card with Raspberry Pi in bitbang mode 
//
// Author: X-death
// Copyright (C) 2017 Ultimate-Consoles

//Pinout Raspberry > Micro_SD Card Reader

//Pin1 3v3 Power  > PIN 1  VCC Input Voltage
//Pin9 Ground       > PIN 10 Power GND 
//Pin16 BCM 23    > Micro SD CS
//Pin15 BCM 22    > Micro SD SCK
//Pin13 BCM 27    > Micro SD MOSI
//Pin11 BCM 17    > Micro SD MISO


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#include <string.h>

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





//////////////////////////////////Custom SD bitbang code start here ////////////////////////////////



int main(void)
{   
    printf("=- Raspberry SD_Reader-=\n\n");
 
setup_io();
init_io();
//scanf("%d");


//xmit_mmc(0x5F);
printf("IO Init OK ...\n");

 
// PFatFs variable
	 
	BYTE res; // byte for Error code
	BYTE buff[64];
	FATFS fs;			//File system object
	int retry=0; 
	
	// var for directory listing
	
	char *ptr;
	UINT i,s1,s2;
	long p1;
	DIR dir;			/* Directory object */
	FILINFO fno;		/* File information */
	
		// Var for dump
//	long p1;
	long p2;
	long j;
	UINT w;
	char *Rom_Data;	



// - Initialize SD Card 
 
 printf("Intialize SD Card ...\n");
res = disk_initialize();
 while ( res!=0 && retry!=5)
 {
                printf("Failed SD init will retry 5 times ...\n");
                retry=retry+1;
                res = disk_initialize();
                if (retry == 5){ die(res);}
}
 printf("SD initialized sucessfully !\n\n");
 
 // - Mount a volume and print some info -
  
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
 
 
  
 // printf("\nDirectory list is : \n");
 
 
 /*
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
			  
	/		  
			  
 
 /* Dumping Test */
 
 
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
 
  printf("\n% Sucess !", j);
			  
			  
			  

    return 0;
}

// Speed is 
//120s 8Mo  60s 4Mo  60s 4096 Ko  66Ko/s
