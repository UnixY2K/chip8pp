#include <chip8pp/memory.hpp>
#include <chip8pp/utils.hpp>
#include <cstddef>
#include <fstream>
#include <memory>
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

namespace chip8pp::utils {

std::tuple<std::unique_ptr<std::byte[]>, std::size_t>
load_file(const std::filesystem::path &path) {
	// check if file exists
	if (!std::filesystem::exists(path)) {
		throw std::runtime_error(
		    format("File {} does not exist", path.filename().string()));
	}
	// check if file is a regular file
	if (!std::filesystem::is_regular_file(path)) {
		throw std::runtime_error(
		    format("{} is not a regular file", path.string()));
	}
	// open file
	std::ifstream rom(path, std::ios::binary);
	if (!rom.is_open()) {
		throw std::runtime_error(
		    format("Could not open file {}", path.filename().string()));
	}
	// get file size
	rom.seekg(0, std::ios::end);
	auto rom_size = rom.tellg();
	rom.seekg(0, std::ios::beg);
	// check file size
	if (rom_size > Memory::RAM_SIZE - Memory::ROM_START) {
		throw std::runtime_error("Rom too big");
	}

	// load file into buffer
	auto buffer = std::make_unique<std::byte[]>(rom_size);
	rom.read(reinterpret_cast<char *>(buffer.get()), rom_size);
	return {std::move(buffer), rom_size};
}

} // namespace chip8pp::utils
