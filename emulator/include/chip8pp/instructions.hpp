#pragma once
#include <cstddef>
#include <cstdint>
#include <span>

namespace chip8pp {

#define CHIP8_INSTRUCTION_ENUM_NAME(INSTRUCTION_NAME) #INSTRUCTION_NAME
enum class InstructionEnum : std::uint16_t {
	INVALID = 0x0000, // used for invalid instructions
	// === Original CHIP-8 instructions === //
	SYS,         // 0x0NNN - Call subroutine at NNN (noop)
	CLS,         // 0x00E0 - Clear Screen
	RET,         // 0x00EE - Return from subroutine
	JMP_NNN,     // 0x1NNN - Jump to NNN
	CALL_NNN,    // 0x2NNN - Call subroutine at NNN
	SE_VX_NN,    // 0x3XNN - Skip next instruction if VX == NN
	SNE_VX_NN,   // 0x4XNN - Skip next instruction if VX != NN
	SE_VX_VY,    // 0x5XY0 - Skip next instruction if VX == VY
	LD_VX_NN,    // 0x6XNN - Set VX to NN
	ADD_VX_NN,   // 0x7XNN - Add NN to VX
	LD_VX_VY,    // 0x8XY0 - Set VX to VY
	OR_VX_VY,    // 0x8XY1 - Set VX to VX | VY
	AND_VX_VY,   // 0x8XY2 - Set VX to VX & VY
	XOR_VX_VY,   // 0x8XY3 - Set VX to VX ^ VY
	ADD_VX_VY,   // 0x8XY4 - Add VY to VX, VF = carry
	SUB_VX_VY,   // 0x8XY5 - Subtract VY from VX, VF = !borrow
	SHR_VX_VY,   // 0x8XY6 - Set VX to VY >> 1, VF = bit 0
	SUBN_VX_VY,  // 0x8XY7 - Set VX to VY - VX, VF = !borrow
	SHL_VX_VY,   // 0x8XYE - Set VX to VY << 1, VF = bit 7
	SNE_VX_VY,   // 0x9XY0 - Skip next instruction if VX != VY
	LD_I_NNN,    // 0xANNN - Set I to NNN
	JMP_V0_NNN,  // 0xBNNN - Jump to NNN + V0
	RND_VX_NN,   // 0xCXNN - Set VX to random byte & NN
	DRW_VX_VY_N, // 0xDXYN - Display N-byte sprite at (VX, VY)
	SKP_VX,      // 0xEX9E - Skip next instruction if key VX is pressed
	SKNP_VX,     // 0xEXA1 - Skip next instruction if key VX is not pressed
	LD_VX_DT,    // 0xFX07 - Set VX to delay timer
	LD_VX_K,     // 0xFX0A - Wait for keypress, store in VX
	LD_DT_VX,    // 0xFX15 - Set delay timer to VX
	LD_ST_VX,    // 0xFX18 - Set sound timer to VX
	ADD_I_VX,    // 0xFX1E - Add VX to I
	LD_F_VX,     // 0xFX29 - Set I to location of sprite for digit VX
	LD_B_VX,     // 0xFX33 - Store BCD representation of VX in I, I+1, I+2
	LD_I_VX,     // 0xFX55 - Store registers V0-VX in memory starting at I
	LD_VX_I,     // 0xFX65 - Read registers V0-VX from memory starting at I
	// used only for counting the number of instructions
	COUNT
};

struct Instruction {
	InstructionEnum instruction;
	std::uint16_t opcode;
	// 2nd nibble
	std::byte x;
	// 3rd nibble
	std::byte y;
	// 4th nibble
	std::byte n;
	// 2nd byte
	std::byte nn;
	// 2nd, 3rd, 4th nibbles
	std::uint16_t nnn;

	static InstructionEnum decodeOpCode(std::uint16_t opcode);
};

} // namespace chip8pp
