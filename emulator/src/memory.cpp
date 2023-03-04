#include <chip8pp/memory.hpp>
#include <stdexcept>

Memory::Memory() { reset(); }

void Memory::load_rom(const std::byte *rom, std::size_t size,
                      std::uint16_t offset) {
	if (offset + size > 4096) {
		throw std::runtime_error("ROM too large");
	}
	std::copy(rom, rom + size, memory.begin() + offset);
}

void Memory::load_rom(std::istream &rom, std::size_t size,
                      std::uint16_t offset) {
	if (offset + size > 4096) {
		throw std::runtime_error("ROM too large");
	}
	rom.read(reinterpret_cast<char *>(memory.data() + offset), size);
}

std::byte *Memory::get_memory() { return memory.data(); }

std::uint16_t *Memory::get_stack() { return stack.data(); }

std::byte Memory::get_byte(std::uint16_t address) { return memory[address]; }

std::uint16_t Memory::get_word(std::uint16_t address) {
	return (static_cast<std::uint16_t>(memory[address]) << 8) |
	       static_cast<std::uint16_t>(memory[address + 1]);
}

void Memory::set_byte(std::uint16_t address, std::byte value) {
	memory[address] = value;
}

void Memory::reset() {
	std::fill(memory.begin(), memory.end(), std::byte(0));
	std::fill(stack.begin(), stack.end(), std::uint16_t(0));
}
