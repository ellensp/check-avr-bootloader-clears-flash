// Tests that the end of flash ram is cleared by the bootloader.

// AVR Fuse reading based on code found at https://www.avrfreaks.net/forum/read-fuse-bits-runtime by David Prentice


// The output

// lockb = 0xFF                information the lock bits in hex
// ext fuse = 0xFD             information the ext fuse bits in hex
// high fuse = 0xD8            information the high fuse bits in hex
// low fuse = 0xFF             information the low fuse bits in hex
// BOOT loader start : 0x3E000   The boot loader start address is hex
// Checking 0x3C000 through 0x3E000 has been cleared.   Infomation that test is inderway

// If the bytes are cleared you get a positive result  "This bootloader is good!"
// If the bytes are not cleared you get, the first affress that failed and its contents in and the following message.
// eg  Addr:3C000[1]
// "This bootloader is faulty, it does not clear the full usable flash area"

#include <Arduino.h>
#include <avr/boot.h>

#define BAUDRATE 9600
uint32_t bootloaderstart = 0;

void print_val(char *msg, uint8_t val)
{
  Serial.print(msg);
  Serial.println(val, HEX);
}

void setup(void)
{
  uint8_t hfuse;

  Serial.begin(BAUDRATE);
  while (!Serial) ;
  print_val((char*)"lockb = 0x", boot_lock_fuse_bits_get(1));
  print_val((char*)"ext fuse = 0x", boot_lock_fuse_bits_get(2));
  hfuse = boot_lock_fuse_bits_get(3);
  print_val((char*)"high fuse = 0x", hfuse);
  print_val((char*)"low fuse = 0x", boot_lock_fuse_bits_get(0));
  //#define GETSIGNATURE
  #if defined(GETSIGNATURE)
    #if defined(SIGRD) || defined(RSIG)
      Serial.print("Signature : ");
      for (uint8_t i = 0; i < 5; i += 2) {
        Serial.print(" 0x");
        Serial.print(boot_signature_byte_get(i), HEX);
      }
      Serial.println();
    #endif
  #endif
  //#define GETSERIALNUMBER
  #if defined(GETSERIALNUMBER)
    Serial.print("Serial Number : ");
    for (uint8_t i = 14; i < 24; i += 1) {
      Serial.print(" 0x");
      Serial.print(boot_signature_byte_get(i), HEX);
    }
    Serial.println();
  #endif    

  Serial.print("BOOT loader start : 0x");
  switch(hfuse&6) {
    case 0:
      bootloaderstart = 0x3E000;
      break;
    case 2:
      bootloaderstart = 0x3F000;
      break;
    case 4:
      bootloaderstart = 0x3F800;
      break;
    case 6:
      bootloaderstart = 0x3FC00;
      break;
    default: 
      Serial.print("This is Impossable");
  }

  Serial.println(bootloaderstart, HEX);
  Serial.print("Checking 0x3C000 through 0x"); Serial.print(bootloaderstart, HEX); Serial.println(" has been cleared.");

  bool failedtoclear = false;
	for( uint32_t addr = 0x3C000; addr < bootloaderstart; addr++) {
		if ( uint8_t byte_u8 = pgm_read_byte_far( addr ) != 0xFF ) {
      failedtoclear = true;
      Serial.print("Addr:"); 
      Serial.print(addr,HEX);
      Serial.print("[");
      Serial.print(byte_u8,HEX);
      Serial.println("]");
      break;
    }
	}

  if (failedtoclear)
    Serial.println("This bootloader is faulty, it does not clear the full usable flash area"); 
  else
    Serial.println("This bootloader is good!");
}

void loop(void)
{
}
