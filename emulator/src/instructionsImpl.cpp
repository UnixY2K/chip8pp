#include <chip8pp/cpu.hpp>
#include <chip8pp/instructions.hpp>
#include <chip8pp/memory.hpp>
#include <cstddef>
#include <cstdint>
#include <random>
// check if format is available
#if __has_include(<format>)
#include <format>
using std::format;
// if not, use fmt
#elif __has_include(<fmt/format.h>)
#include <fmt/format.h>
using fmt::format;
#else
#error "No <format> or <fmt/format.h> found"
#endif

namespace chip8pp {
namespace instructions {

std::uint8_t generateRandomNumber() {
	// get a static random number generator
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_int_distribution<> dis(0, 255);
	return (std::uint8_t)dis(gen);
}

void invalid(Instruction, CPU &, Memory &, Screen &, Keypad &) {
	throw std::runtime_error("Invalid instruction");
}

// the non implemented instruction, used for fallback
void notImplemented(Instruction instruction, CPU &, Memory &, Screen &,
                    Keypad &) {
	throw std::runtime_error(format("Instruction {:#x} not implemented",
	                                (uint16_t)instruction.opcode));
}
void noop(Instruction, CPU &, Memory &, Screen &, Keypad &) {}

void CLS(Instruction, CPU &, Memory &, Screen &screen, Keypad &) {
	screen.clear();
}

void RET(Instruction, CPU &cpu, Memory &, Screen &, Keypad &) {
	// check if stack is empty
	if (cpu.sp == 0) {
		// stack underflow at address pc
		throw std::runtime_error(
		    format("Stack underflow at address {:#x}", (uint16_t)cpu.pc));
	}
	cpu.sp--;
	cpu.pc = cpu.stack[cpu.sp];
}

void JMP_NNN(Instruction instruction, CPU &cpu, Memory &, Screen &, Keypad &) {
	cpu.pc = instruction.nnn;
}

void CALL_NNN(Instruction instruction, CPU &cpu, Memory &, Screen &, Keypad &) {
	// check if stack is full
	if (cpu.sp >= CPU::STACK_SIZE) {
		// stack overflow at address pc
		throw std::runtime_error(
		    format("Stack overflow at address {:#x}", (uint16_t)cpu.pc));
	}
	cpu.stack[cpu.sp] = cpu.pc;
	cpu.sp++;
	cpu.pc = instruction.nnn;
}

void SE_VX_NN(Instruction instruction, CPU &cpu, Memory &, Screen &, Keypad &) {
	if (cpu.registers[(uint8_t)instruction.x] == instruction.nn) {
		cpu.pc += 2;
	}
}

void SNE_VX_NN(Instruction instruction, CPU &cpu, Memory &, Screen &,
               Keypad &) {
	if (cpu.registers[(uint8_t)instruction.x] != instruction.nn) {
		cpu.pc += 2;
	}
}

void SE_VX_VY(Instruction instruction, CPU &cpu, Memory &, Screen &, Keypad &) {
	if (cpu.registers[(uint8_t)instruction.x] ==
	    cpu.registers[(uint8_t)instruction.y]) {
		cpu.pc += 2;
	}
}

void LD_VX_NN(Instruction instruction, CPU &cpu, Memory &, Screen &, Keypad &) {
	cpu.registers[(uint8_t)instruction.x] = instruction.nn;
}

void ADD_VX_NN(Instruction instruction, CPU &cpu, Memory &, Screen &,
               Keypad &) {
	cpu.registers[(uint8_t)instruction.x] =
	    (std::byte)((uint8_t)cpu.registers[(size_t)instruction.x] +
	                (uint8_t)instruction.nn);
}

void LD_VX_VY(Instruction instruction, CPU &cpu, Memory &, Screen &, Keypad &) {
	cpu.registers[(uint8_t)instruction.x] =
	    cpu.registers[(uint8_t)instruction.y];
}

void OR_VX_VY(Instruction instruction, CPU &cpu, Memory &, Screen &, Keypad &) {
	cpu.registers[(uint8_t)instruction.x] =
	    (std::byte)((uint8_t)cpu.registers[(size_t)instruction.x] |
	                (uint8_t)cpu.registers[(size_t)instruction.y]);
}

void AND_VX_VY(Instruction instruction, CPU &cpu, Memory &, Screen &,
               Keypad &) {
	cpu.registers[(uint8_t)instruction.x] =
	    (std::byte)((uint8_t)cpu.registers[(size_t)instruction.x] &
	                (uint8_t)cpu.registers[(size_t)instruction.y]);
}

void XOR_VX_VY(Instruction instruction, CPU &cpu, Memory &, Screen &,
               Keypad &) {
	cpu.registers[(uint8_t)instruction.x] =
	    (std::byte)((uint8_t)cpu.registers[(size_t)instruction.x] ^
	                (uint8_t)cpu.registers[(size_t)instruction.y]);
}

void ADD_VX_VY(Instruction instruction, CPU &cpu, Memory &, Screen &,
               Keypad &) {
	// add the values as 16 bit integers
	std::int16_t sum = (std::int16_t)cpu.registers[(uint8_t)instruction.x] +
	                   (std::int16_t)cpu.registers[(uint8_t)instruction.y];
	// set the carry flag
	cpu.registers[0xF] = (sum > 0xFF) ? std::byte(1) : std::byte(0);
	// set the register
	cpu.registers[(uint8_t)instruction.x] = (std::byte)(sum & 0xFF);
}

void SUB_VX_VY(Instruction instruction, CPU &cpu, Memory &, Screen &,
               Keypad &) {
	// add the values as 16 bit integers
	std::int16_t diff = (std::int16_t)cpu.registers[(uint8_t)instruction.x] -
	                    (std::int16_t)cpu.registers[(uint8_t)instruction.y];
	// set the carry flag
	cpu.registers[0xF] = (diff < 0) ? std::byte(0) : std::byte(1);
	// set the register
	cpu.registers[(uint8_t)instruction.x] = (std::byte)(diff & 0xFF);
}

void SHR_VX_VY(Instruction instruction, CPU &cpu, Memory &, Screen &,
               Keypad &) {
	// set the register VX = VY >> 1
	cpu.registers[(uint8_t)instruction.x] = (std::byte)(
	    ((uint8_t)cpu.registers[(uint8_t)instruction.y] >> 1) & 0xFF);

	// set the carry flag with the bit that was shifted out
	cpu.registers[0xF] =
	    (bool)(cpu.registers[(uint8_t)instruction.x] & std::byte(0b00000001))
	        ? std::byte(1)
	        : std::byte(0);
}

void SUBN_VX_VY(Instruction instruction, CPU &cpu, Memory &, Screen &,
                Keypad &) {
	// add the values as 16 bit integers
	std::int16_t diff = (std::int16_t)cpu.registers[(uint8_t)instruction.y] -
	                    (std::int16_t)cpu.registers[(uint8_t)instruction.x];
	// set the carry flag
	cpu.registers[0xF] = (diff < 0) ? std::byte(0) : std::byte(1);
	// set the register
	cpu.registers[(uint8_t)instruction.x] = (std::byte)(diff & 0xFF);
}

void SHL_VX_VY(Instruction instruction, CPU &cpu, Memory &, Screen &,
               Keypad &) {
	// set the register VX = VY << 1
	cpu.registers[(uint8_t)instruction.x] = (std::byte)(
	    ((uint8_t)cpu.registers[(uint8_t)instruction.y] << 1) & 0xFF);

	// set the carry flag with the bit that was shifted out
	cpu.registers[0xF] =
	    (bool)(cpu.registers[(uint8_t)instruction.x] & std::byte(0b10000000))
	        ? std::byte(1)
	        : std::byte(0);
}

void SNE_VX_VY(Instruction instruction, CPU &cpu, Memory &, Screen &,
               Keypad &) {
	if (cpu.registers[(uint8_t)instruction.x] !=
	    cpu.registers[(uint8_t)instruction.y]) {
		cpu.pc += 2;
	}
}

void LD_I_NNN(Instruction instruction, CPU &cpu, Memory &, Screen &, Keypad &) {
	cpu.index = instruction.nnn;
}

void JMP_V0_NNN(Instruction instruction, CPU &cpu, Memory &, Screen &,
                Keypad &) {
	cpu.pc = (std::uint16_t)instruction.nnn + (std::uint16_t)cpu.registers[0];
}

void RND_VX_NN(Instruction instruction, CPU &cpu, Memory &, Screen &,
               Keypad &) {
	std::uint8_t random = generateRandomNumber();
	cpu.registers[(uint8_t)instruction.x] =
	    (std::byte)(random & (std::uint8_t)instruction.nn);
}

void DRW_VX_VY_N(Instruction instruction, CPU &cpu, Memory &memory,
                 Screen &screen, Keypad &) {
	// set F register to 0
	cpu.registers[0xF] = std::byte(0);
	// loop through the height of the sprite
	for (std::uint8_t hline = 0; hline < (std::uint8_t)instruction.n; hline++) {
		std::uint8_t spr_byte =
		    (std::uint8_t)memory.get_byte(cpu.index + hline);
		// loop through the width of the sprite
		for (std::uint8_t vline = 0; vline < 8; vline++) {
			// check if the pixel is set
			if ((spr_byte & (0x80 >> vline)) != 0) {
				// check if the pixel is already set
				pixelRGBA_t pixel = screen.getPixel(
				    (std::uint8_t)cpu.registers[(std::uint8_t)instruction.x] +
				        vline,
				    (std::uint8_t)cpu.registers[(std::uint8_t)instruction.y] +
				        hline);
				if (pixel == 0xFFFFFFFF) {
					// set F register to 1
					cpu.registers[0xF] = std::byte(1);
				}
				// save the pixel to the screen
				screen.setPixel(
				    (std::uint8_t)cpu.registers[(std::uint8_t)instruction.x] +
				        vline,
				    (std::uint8_t)cpu.registers[(std::uint8_t)instruction.y] +
				        hline,
				    0xFFFFFFFF);
			}
		}
	}
}

void SKP_VX(Instruction instruction, CPU &cpu, Memory &, Screen &,
            Keypad &keypad) {
	Keypad::Key key = (Keypad::Key)cpu.registers[(std::uint8_t)instruction.x];
	if (keypad.is_pressed(key)) {
		cpu.pc += 2;
	}
}

void SKNP_VX(Instruction instruction, CPU &cpu, Memory &, Screen &,
             Keypad &keypad) {
	Keypad::Key key = (Keypad::Key)cpu.registers[(std::uint8_t)instruction.x];
	if (!keypad.is_pressed(key)) {
		cpu.pc += 2;
	}
}

void LD_VX_DT(Instruction instruction, CPU &cpu, Memory &, Screen &, Keypad &) {
	cpu.registers[(std::uint8_t)instruction.x] = cpu.getDelayTimer();
}

void LD_VX_K(Instruction instruction, CPU &cpu, Memory &, Screen &,
             Keypad &keypad) {
	// check if a key is pressed
	for (std::uint8_t i = 0; i < 16; i++) {
		Keypad::Key key = (Keypad::Key)i;
		if (keypad.is_pressed(key)) {
			cpu.registers[(std::uint8_t)instruction.x] = (std::byte)i;
			return;
		}
	}
	// if no key is pressed, decrement the program counter
	cpu.pc -= 2;
}

void LD_DT_VX(Instruction instruction, CPU &cpu, Memory &, Screen &, Keypad &) {
	cpu.setDelayTimer(cpu.registers[(std::uint8_t)instruction.x]);
}

void LD_ST_VX(Instruction instruction, CPU &cpu, Memory &, Screen &, Keypad &) {
	cpu.setSoundTimer(cpu.registers[(std::uint8_t)instruction.x]);
}

void ADD_I_VX(Instruction instruction, CPU &cpu, Memory &, Screen &, Keypad &) {
	cpu.index += (std::uint16_t)cpu.registers[(std::uint8_t)instruction.x];
}

void LD_F_VX(Instruction instruction, CPU &cpu, Memory &, Screen &, Keypad &) {
	cpu.index = (std::uint16_t)cpu.registers[(std::uint8_t)instruction.x] * 5;
}

void LD_B_VX(Instruction instruction, CPU &cpu, Memory &memory, Screen &,
             Keypad &) {
	std::uint8_t value =
	    (std::uint8_t)cpu.registers[(std::uint8_t)instruction.x];
	memory.set_byte(cpu.index, (std::byte)(value / 100));
	memory.set_byte(cpu.index + 1, (std::byte)((value / 10) % 10));
	memory.set_byte(cpu.index + 2, (std::byte)((value % 100) % 10));
}

void LD_I_VX(Instruction instruction, CPU &cpu, Memory &memory, Screen &,
             Keypad &) {
	for (std::uint8_t i = 0; i <= (std::uint8_t)instruction.x; i++) {
		memory.set_byte((cpu.index + i), cpu.registers[i]);
	}
	// check for load/store quirk
	if (cpu.quirks[(size_t)CPU::Quirk::ModifyIndexloadStore]) {
		cpu.index += (std::uint8_t)instruction.x + 1;
	}
}

void LD_VX_I(Instruction instruction, CPU &cpu, Memory &memory, Screen &,
             Keypad &) {
	for (std::uint8_t i = 0; i <= (std::uint8_t)instruction.x; i++) {
		cpu.registers[i] = memory.get_byte(cpu.index + i);
	}

	// check for load/store quirk
	if (cpu.quirks[(size_t)CPU::Quirk::ModifyIndexloadStore]) {
		cpu.index += (std::uint8_t)instruction.x + 1;
	}
}

} // namespace instructions

std::span<InstructionCallback> getInstructionList() {
	// map all the instructions to the callback functions
	// if the instruction is not implemented, use the notImplemented function
	static std::array<InstructionCallback, (size_t)InstructionEnum::COUNT>
	    instructionList{
	        // Invalid instruction
	        instructions::invalid,
	        // sys
	        instructions::noop,
	        // cls
	        instructions::CLS,
	        // ret
	        instructions::RET,
	        // JMP_NNN
	        instructions::JMP_NNN,
	        // CALL_NNN
	        instructions::CALL_NNN,
	        // SE_VX_NN
	        instructions::SE_VX_NN,
	        // SNE_VX_NN
	        instructions::SNE_VX_NN,
	        // SE_VX_VY
	        instructions::SE_VX_VY,
	        // LD_VX_NN
	        instructions::LD_VX_NN,
	        // ADD_VX_NN
	        instructions::ADD_VX_NN,
	        // LD_VX_VY
	        instructions::LD_VX_VY,
	        // OR_VX_VY
	        instructions::OR_VX_VY,
	        // AND_VX_VY
	        instructions::AND_VX_VY,
	        // XOR_VX_VY
	        instructions::XOR_VX_VY,
	        // ADD_VX_VY
	        instructions::ADD_VX_VY,
	        // SUB_VX_VY
	        instructions::SUB_VX_VY,
	        // SHR_VX_VY
	        instructions::SHR_VX_VY,
	        // SUBN_VX_VY
	        instructions::SUBN_VX_VY,
	        // SHL_VX_VY
	        instructions::SHL_VX_VY,
	        // SNE_VX_VY
	        instructions::SNE_VX_VY,
	        // LD_I_NNN
	        instructions::LD_I_NNN,
	        // JMP_V0_NNN
	        instructions::JMP_V0_NNN,
	        // RND_VX_NN
	        instructions::RND_VX_NN,
	        // DRW_VX_VY_N
	        instructions::DRW_VX_VY_N,
	        // SKP_VX
	        instructions::SKP_VX,
	        // SKNP_VX
	        instructions::SKNP_VX,
	        // LD_VX_DT
	        instructions::LD_VX_DT,
	        // LD_VX_K
	        instructions::LD_VX_K,
	        // LD_DT_VX
	        instructions::LD_DT_VX,
	        // LD_ST_VX
	        instructions::LD_ST_VX,
	        // ADD_I_VX
	        instructions::ADD_I_VX,
	        // LD_F_VX
	        instructions::LD_F_VX,
	        // LD_B_VX
	        instructions::LD_B_VX,
	        // LD_I_VX
	        instructions::LD_I_VX,
	        // LD_VX_I
	        instructions::LD_VX_I,
	    };

	return instructionList;
}
} // namespace chip8pp
