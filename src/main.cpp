#include "Chip8Emulator.h"
#include <string>

int main(int argc, char* argv[])
{
   if (argc > 1)
   {
      std::string filePath(argv[1]);
      Chip8Emulator emu;
      if (emu.loadApplication(filePath))
      {
         emu.emulate();
      }
   }
	return 0;
}
