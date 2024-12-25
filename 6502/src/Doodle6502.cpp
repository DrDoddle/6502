#include "Doodle6502.h"

Doodle6502::Doodle6502()
{
	memory.fill(0x00);
	writeMemory(0xfffd, 0x08);
	reset();
}

Doodle6502::~Doodle6502()
{
}

void Doodle6502::reset()
{
	//reset the CPU
	A = 0;
	X = 0;
	Y = 0;
	sp = 0xFD;
	p = 0x34;
	pc = (readMemory(0xFFFD) << 8) | readMemory(0xFFFC); // Fetch reset vector
	
}

void Doodle6502::executeInstruction()
{
	uint8_t opcode = readMemory(pc++);
	Instruction& instruction = opcodeTable[opcode];
	(this->*instruction.execute)(getAddress(instruction.addressingmode));
}

uint8_t Doodle6502::readMemory(uint16_t address)
{
	return memory[address];
}

void Doodle6502::writeMemory(uint16_t address, uint8_t value)
{
	memory[address] = value;
}

void Doodle6502::setFlag(StatusFlag flag, bool value)
{
	if (value)
	{
		p |= (1 << flag);
	}
	else {
		p &= ~(1 << flag);
	}
}

bool Doodle6502::getFlag(StatusFlag flag)
{
	return (p & (1 << flag)) != 0;
}

void Doodle6502::pushStack(uint8_t value)
{
	writeMemory(0x100 + sp, value);
	sp--;
}

uint8_t Doodle6502::popStack()
{
	sp++;
	return readMemory(0x100 + sp);
}

uint16_t Doodle6502::getAddress(AddressingMode mode)
{
	switch (mode)
		case accumulator:
			return 0;
}

void Doodle6502::BRK(uint16_t address)
{
	pc++;
	pushStack((pc >> 8) & 0xff);
	pushStack(pc & 0xff);
	pushStack(p | 0x20 | 0x10);
	setFlag(I, 1);
	pc = ((readMemory(0xFFFE) << 8) + (readMemory(0xFFFF)));
	return;
}

