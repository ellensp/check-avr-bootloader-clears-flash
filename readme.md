Tests that the end of flash ram is cleared by the bootloader.

AVR Fuse reading based on code found at https://www.avrfreaks.net/forum/read-fuse-bits-runtime by David Prentice


Example output consists of:

information:

"lockb = 0xFF"
"ext fuse = 0xFD"
"high fuse = 0xD8"
"low fuse = 0xFF"
"BOOT loader start : 0x3E000"

Now the code check that all high bytes are set to FF begins:

"Checking 0x3C000 through 0x3E000 has been cleared."

If the bytes are cleared you get a positive result

"This bootloader is good!"

If the bytes are not cleared you get, the first address that failed and its contents
eg  "Addr:3C000[1]"
    "This bootloader is faulty, it does not clear the full usable flash area"

