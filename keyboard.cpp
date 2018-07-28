#include "keyboard.h"

void printf(char*);

KeyboardDriver::KeyboardDriver(InterruptManager* manager)
  : InterruptHandler(0x21, manager),
    dataport(0x60),
    commandport(0x64)
{
  printf("In constructor of KeyboardDriver\n");

  printf("Exiting constructor of KeyboardDriver\n");
}

KeyboardDriver::~KeyboardDriver()
{
}

void KeyboardDriver::Activate()
{
  printf("Set up KeyboardDriver\n");

  while(commandport.Read() & 0x1)
  {
    dataport.Read();
  }
  commandport.Write(0xAE);  // activate interrupts
  commandport.Write(0x20);  // get current state
  uint8_t status = (dataport.Read() | 1) & ~0x10;
  commandport.Write(0x60);  // set state
  dataport.Write(status);   // Write state
  dataport.Write(0xF4);
}

uint32_t KeyboardDriver::HandleInterrupt(uint32_t esp)
{
  uint8_t key = dataport.Read();


  if(key < 0x80)
  {
    switch (key) {
      case 0xFA:
      case 0x45:
      case 0xC5:
        break;
      case 0x1E:
        printf("a");
        break;
      default:
        char* foo = "KEYBOARD 0X00 ";
        char* hex = "0123456789ABCDEF";
        foo[11] = hex[(key >> 4) & 0x0F];
        foo[12] = hex[(key) & 0x0F];
        printf(foo);
        break;
    }
  }

  return esp;
}
