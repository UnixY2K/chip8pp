#pragma once
#include <array>
#include <chip8pp/instructions.hpp>
#include <chip8pp/keypad.hpp>
#include <chip8pp/memory.hpp>
#include <cstddef>
#include <cstdint>
#include <libcanvas/screen.hpp>
namespace chip8pp {

struct CPU {
	// contains the list of supported quirks
	enum class Quirk {
		// Enables the change of the index register when using Load/Store
		// operations
		ModifyIndexloadStore,
		// used only to count the number of quirks
		COUNT,
	};

	// list of quirks
	std::array<bool, static_cast<std::size_t>(Quirk::COUNT)> quirks = {false};
	static constexpr std::size_t STACK_SIZE = 16;
	// program counter (2 bytes)
	std::uint16_t pc{0x200};
	// index register (2 bytes)
	std::uint16_t index{0x000};
	// timers function
	void timerTick();
	std::byte getDelayTimer();
	std::byte getSoundTimer();
	void setDelayTimer(std::byte value);
	void setSoundTimer(std::byte value);

	// stack
	std::array<std::uint16_t, STACK_SIZE> stack;
	// stack pointer
	std::size_t sp{0};
	// registers [V0, V1, ..., VF]
	std::array<std::byte, 16> registers;

	std::uint16_t fetch(Memory &memory);
	Instruction decode(std::uint16_t opcode);
	bool execute(Instruction instruction, Memory &memory, Screen &screen,
	             Keypad &keypad);

  private:
	// timers - need to be thread safe
	std::byte delay_timer{0x00};
	std::byte sound_timer{0x00};
	// timers mutex
	std::mutex timers_mutex;
};

using InstructionCallback = void (*)(Instruction instruction, CPU &cpu,
                                     Memory &memory, Screen &screen,
                                     Keypad &keypad);

std::span<InstructionCallback> getInstructionList();
} // namespace chip8pp
