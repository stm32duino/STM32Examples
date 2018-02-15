/*
  This sketch employs mallinfo() to retrieve memory allocation
  statistics before and after allocating and freeing blocks of
  memory. The statistics are displayed on Serial.

  Creation 14 Feb 2018
  by Frederic Pillon

  This example code is in the public domain.

  Based on example from:
  http://man7.org/linux/man-pages/man3/mallinfo.3.html
*/
#include <malloc.h>

extern "C" char *sbrk(int i);
extern char _end; /* Defined by the linker */

// Get from memory area defined in linker script
// Example for STM32F103C8
// RAM (xrw)      : ORIGIN = 0x20000000, LENGTH = 20K
char *ramstart=(char *)0x20000000;
char *ramend=(char *)0x20005000;

#define NUM_BLOCKS 100
#define BLOCK_SIZE 4

void display_mallinfo(void)
{
  char *heapend=(char*)sbrk(0);
  register char * stack_ptr asm("sp");
  struct mallinfo mi=mallinfo();

  Serial.print("Total non-mmapped bytes (arena):       ");
  Serial.println(mi.arena);
  Serial.print("# of free chunks (ordblks):            ");
  Serial.println(mi.ordblks);
  Serial.print("# of free fastbin blocks (smblks):     ");
  Serial.println(mi.smblks);
  Serial.print("# of mapped regions (hblks):           ");
  Serial.println(mi.hblks);
  Serial.print("Bytes in mapped regions (hblkhd):      ");
  Serial.println(mi.hblkhd);
  Serial.print("Max. total allocated space (usmblks):  ");
  Serial.println(mi.usmblks);
  Serial.print("Free bytes held in fastbins (fsmblks): ");
  Serial.println(mi.fsmblks);
  Serial.print("Total allocated space (uordblks):      ");
  Serial.println(mi.uordblks);
  Serial.print("Total free space (fordblks):           ");
  Serial.println(mi.fordblks);
  Serial.print("Topmost releasable block (keepcost):   ");
  Serial.println(mi.keepcost);

  Serial.print("RAM Start 0x");
  Serial.println((unsigned long)ramstart, HEX);
  Serial.print("Data/Bss end  0x");
  Serial.println((unsigned long)&_end, HEX);
  Serial.print("Heap End  0x");
  Serial.println((unsigned long)heapend, HEX);
  Serial.print("Stack Ptr  0x");
  Serial.println((unsigned long)stack_ptr, HEX);
  Serial.print("RAM End  0x");
  Serial.println((unsigned long)ramend, HEX);

  Serial.print("Heap RAM Used: ");
  Serial.println(mi.uordblks);
  Serial.print("Program RAM Used ");
  Serial.println(&_end - ramstart);
  Serial.print("Stack RAM Used ");
  Serial.println(ramend - stack_ptr);

  Serial.print("Estimated Free RAM: ");
  Serial.println(stack_ptr - heapend + mi.fordblks);
}

void setup() {
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  delay(1000);
}

void loop() {
  unsigned int n=0;
  char *alloc[NUM_BLOCKS];

  Serial.println("============== Before allocating blocks ==============");
  display_mallinfo();

  for (n = 0; n < NUM_BLOCKS; n++) {
    alloc[n] = (char*)malloc(BLOCK_SIZE);
    if (alloc[n] == NULL) {
      Serial.print("Failed to allocate blocks ");
      Serial.println(n);
      while(1);
    }
  }

  Serial.println("============== After allocating blocks ==============");
  display_mallinfo();

  for (n = 0; n < NUM_BLOCKS; n++)
    free(alloc[n]);

  Serial.println("============== After freeing blocks ==============");
  display_mallinfo();
  while(1);
}


