#include <stdio.h>
#include <stdlib.h>

#include <wiringPi.h>

#include "pinout.h"
#include "wonderswan.h"
#include "Swan_chksm.h"

//Variables static

/*
static int i=0;
static int interrupt=0;*/
/*


PI_THREAD (myThread)
{

    int i=0;
    int j=0;
    for (j=0; j<9; j++)
    {


        digitalWrite (CLK, 0);
        for (i=0; i< 121; i++)
        {
            asm("nop");
        }
        digitalWrite (CLK, 1);
        for (i=0; i< 121; i++)
        {
            asm("nop");
        }


    }
}

*/
int main (void)
{

long Game_size=0;
int choixMenu=0;

    printf("-------------------------\n");
    printf("-= WonderSwan Dumper =--=\n");
    printf("-------------------------\n");
    printf("2018/01 X-DEATH \n");
    printf("RASPBERRY PI ver. 1.0\n");
    printf("www.ultimate-consoles.fr \n\n");

    wiringPiSetup () ;

    Clean_Data();
    Init_Memory();

    printf ("GPIO Init Sucessfully \n");

// printf ("Generating 380 Khz Clock...\n");

//   printf("Try to Unlock MBC...!\n");

 /*
     printf ("Generating 384 Khz Clock...\n");

     x = piThreadCreate(myThread);
     if ( x!=0)
     {
         printf ("Thread Failed \n");
     }
     printf("Clock generated sucessfully !\n");

     x = piThreadCreate(myThread);
      if ( x!=0)
      {  printf ("Thread Failed \n");  }*/
//  printf("Clock generated sucessfully !\n");

// printf ("Try to read a word ... \n");

    digitalWrite (RESET,1); // Disable Reset for enable ROM/SRAM Read
    digitalWrite (SEL, 0);
    digitalWrite (MIO,0); // When processor want to access IO

    // READ REGISTER//

    printf("Reading Bandai Register ...\n");
    char byte = ReadRegisters(0x00);
    printf("Register C0 is 0x%02x\n",byte);
    byte = ReadRegisters(1);
    printf("Register C1 is 0x%02x\n",byte);
    byte = ReadRegisters(2);
    printf("Register C2 is 0x%02x\n",byte);
    byte = ReadRegisters(3);
    printf("Register C3 is 0x%02x\n",byte);

// Reading Header

    unsigned char Swan_Header[10];
    unsigned short word=0;
    unsigned int i=0;

    WriteRegisters(0,0x0F); // C2
    WriteRegisters(1,0xFF);
    WriteRegisters(2,0xFF);
    WriteRegisters(3,0xFF);

    /*SetData_OUTPUT();
    DirectWrite16(0xFFFF);*/

    for (i=0; i< 10; i=i+2)
    {
        word = ReadFlash16(0xFFF6+i);
        Swan_Header[i+1] = (word & 0xFF);
        Swan_Header[i]= (word & 0xFF00) >> 8;
    }
    printf("\nCartridge Header is :");
    for (i=0; i< 10; i++)
    {
        printf("%02x ",Swan_Header[i]);
    }
    printf("\nDeveloper ID %02x",Swan_Header[0]);

    if (Swan_Header[1] != 0){ printf("\nGame is for WS Color/Crystal Only!");}
    else{printf("\nGame compatible with all Wonderswan");}

    if (Swan_Header[4] ==0x06){
    printf("\nGame size is 32 Mbit");Game_size=1024*4096;}
    else if (Swan_Header[4] ==0x04){
    printf("\nGame size is 16 Mbit");Game_size=1024*2048;}
    else if (Swan_Header[4] ==0x03){
    printf("\nGame size is 8 Mbit");Game_size=1024*1024;}
    else if (Swan_Header[4] ==0x02){
    printf("\nGame size is 4 Mbit");Game_size=1024*512;}

    if (Swan_Header[5] ==0x00){
    printf("\nGame don't have SRAM/EEPROM\n");}
    else {printf("\nGame have backup Memory type : ");
         if (Swan_Header[5] ==0x10) {printf("EEPROM 1K");}
          else if (Swan_Header[5] ==0x20) {printf("EEPROM 8K");}
          else if (Swan_Header[5] ==0x50) {printf("EEPROM 16K");}
          else if (Swan_Header[5] ==0x01) {printf("SRAM 64K");}
          else if (Swan_Header[5] ==0x02) {printf("SRAM 256K");}
          else if (Swan_Header[5] ==0x03) {printf("SRAM 1M");}
          else if (Swan_Header[5] ==0x04) {printf("SRAM 2M");}
    }

    if (Swan_Header[7] ==0x00){
    printf("Game don't have RTC");}
    else {printf("\nReal Time Clock supported ! ");}

    unsigned short Swan_Checksum=0;
    Swan_Checksum = ReadFlash16(0xFFFE);
    printf("\nChecksum game is %d\n",Swan_Checksum);
    printf("Reading database ...\n");
    unsigned short j=0;
    i=0;
    while ( j != Swan_Checksum || i==189 )
    {
       j=WS_Checksum[i];
       i++;
    }
    if (i != 189)
    {
    printf("Found game with same Checksum in database :\n");
    printf("%s\n",WS_Names[i-1]);
    }
    else{
    printf("No game found with this checksum in database\n");}


    printf("\n--- MENU ---\n");
    printf("1) Dump WS ROM \n");
    printf("2) Dump WS Save \n");
    printf("3) Write WS Save \n");
    printf("4) Erase WS Save \n");
    printf("5) Write WS Flash (16bits) \n");
    printf("6) Erase WS Flash \n");
    printf("10) Exit \n\n");
    scanf("%d",&choixMenu);

    switch(choixMenu)
    {
      case 1: // DUMP WS ROM
                choixMenu=0;
                WriteRegisters(2,0x00); // Switch to first bank
                DumpRom(Game_size);
                break;



    }







    //word = ReadFlash16(0x4B0);
    //printf("word is %02x\n",word);
    //  DumpRom(1500);



    //DumpRom(65536);

    //Dump SRAM

    // digitalWrite (MIO,0); // When processor want to access IO
    //  DumpSram(32768);



    // Unclock MBC

    /*  printf("Try to Unlock MBC...!\n");

      digitalWrite (MIO,1);
      digitalWrite (RESET,1);
      SetFlashOE(1);
      SetFlashWE(1);
      SetAddress(0xA0005); // A5
      SetData_OUTPUT();
      DirectWrite8(0xFF);*/

    /*    x = piThreadCreate(myThread);
           if ( x!=0)
           {  printf ("Thread Failed \n");  }
            printf("Clock generated sucessfully !\n");



    DumpSram(32768);*/

//SetAddress(0xFFFFF);

    /* unsigned int Word=0;
     unsigned char byte=0;

     Word=ReadFlash16(0x00);
    //SetData_Input;
    //Word=DirectRead16(0);
    //byte=DirectRead8(0);
     printf("Word is %02x\n",Word);
    //printf("byte is %02x\n",byte);*/




    return 0 ;
}
