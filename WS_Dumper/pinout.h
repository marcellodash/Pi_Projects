//First we need to Map CPLD pin to the Raspberry  GPIO Port with the WiringPi value
// defined as : PinName WiringPivalue BCM value
// docs pi.gadgetoid.com/pinout/

// Wonderswan Part

#define D0   07 //BCM 04 OK
#define D1   00 //BCM 17 OK
#define D2   01 //BCM 18 OK
#define D3   02 //BCM 27 OK
#define D4   03 //BCM 22 OK
#define D5   04 //BCM 23 OK
#define D6   05 //BCM 24 OK
#define D7   06 //BCM 25 OK

#define D8   12 //BCM 10 OK
#define D9   13 //BCM 09 OK
#define D10  10 //BCM 08 OK
#define D11  11 //BCM 07 OK
#define D12  31 //BCM 01 OK
#define D13  30 //BCM 00 OK
#define D14  21 //BCM 05 OK
#define D15  22 //BCM 06 OK

#define OE   16 //BCM 15
#define WE 8 //BCM 02

#define RESET 15 //BCM 14
#define SEL   9 //BCM 03
#define CLK   14 //BCM 11
#define MBC   26 //BCM 12
#define MIO   27 //BCM 16
#define IRQ   28 //BCM 20

// CPLD Part ( for Addr Mux)

#define CLK1  23 //BCM 13
#define CLK2  24 //BCM 19
#define CLK3  25 //BCM 26
#define RST   29 //BCM 21

