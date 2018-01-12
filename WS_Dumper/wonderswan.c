/**
 * \file wonderswan.c
 * \brief Function for R/W Wonderswan Cartridge
 * \author X-death
 * \date  16/09/2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include "pinout.h"


void Clean_Data(void)
{
    pinMode (D0,OUTPUT),pinMode (D1,OUTPUT),pinMode (D2,OUTPUT);
    pinMode (D3,OUTPUT),pinMode (D4,OUTPUT),pinMode (D5,OUTPUT);
    pinMode (D6,OUTPUT),pinMode (D7,OUTPUT);
    pinMode (D8,OUTPUT),pinMode (D9,OUTPUT),pinMode (D10,OUTPUT);
    pinMode (D11,OUTPUT),pinMode (D12,OUTPUT),pinMode (D13,OUTPUT);
    pinMode (D14,OUTPUT),pinMode (D15,OUTPUT);
    digitalWrite (D0, 0),digitalWrite (D1, 0),digitalWrite (D2, 0);
    digitalWrite (D3, 0),digitalWrite (D4, 0),digitalWrite (D5, 0);
    digitalWrite (D6, 0),digitalWrite (D7, 0);
    digitalWrite (D8, 0),digitalWrite (D9, 0),digitalWrite (D10, 0);
    digitalWrite (D11, 0),digitalWrite (D12, 0),digitalWrite (D13, 0);
    digitalWrite (D14, 0),digitalWrite (D15, 0);
}

void Init_Memory(void)
{
    //Setup Pins value

    pinMode (CLK1,OUTPUT); // CPLD MUX1
    pinMode (CLK2,OUTPUT); // CPLD MUX2
    pinMode (CLK3,OUTPUT); // CPLD MUX3
    pinMode (RST,INPUT);   // CPLD RESET for MUX

    pinMode (OE,OUTPUT);  // OE for Cartridge
    pinMode (WE,OUTPUT);  // WE for Cartridge
    pinMode (RESET,OUTPUT); // RESET for Cartridge

    pinMode (SEL,OUTPUT);    // Cart Select PIN
    pinMode (CLK,OUTPUT);    // Cartridge Main Clock (384 Khz for real swan)
    pinMode (MIO,OUTPUT);    // Request from CPU to access IO

    pinMode (MBC,INPUT);     // MBC Serial Link from Bandai to CPU
    pinMode (IRQ,INPUT);     // ??

    digitalWrite (OE, 1);
    digitalWrite (WE, 1);
    digitalWrite (RESET,1);  // Disable ROM & SRAM Access
    digitalWrite (SEL, 1);
    digitalWrite (CLK, 1);
    digitalWrite (MIO, 1);

    digitalWrite (CLK1, 0);
    delayMicroseconds(2);
    digitalWrite (CLK1, 1);
    digitalWrite (CLK2, 0);
    delayMicroseconds(2);
    digitalWrite (CLK2, 1);
    digitalWrite (CLK3, 0);
    delayMicroseconds(2);
    digitalWrite (CLK3, 1);
}

void SetData_Input(void)
{

    pinMode (D0,INPUT),pinMode (D1,INPUT),pinMode (D2,INPUT);
    pinMode (D3,INPUT),pinMode (D4,INPUT),pinMode (D5,INPUT);
    pinMode (D6,INPUT),pinMode (D7,INPUT);
    pinMode (D8,INPUT),pinMode (D9,INPUT),pinMode (D10,INPUT);
    pinMode (D11,INPUT),pinMode (D12,INPUT),pinMode (D13,INPUT);
    pinMode (D14,INPUT),pinMode (D15,INPUT);
}

void SetData_OUTPUT(void)
{
    pinMode (D0,OUTPUT),pinMode (D1,OUTPUT),pinMode (D2,OUTPUT);
    pinMode (D3,OUTPUT),pinMode (D4,OUTPUT),pinMode (D5,OUTPUT);
    pinMode (D6,OUTPUT),pinMode (D7,OUTPUT);
    pinMode (D8,OUTPUT),pinMode (D9,OUTPUT),pinMode (D10,OUTPUT);
    pinMode (D11,OUTPUT),pinMode (D12,OUTPUT),pinMode (D13,OUTPUT);
    pinMode (D14,OUTPUT),pinMode (D15,OUTPUT);
}

void SetFlashOE(unsigned char state)
{
    pinMode (OE,OUTPUT);
    digitalWrite (OE,state);
}

void SetFlashWE(unsigned char state)
{
    pinMode (WE,OUTPUT);
    digitalWrite (WE,state);
}


void DirectWrite8(unsigned char val)
{
    digitalWrite (D0, (val & 0x01)) ; //D0
    digitalWrite (D1, (val & 0x02)) ; //D1
    digitalWrite (D2, (val & 0x04)) ; //D2
    digitalWrite (D3, (val & 0x08)) ; //D3
    digitalWrite (D4, (val & 0x10)) ; //D4
    digitalWrite (D5, (val & 0x20)) ; //D5
    digitalWrite (D6, (val & 0x40)) ; //D6
    digitalWrite (D7, (val & 0x80)) ; //D7
}

void DirectWrite16(unsigned short val)
{
    digitalWrite (D0, (val & 0x01)) ; //D0
    digitalWrite (D1, (val & 0x02)) ; //D1
    digitalWrite (D2, (val & 0x04)) ; //D2
    digitalWrite (D3, (val & 0x08)) ; //D3
    digitalWrite (D4, (val & 0x10)) ; //D4
    digitalWrite (D5, (val & 0x20)) ; //D5
    digitalWrite (D6, (val & 0x40)) ; //D6
    digitalWrite (D7, (val & 0x80)) ; //D7

    digitalWrite (D8, (val & 0x100)) ; //D8
    digitalWrite (D9, (val & 0x200)) ; //D9
    digitalWrite (D10, (val & 0x400)) ; //D10
    digitalWrite (D11, (val & 0x800)) ; //D11
    digitalWrite (D12, (val & 0x1000)) ; //D12
    digitalWrite (D13, (val & 0x2000)) ; //D13
    digitalWrite (D14, (val & 0x4000)) ; //D14
    digitalWrite (D15, (val & 0x8000)) ; //D15
}

void SetAddress (unsigned long address)
{
    SetData_OUTPUT();
    pinMode (RST,OUTPUT);
    digitalWrite (RST, 1);
    DirectWrite8(address & 0xff);
    digitalWrite (CLK1, 0);
    digitalWrite (CLK1, 1);
    DirectWrite8((address >> 8) & 0xff);
    digitalWrite (CLK2, 0);
    digitalWrite (CLK2, 1);
    DirectWrite8((address >> 16) & 0xff);
    digitalWrite (CLK3, 0);
    digitalWrite (CLK3, 1);
    SetData_Input();
}

void SetFlashCE(unsigned char state)
{
    if (state==0)
    {
        SetAddress(0x20000);
    }
    else
    {
        SetAddress(0x10000);
    }
}

void SetSramCE(unsigned char state)
{
    if (state==0)
    {
        SetAddress(0x10000);
    }
    else
    {
        SetAddress(1);
    }
}

unsigned char DirectRead8()
{
    unsigned char result=0;
    if (digitalRead(D0)) result |= 1;
    if (digitalRead(D1)) result |= 2;
    if (digitalRead(D2)) result |= 4;
    if (digitalRead(D3)) result |= 8;
    if (digitalRead(D4)) result |= 16;
    if (digitalRead(D5)) result |= 32;
    if (digitalRead(D6)) result |= 64;
    if (digitalRead(D7)) result |= 128;
    return result;
}

unsigned int DirectRead16()
{
// Wonderswan is a Little Endian machine so we must invert byte order

    unsigned int result=0;
    if (digitalRead(D8)) result |= 1;
    if (digitalRead(D9)) result |= 2;
    if (digitalRead(D10)) result |= 4;
    if (digitalRead(D11)) result |= 8;
    if (digitalRead(D12)) result |= 16;
    if (digitalRead(D13)) result |= 32;
    if (digitalRead(D14)) result |= 64;
    if (digitalRead(D15)) result |= 128;

    if (digitalRead(D0))  result |= 256;
    if (digitalRead(D1))  result |= 512;
    if (digitalRead(D2)) result |= 1024;
    if (digitalRead(D3)) result |= 2048;
    if (digitalRead(D4)) result |= 4096;
    if (digitalRead(D5)) result |= 8192;
    if (digitalRead(D6)) result |= 16384;
    if (digitalRead(D7)) result |= 32768;
    return result;
}

void WriteRegisters(unsigned char Registers,unsigned char value)
{
    digitalWrite (MIO,0);
    SetFlashOE(1);
    SetFlashWE(1);
    SetAddress(0xC0000+Registers);
    SetFlashOE(0);
    SetFlashWE(0);
    SetData_OUTPUT();
    DirectWrite8(value);
    SetFlashOE(1);
    SetFlashWE(1);
    digitalWrite (MIO,1);
}

unsigned char ReadRegisters(unsigned char Registers)
{
    digitalWrite (MIO,0);
    SetFlashOE(1);
    SetFlashWE(1);
    SetAddress(0xC0000+Registers);
    SetFlashOE(0);
    SetData_Input();
    char byte = DirectRead8();
    SetFlashOE(1);
    SetFlashWE(1);
    digitalWrite (MIO,1);
    return byte;
}
/*
int UnlockMBC(void)
{
digitalWrite (RESET,0);
delayMicroseconds(1);
digitalWrite (RESET,1);
digitalWrite (CLK,0);
delayMicroseconds(1);
SetAddress(0xA0005); // A5
delayMicroseconds(1);
digitalWrite (CLK,1);
return (digitalRead(MBC));
}*/

int ReadSram(int address)
{
    SetSramCE(1);
    SetFlashOE(1);
    SetAddress(0x10000+address);
    delayMicroseconds(1);
    SetFlashOE(0);
    delayMicroseconds(1);
    SetData_Input();
    delayMicroseconds(1);
    unsigned char result = DirectRead8();
    delayMicroseconds(1);
    SetSramCE(1);
    delayMicroseconds(1);
    SetFlashOE(1);
    return result;
}

unsigned int ReadFlash16(int address)
{
    SetFlashCE(1);
    SetFlashOE(1);
    SetAddress(0x20000 + address);
    delayMicroseconds(1);
    SetFlashOE(0);
    delayMicroseconds(1);
    SetData_Input(); // rasp data en entrée
    delayMicroseconds(1);
    unsigned int result = DirectRead16();
    SetFlashCE(1);
    delayMicroseconds(1);
    SetFlashOE(1);
    return result;
}

void DumpSram(long NumberOfByte)
{
    char *Buffer;
    FILE *dump;
    char OctetActuel=0;
    int i=0;
    unsigned int start, finish;
    printf("Dumping Sram...\n");
    start = micros ();
    Buffer = (char*)malloc(NumberOfByte);
    for (i=0; i< NumberOfByte; i++)
    {
        OctetActuel = ReadSram(0x0000+i);
        Buffer[i]= OctetActuel;
    }
    dump=fopen("dump.sav","wb");
    fwrite(Buffer,1,NumberOfByte,dump);
    finish = micros();
    printf ("Temps : %d Ms\n", (finish -start)/1000);
}

void DumpRom(long NumberOfByte)
{
    char *Buffer;
    FILE *dump;
    short WordActuel=0;
    int i=0;
    int k=0;
    int bank=0;
    unsigned int start, finish;
    printf("Dumping Rom...\n");
    start = micros ();
    Buffer = (char*)malloc(NumberOfByte);
    for (i=0; i< NumberOfByte; i=i+2)
    {
        WordActuel= ReadFlash16(k);
        k=k+2;
        Buffer[i+1] = (WordActuel & 0xFF);
        Buffer[i]= (WordActuel & 0xFF00) >> 8;
        //j=j+2;
        if (k==65536){bank++;WriteRegisters(2,bank);k=0;}
    }
    dump=fopen("dump.ws","wb");
    fwrite(Buffer,1,NumberOfByte,dump);
    finish = micros();
    printf("Wonderswan ROM dumped successfully !\n");
    printf ("Time : %d Ms\n", (finish -start)/1000);
}



