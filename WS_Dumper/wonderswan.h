/**
 * \file wonderswan.h
 * \brief Function for R/W Wonderswan Cartridge
 * \author X-death
 * \date  16/09/2017
 */

/**
 * Initialize all Data Lines to 0
 */

 void Clean_Data(void);

/**
 * Setup all I/O direction
 */

 void Init_Memory(void);

 /**
 * Turn Data Pin to INPUT
 */

 void SetData_Input(void);

/**
 * Turn Data Pin to OUTPUT
 */

 void SetData_OUTPUT(void);

/**
 * Control state of /CE Pin for FlashRom
 */

void SetFlashCE(unsigned char state);

/**
 * Control state of /OE Pin
 */

void SetFlashOE(unsigned char state);

/**
 * Control state of /WE Pin
 */

void SetFlashWE(unsigned char state);

/**
 * Control state of /CE Pin for Sram
 */

void SetSramCE(unsigned char state);

 /**
 * Set an Address to the BUS
 */

void SetAddress (unsigned long address);

 /**
 * Send a byte to the 8 bits BUS
 */

 void DirectWrite8(unsigned char val);

 /**
 * Send a word to the 16 bits BUS
 */

 void DirectWrite16(unsigned short val);

 /**
 * Read a byte from the 8 bits BUS
 */

 unsigned char DirectRead8();

/**
 * Read a word from the 16 bits BUS
*/

 unsigned int DirectRead16();


void WriteRegisters(unsigned char Registers,unsigned char value);
unsigned char ReadRegisters(unsigned char Registers);
unsigned char UnlockMBC(void);

 /**
 * Read and return byte from the sram at specified address
 */

 unsigned int ReadSram(int adress);

  /**
 * Read and return word from the FlashRom at specified address
 */

 unsigned int ReadFlash16(int adress);

   /**
 * Read in 8 bits specified bytes from the SRAM and write the dump.sav file
 */

 void DumpSram(long NumberOfByte);

    /**
 * Read in 16 bits specified bytes from the ROM and write the dump.bin file
 */

 void DumpRom(long NumberOfByte);





