#pragma once
#include <array>
#include <cstddef>
#include <cstdint>
#include <istream>

constexpr std::byte font[]{
    static_cast<std::byte>(0xF0), static_cast<std::byte>(0x90),
    static_cast<std::byte>(0x90), static_cast<std::byte>(0x90),
    static_cast<std::byte>(0xF0), // 0
    static_cast<std::byte>(0x20), static_cast<std::byte>(0x60),
    static_cast<std::byte>(0x20), static_cast<std::byte>(0x20),
    static_cast<std::byte>(0x70), // 1
    static_cast<std::byte>(0xF0), static_cast<std::byte>(0x10),
    static_cast<std::byte>(0xF0), static_cast<std::byte>(0x80),
    static_cast<std::byte>(0xF0), // 2
    static_cast<std::byte>(0xF0), static_cast<std::byte>(0x10),
    static_cast<std::byte>(0xF0), static_cast<std::byte>(0x10),
    static_cast<std::byte>(0xF0), // 3
    static_cast<std::byte>(0x90), static_cast<std::byte>(0x90),
    static_cast<std::byte>(0xF0), static_cast<std::byte>(0x10),
    static_cast<std::byte>(0x10), // 4
    static_cast<std::byte>(0xF0), static_cast<std::byte>(0x80),
    static_cast<std::byte>(0xF0), static_cast<std::byte>(0x10),
    static_cast<std::byte>(0xF0), // 5
    static_cast<std::byte>(0xF0), static_cast<std::byte>(0x80),
    static_cast<std::byte>(0xF0), static_cast<std::byte>(0x90),
    static_cast<std::byte>(0xF0), // 6
    static_cast<std::byte>(0xF0), static_cast<std::byte>(0x10),
    static_cast<std::byte>(0x20), static_cast<std::byte>(0x40),
    static_cast<std::byte>(0x40), // 7
    static_cast<std::byte>(0xF0), static_cast<std::byte>(0x90),
    static_cast<std::byte>(0xF0), static_cast<std::byte>(0x90),
    static_cast<std::byte>(0xF0), // 8
    static_cast<std::byte>(0xF0), static_cast<std::byte>(0x90),
    static_cast<std::byte>(0xF0), static_cast<std::byte>(0x10),
    static_cast<std::byte>(0xF0), // 9
    static_cast<std::byte>(0xF0), static_cast<std::byte>(0x90),
    static_cast<std::byte>(0xF0), static_cast<std::byte>(0x90),
    static_cast<std::byte>(0x90), // A
    static_cast<std::byte>(0xE0), static_cast<std::byte>(0x90),
    static_cast<std::byte>(0xE0), static_cast<std::byte>(0x90),
    static_cast<std::byte>(0xE0), // B
    static_cast<std::byte>(0xF0), static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x80), static_cast<std::byte>(0x80),
    static_cast<std::byte>(0xF0), // C
    static_cast<std::byte>(0xE0), static_cast<std::byte>(0x90),
    static_cast<std::byte>(0x90), static_cast<std::byte>(0x90),
    static_cast<std::byte>(0xE0), // D
    static_cast<std::byte>(0xF0), static_cast<std::byte>(0x80),
    static_cast<std::byte>(0xF0), static_cast<std::byte>(0x80),
    static_cast<std::byte>(0xF0), // E
    static_cast<std::byte>(0xF0), static_cast<std::byte>(0x80),
    static_cast<std::byte>(0xF0), static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x80) // F
};

class Memory {

  public:
	static constexpr std::uint16_t ROM_OFFSET = 0x200;
	static constexpr std::uint16_t RAM_SIZE = 0x1000;
	static constexpr std::uint16_t ROM_START = 0x200;
	static constexpr std::uint16_t STACK_SIZE = 0x10;
	Memory();
	void load_rom(const std::byte *rom, std::size_t size,
	              std::uint16_t offset = ROM_OFFSET);
	// istream load_rom
	void load_rom(std::istream &rom, std::size_t size,
	              std::uint16_t offset = ROM_OFFSET);
	std::byte *get_memory();
	std::uint16_t *get_stack();
	std::byte get_byte(std::uint16_t address);
	std::uint16_t get_word(std::uint16_t address);

	void set_byte(std::uint16_t address, std::byte value);

	void reset();

  private:
	std::array<std::byte, RAM_SIZE> memory;
	std::array<std::uint16_t, STACK_SIZE> stack;
};
