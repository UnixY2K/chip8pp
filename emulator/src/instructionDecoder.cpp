#include <chip8pp/instructions.hpp>
#include <cstddef>
#include <cstdint>

namespace chip8pp {

InstructionEnum Instruction::decodeOpCode(std::uint16_t opcode) {
	std::byte instructionnibble = (std::byte)((opcode & 0xF000) >> 12);
	switch ((std::uint8_t)instructionnibble) {
	case 0x00:
		switch ((std::uint8_t)opcode) {
		case 0x00E0:
			return InstructionEnum::CLS;
		case 0x00EE:
			return InstructionEnum::RET;
		default:
			return InstructionEnum::SYS;
		}
	case 0x01:
		return InstructionEnum::JMP_NNN;
	case 0x02:
		return InstructionEnum::CALL_NNN;
	case 0x03:
		return InstructionEnum::SE_VX_NN;
	case 0x04:
		return InstructionEnum::SNE_VX_NN;
	case 0x05:
		return InstructionEnum::SE_VX_VY;
	case 0x06:
		return InstructionEnum::LD_VX_NN;
	case 0x07:
		return InstructionEnum::ADD_VX_NN;
	case 0x08:
		switch ((std::uint8_t)opcode & 0x000F) {
		case 0x00:
			return InstructionEnum::LD_VX_VY;
		case 0x01:
			return InstructionEnum::OR_VX_VY;
		case 0x02:
			return InstructionEnum::AND_VX_VY;
		case 0x03:
			return InstructionEnum::XOR_VX_VY;
		case 0x04:
			return InstructionEnum::ADD_VX_VY;
		case 0x05:
			return InstructionEnum::SUB_VX_VY;
		case 0x06:
			return InstructionEnum::SHR_VX_VY;
		case 0x07:
			return InstructionEnum::SUBN_VX_VY;
		case 0x0E:
			return InstructionEnum::SHL_VX_VY;
		default:
			return InstructionEnum::INVALID;
		}
		break;
	case 0x09:
		return InstructionEnum::SNE_VX_VY;
	case 0x0A:
		return InstructionEnum::LD_I_NNN;
	case 0x0B:
		return InstructionEnum::JMP_V0_NNN;
	case 0x0C:
		return InstructionEnum::RND_VX_NN;
	case 0x0D:
		return InstructionEnum::DRW_VX_VY_N;
	case 0x0E:
		switch ((std::uint8_t)opcode & 0x00FF) {
		case 0x9E:
			return InstructionEnum::SKP_VX;
		case 0xA1:
			return InstructionEnum::SKNP_VX;
		default:
			return InstructionEnum::INVALID;
		}
		break;
	case 0x0F:
		switch ((std::uint8_t)opcode & 0x00FF) {
		case 0x07:
			return InstructionEnum::LD_VX_DT;
		case 0x0A:
			return InstructionEnum::LD_VX_K;
		case 0x15:
			return InstructionEnum::LD_DT_VX;
		case 0x18:
			return InstructionEnum::LD_ST_VX;
		case 0x1E:
			return InstructionEnum::ADD_I_VX;
		case 0x29:
			return InstructionEnum::LD_F_VX;
		case 0x33:
			return InstructionEnum::LD_B_VX;
		case 0x55:
			return InstructionEnum::LD_I_VX;
		case 0x65:
			return InstructionEnum::LD_VX_I;
		default:
			return InstructionEnum::INVALID;
		}
		break;
	}

	return InstructionEnum::INVALID;
}

} // namespace chip8pp
