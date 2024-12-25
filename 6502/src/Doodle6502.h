#pragma once

#include <stdint.h>
#include <bitset>
#include <array>


//list from https://www.nesdev.org/obelisk-6502-guide/addressing.html
enum AddressingMode {
	implicit,
	accumulator,
	immediate,
	zeropage, zeropage_x, zeropage_y,
	relative,
	absolute, absolute_x, absolute_y,
	indirect,
	indexed_indirect,
	indirect_indexed
};

enum StatusFlag {
	C = 0,  // Carry
	Z = 1,  // Zero
	I = 2,  // Interrupt Disable
	D = 3,  // Decimal Mode
	B = 4,  // Break Command
	U = 5,  // Unused
	V = 6,  // Overflow
	N = 7   // Negative
};



class Doodle6502 
{
public:
	Doodle6502();
	~Doodle6502();


					//REGISTERS
	uint8_t A;		//Accumulator register
	uint8_t X;		//X-index register
	uint8_t Y;		//Y-index register

	uint8_t sp;		//Stack Pointer
	uint16_t pc;	//Program Counter

	uint8_t p;		//Status Register
		
	
	
	std::array<uint8_t, 0x10000> memory = {}; // 64kb of memory

	void reset();
	void executeInstruction();
	uint8_t readMemory(uint16_t);
	void writeMemory(uint16_t, uint8_t);
	void setFlag(StatusFlag, bool);
	bool getFlag(StatusFlag);

	void pushStack(uint8_t);
	uint8_t popStack();

	uint16_t getAddress(AddressingMode); //calculate effective address based off addressing mode
	
	

private:


	struct Instruction
	{
		const char* name;		// ex "LDA" or "JMP"
		AddressingMode addressingmode;
		uint8_t cycles;
		void (Doodle6502::* execute)(uint16_t address);
	};

	std::array<Instruction, 256> opcodeTable = { {{"BRK",AddressingMode::implicit,7, &Doodle6502::BRK}}};
	

	void BRK(uint16_t);
};
