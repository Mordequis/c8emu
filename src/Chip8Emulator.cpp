#include "Chip8Emulator.h"
#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>

Chip8Emulator::Chip8Emulator() :
   theMemoryAddressRegisterI(0),
   theStackPointer(STACK_START_ADDRESS),
   theProgramCounter(PROGRAM_START_ADDRESS),
   theActualProgramSize(0)
{
   // Initialize everything to 0 to have a known starting point
   memset(theRegistersV, 0, NUM_V_REGISTERS * sizeof(uint8_t));
   memset(theMemory, 0, MEMORY_SIZE * sizeof(uint8_t));
}

Chip8Emulator::~Chip8Emulator() {
	// TODO Auto-generated destructor stub
}

bool Chip8Emulator::loadApplication(const std::string& filePath)
{
   bool success = false;
   std::ifstream appFile(filePath, std::ifstream::in | std::ifstream::binary);
   if (!appFile.good())
   {
      std::cout << "Failed to open " << filePath << std::endl;
   }
   else
   {
      appFile.read(reinterpret_cast<char*>(&(theMemory[PROGRAM_START_ADDRESS])),
                   MAX_PROGRAM_SIZE);
      if (appFile.bad())
      {
         std::cout << "Failed to read " << filePath << std::endl;
      }
      else if (appFile.eof()) // Read all that it could
      {
         success = true;
      }
      else //Check if the application is larger than the memory supported
      {
         std::cout << appFile.get() << std::endl; // Try to get one more character
         success = appFile.eof();
         if (!success)
         {
            std::cout << "Application does not fit in memory " << MAX_PROGRAM_SIZE << std::endl;
         }
      }

      if (success)
      {
         theActualProgramSize = appFile.gcount();
         std::cout << "Loaded " << theActualProgramSize << " bytes from " << filePath << std::endl;
      }
   }
   return success;
}

void Chip8Emulator::emulate()
{
   bool unsupported = false;
   std::cout << "ADDR\tOPCODE\tINST\tOPERANDS" << std::endl; /// @todo change to file output and format numbers better
   // While there is another instruction to execute
   while (theProgramCounter < (PROGRAM_START_ADDRESS + theActualProgramSize)) /// @todo worry about odd length
   {
      uint16_t instruction = (theMemory[theProgramCounter] << 8) |
                              theMemory[theProgramCounter + 1];
      std::cout << std::hex << theProgramCounter << '\t' << instruction << '\t';
      if (instruction == 0x00E0) // Clear the Screen
      {
         unsupported = true; /// @todo implement
      }
      else if (instruction == 0x00EE) // Return from subroutine
      {
         unsupported = true; /// @todo implement
      }
      else if ((instruction & 0xF000) == 0x0000) // 0NNN  RCA 1802 program
      {
         unsupported = true;
      }
      else if ((instruction & 0xF000) == 0x1000) // 1NNN  Jump to address NNN
      {
         theProgramCounter = instruction & 0x0FFF;
         std::cout << "jmp\t" << theProgramCounter << std::endl;
         continue;
      }
      else if ((instruction & 0xF000) == 0x2000) // 2NNN  Call subroutine at NNN
      {
         unsupported = true; /// @todo implement
      }
      else if ((instruction & 0xF000) == 0x3000) // 3XNN  Skip the next instruction if VX equals NN
      {
         unsupported = true; /// @todo implement
      }
      else if ((instruction & 0xF000) == 0x4000) // 4XNN  Skip the next instruction if VX doesn't equal NN
      {
         unsupported = true; /// @todo implement
      }
      else if ((instruction & 0xF00F) == 0x5000) // 5XY0  Skip the next instruction if VX equals VY
      {
         unsupported = true; /// @todo implement
      }
      else if ((instruction & 0xF000) == 0x6000) // 6XNN  Set VX to NN
      {
         uint8_t x = (instruction & 0x0F00) >> 8;
         uint8_t nn = instruction & 0x00FF;
         if (x < NUM_V_REGISTERS)
         {
            theRegistersV[x] = nn;
         }
         std::cout << "mov\tV" << static_cast<uint16_t>(x) << ", " << static_cast<uint16_t>(nn) << std::endl;
      }
      else if ((instruction & 0xF000) == 0x7000) // 7XNN  Add NN to VX
      {
         unsupported = true; /// @todo implement
      }
      else if ((instruction & 0xF00F) == 0x8000) // 8XY0  Set VX to the value of VY
      {
         uint8_t x = (instruction & 0x0F00) >> 8;
         uint8_t y = (instruction & 0x00F0) >> 4;
         if ((x < NUM_V_REGISTERS) && (y < NUM_V_REGISTERS))
         {
            theRegistersV[x] = theRegistersV[y];
         }
         std::cout << "mov\tV" << static_cast<uint16_t>(x) << ", V" << static_cast<uint16_t>(y) << std::endl;
      }
      else if ((instruction & 0xF00F) == 0x8001) // 8XY1  Set VX to VX or VY
      {
         unsupported = true; /// @todo implement
      }
      else if ((instruction & 0xF00F) == 0x8002) // 8XY2  Set VX to VX and VY
      {
         unsupported = true; /// @todo implement
      }
      else if ((instruction & 0xF00F) == 0x8003) // 8XY3  Set VX to VX xor VY
      {
         unsupported = true; /// @todo implement
      }
      else if ((instruction & 0xF00F) == 0x8004) // 8XY4  Add VY to VX. Set VF to 1 when there's a carry, and to 0 when there isn't
      {
         unsupported = true; /// @todo implement
      }
      else if ((instruction & 0xF00F) == 0x8005) // 8XY5  Subtract VY from VX. Set VF to 0 when there's a borrow, and 1 when there isn't
      {
         unsupported = true; /// @todo implement
      }
      else if ((instruction & 0xF00F) == 0x8006) // 8XY6  Shift VX right by one. Set VF to the value of the least significant bit of VX before the shift
      {
         unsupported = true; /// @todo implement
      }
      else if ((instruction & 0xF00F) == 0x8007) // 8XY7  Set VX to VY minus VX. Set VF to 0 when there's a borrow, and 1 when there isn't
      {
         unsupported = true; /// @todo implement
      }
      else if ((instruction & 0xF00F) == 0x800E) // 8XYE  Shift VX left by one. Set VF to the value of the most significant bit of VX before the shift
      {
         unsupported = true; /// @todo implement
      }
      else if ((instruction & 0xF00F) == 0x9000) // 9XY0  Skip the next instruction if VX doesn't equal VY
      {
         unsupported = true; /// @todo implement
      }
      else if ((instruction & 0xF000) == 0xA000) // ANNN  Set I to the address NNN
      {
         uint16_t nnn = instruction & 0x0FFF;
         theMemoryAddressRegisterI = nnn;
         std::cout << "mov\tI, " << nnn << std::endl;
      }
      else if ((instruction & 0xF000) == 0xB000) // BNNN  Jump to the address NNN plus V0
      {
         unsupported = true; /// @todo implement
      }
      else if ((instruction & 0xF000) == 0xC000) // CXNN  Set VX to the result of a bitwise and operation on a random number and NN
      {
         unsupported = true; /// @todo implement
      }
      else if ((instruction & 0xF000) == 0xD000) // DXYN  Sprites stored in memory at location in index register (I), 8bits wide. Wraps around the screen. If when drawn, clears a pixel, register VF is set to 1 otherwise it is zero. All drawing is XOR drawing (i.e. it toggles the screen pixels). Sprites are drawn starting at position VX, VY. N is the number of 8bit rows that need to be drawn. If N is greater than 1, second line continues at position VX, VY+1, and so on.
      {
         unsupported = true; /// @todo implement
      }
      else if ((instruction & 0xF0FF) == 0xE09E) // EX9E  Skip the next instruction if the key stored in VX is pressed
      {
         unsupported = true; /// @todo implement
      }
      else if ((instruction & 0xF0FF) == 0xE0A1) // EXA1  Skip the next instruction if the key stored in VX isn't pressed
      {
         unsupported = true; /// @todo implement
      }
      else if ((instruction & 0xF0FF) == 0xF007) // FX07  Set VX to the value of the delay timer
      {
         unsupported = true; /// @todo implement
      }
      else if ((instruction & 0xF0FF) == 0xF00A) // FX0A  A key press is awaited, and then stored in VX
      {
         unsupported = true; /// @todo implement
      }
      else if ((instruction & 0xF0FF) == 0xF015) // FX15  Set the delay timer to VX
      {
         unsupported = true; /// @todo implement
      }
      else if ((instruction & 0xF0FF) == 0xF018) // FX18  Set the sound timer to VX
      {
         unsupported = true; /// @todo implement
      }
      else if ((instruction & 0xF0FF) == 0xF01E) // FX1E  Add VX to I
      {
         unsupported = true; /// @todo implement
      }
      else if ((instruction & 0xF0FF) == 0xF029) // FX29  Set I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font
      {
         unsupported = true; /// @todo implement
      }
      else if ((instruction & 0xF0FF) == 0xF033) // FX33  Store the Binary-coded decimal representation of VX, with the most significant of three digits at the address in I, the middle digit at I plus 1, and the least significant digit at I plus 2. (In other words, take the decimal representation of VX, place the hundreds digit in memory at location in I, the tens digit at location I+1, and the ones digit at location I+2.)
      {
         unsupported = true; /// @todo implement
      }
      else if ((instruction & 0xF0FF) == 0xF055) // FX55  Store V0 to VX in memory starting at address I
      {
         unsupported = true; /// @todo implement
      }
      else if ((instruction & 0xF0FF) == 0xF065) // FX65  Fill V0 to VX with values from memory starting at address I
      {
         unsupported = true; /// @todo implement
      }
      else
      {
         std::cout << "Invalid instruction " << std::hex << instruction << ". Exiting program!" << std::endl;
         break;
      }

      if (unsupported)
      {
         std::cout << "Unsupported instruction " << std::hex << instruction << ". Exiting program!" << std::endl;
         break;
      }

      theProgramCounter += 2;
   }
}
