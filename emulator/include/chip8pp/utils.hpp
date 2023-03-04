#pragma once
#include <filesystem>
#include <memory>
#include <tuple>

namespace chip8pp::utils {
// load a rom from a file path into a memory buffer
std::tuple<std::unique_ptr<std::byte[]>, std::size_t>
load_file(std::filesystem::path const &path);
} // namespace chip8pp::utils
