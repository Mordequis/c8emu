#ifndef CHIP8EMULATOR_H_
#define CHIP8EMULATOR_H_
#include <cstdint>
#include <string>

class Chip8Emulator {
public:
   // Conventions outlined by emulator101.com/chip-8-emulator
   static const uint16_t MEMORY_SIZE = 4096;
   static const uint8_t NUM_V_REGISTERS = 16;
   static const uint16_t PROGRAM_START_ADDRESS = 0x0200;
   static const uint16_t STACK_START_ADDRESS = 0x0EA0;
   static const uint16_t MAX_PROGRAM_SIZE = STACK_START_ADDRESS - PROGRAM_START_ADDRESS;
   Chip8Emulator();
   virtual ~Chip8Emulator();

   bool loadApplication(const std::string& filePath);
   void emulate();
protected:

   uint8_t theRegistersV[NUM_V_REGISTERS];
   uint16_t theMemoryAddressRegisterI;
   uint16_t theStackPointer;
   uint16_t theProgramCounter;
   uint16_t theActualProgramSize;
   uint8_t theMemory[MEMORY_SIZE];
};

#endif /* CHIP8EMULATOR_H_ */
