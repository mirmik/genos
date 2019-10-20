#include <hal/board.h>
#include <OneWire.h>
#include <Arduino.h>

uint8_t findDevices(int pin)
{
  drivers::OneWire ow(getPin(pin));

  uint8_t address[8];
  uint8_t count = 0;


  if (ow.search(address))
  {
    dpr("\nuint8_t pin");
    dpr(pin);
    dprln("[][8] = {");
    do {
      count++;
      dprln("  {");
      for (uint8_t i = 0; i < 8; i++)
      {
        dpr("0x");
        if (address[i] < 0x10) dpr("0");
        dprhex(address[i]);
        if (i < 7) dpr(", ");
      }
      dprln("  },");
    } while (ow.search(address));

    dprln("};");
    dpr("// nr devices found: ");
    dprln(count);
  }

  return count;
}

void setup()
{
	board_init();

  dprln("//\n// Start oneWireSearch.ino \n//");

  for (uint8_t pin = 2; pin < 13; pin++)
  {
    findDevices(pin);
  }
  dprln("\n//\n// End oneWireSearch.ino \n//");
}

void loop()
{
}

int main() 
{
	setup();
	while(1) loop();	
}