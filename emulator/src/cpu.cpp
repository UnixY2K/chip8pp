#include <chip8pp/cpu.hpp>
#include <chip8pp/instructions.hpp>
#include <cstddef>
#include <cstdint>

namespace chip8pp {

void CPU::timerTick() {
	// lock the mutex
	std::lock_guard<std::mutex> lock(timers_mutex);
	// update the timers
	if ((std::uint8_t)delay_timer > 0) {
		delay_timer = (std::byte)((std::uint8_t)delay_timer - 1);
	}
	if ((std::uint8_t)sound_timer > 0) {
		sound_timer = (std::byte)((std::uint8_t)sound_timer - 1);
	}
}

std::byte CPU::getDelayTimer() {
	// lock the mutex
	std::lock_guard<std::mutex> lock(timers_mutex);
	// return the delay timer
	return delay_timer;
}

std::byte CPU::getSoundTimer() {
	// lock the mutex
	std::lock_guard<std::mutex> lock(timers_mutex);
	// return the sound timer
	return sound_timer;
}

void CPU::setDelayTimer(std::byte value) {
	// lock the mutex
	std::lock_guard<std::mutex> lock(timers_mutex);
	// set the delay timer
	delay_timer = value;
}

void CPU::setSoundTimer(std::byte value) {
	// lock the mutex
	std::lock_guard<std::mutex> lock(timers_mutex);
	// set the sound timer
	sound_timer = value;
}

std::uint16_t CPU::fetch(Memory &memory) {
	std::uint16_t opcode = memory.get_word(pc);
	pc = (pc + 2) & 0x0FFF;
	return opcode;
}

Instruction CPU::decode(std::uint16_t opcode) {
	// Decode the instruction into a struct
	// - Instruction : the instruction (1st nibble)
	// - X           : the 2nd nibble
	// - Y           : the 3rd nibble
	// - N           : the 4th nibble
	// - NN          : the 2nd byte (3rd and 4th nibble)
	// - NNN         : the 2nd 3rd and 4th nibble
	Instruction instruction;
	instruction.instruction = Instruction::decodeOpCode(opcode);
	instruction.opcode = opcode;
	instruction.x = (std::byte)((opcode & 0x0F00) >> 8);
	instruction.y = (std::byte)((opcode & 0x00F0) >> 4);
	instruction.n = (std::byte)(opcode & 0x000F);
	instruction.nn = (std::byte)(opcode & 0x00FF);
	instruction.nnn = (std::uint16_t)(opcode & 0x0FFF);
	return instruction;
}

bool CPU::execute(Instruction instruction, Memory &memory, Screen &screen,
                  Keypad &keypad) {
	// get the instruction list
	static auto instructions = getInstructionList();
	// check that the instruction is not out of bounds
	if (instruction.instruction >= InstructionEnum::COUNT) {
		return false;
	}
	// execute the instruction
	instructions[static_cast<std::size_t>(instruction.instruction)](
	    instruction, *this, memory, screen, keypad);
	return true;
}

} // namespace chip8pp
