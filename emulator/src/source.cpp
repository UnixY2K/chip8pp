#define SDL_MAIN_HANDLED

#include <chrono>
#include <format>
#include <iostream>
#include <string>
#include <thread>

#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>

#include <CLI/App.hpp>
#include <CLI/CLI.hpp>

#include <libcanvas/screen.hpp>

#include <chip8pp/cpu.hpp>
#include <chip8pp/instructions.hpp>
#include <chip8pp/keypad.hpp>
#include <chip8pp/memory.hpp>
#include <chip8pp/utils.hpp>

void cpu_thread_fn(std::stop_token stop_token, chip8pp::CPU &cpu,
                   Memory &memory, Screen &screen, chip8pp::Keypad &keypad) {
	try {
		while (!stop_token.stop_requested()) {
			// std::this_thread::sleep_for(std::chrono::milliseconds(100));
			std::uint16_t opcode = cpu.fetch(memory);
			// print the program counter and the opcode
			// std::cout << format("PC: {:#04x} | Opcode: {:#06x}\n",
			//                    (std::uint16_t)cpu.pc, opcode);
			chip8pp::Instruction instruction = cpu.decode(opcode);
			cpu.execute(instruction, memory, screen, keypad);
		}
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
}

void timer_thread_fn(std::stop_token stop_token, chip8pp::CPU &cpu) {
	try {
		while (!stop_token.stop_requested()) {
			std::this_thread::sleep_for(std::chrono::milliseconds(16));
			cpu.timerTick();
		}
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
}

void main_thread_fn(chip8pp::CPU &, Memory &, Screen &screen,
                    chip8pp::Keypad &keypad) {
	// hashmaps to map SDL keys to chip8 keys
	std::unordered_map<SDL_Keycode, chip8pp::Keypad::Key> keymap = {
	    // 1 2 3 C | 1 2 3 4
	    // 4 5 6 D | Q W E R
	    // 7 8 9 E | A S D F
	    // A 0 B F | Z X C V
	    {SDLK_1, chip8pp::Keypad::Key::KEY_1},
	    {SDLK_2, chip8pp::Keypad::Key::KEY_2},
	    {SDLK_3, chip8pp::Keypad::Key::KEY_3},
	    {SDLK_4, chip8pp::Keypad::Key::KEY_C},
	    {SDLK_Q, chip8pp::Keypad::Key::KEY_4},
	    {SDLK_W, chip8pp::Keypad::Key::KEY_5},
	    {SDLK_E, chip8pp::Keypad::Key::KEY_6},
	    {SDLK_R, chip8pp::Keypad::Key::KEY_D},
	    {SDLK_A, chip8pp::Keypad::Key::KEY_7},
	    {SDLK_S, chip8pp::Keypad::Key::KEY_8},
	    {SDLK_D, chip8pp::Keypad::Key::KEY_9},
	    {SDLK_F, chip8pp::Keypad::Key::KEY_E},
	    {SDLK_Z, chip8pp::Keypad::Key::KEY_A},
	    {SDLK_X, chip8pp::Keypad::Key::KEY_0},
	    {SDLK_C, chip8pp::Keypad::Key::KEY_B},
	    {SDLK_V, chip8pp::Keypad::Key::KEY_F},
	};
	// timer to draw each 16.666 ms
	std::chrono::steady_clock::time_point last_draw =
	    std::chrono::steady_clock::now();
	bool continue_loop = true;
	while (continue_loop) {
		keypad.clear();
		// process SDL events
		SDL_Event event;
		if (SDL_PollEvent(&event)) {
			switch (event.type) {
			// key pressed
			case SDL_EVENT_KEY_DOWN:
				// check for each mapped key if it was pressed
				for (auto &[sdl_key, chip8_key] : keymap) {
					if (event.key.key == sdl_key) {
						keypad.press(chip8_key);
					}
				}
				break;
			case SDL_EVENT_QUIT:
				continue_loop = false;
				break;
			}
		}
		// get the remaining time to draw
		auto remaining_time =
		    std::chrono::duration_cast<std::chrono::milliseconds>(
		        std::chrono::steady_clock::now() - last_draw)
		        .count();
		// if 16.666 ms have passed, draw the screen
		if (remaining_time >= 16) {
			screen.update();
			last_draw = std::chrono::steady_clock::now();
		} else {
			// sleep the remaining time
			std::this_thread::sleep_for(
			    std::chrono::milliseconds(16 - remaining_time));
		}
	}
}

int main(int argc, char **argv) {
	CLI::App app{"Chip8 Emulator", "chip8pp"};
	bool verbose = {false};
	app.add_flag("-v,--verbose", verbose, "Verbose mode");
	// rom positional parameter
	std::filesystem::path rom_path;
	app.add_option("rom", rom_path, "Path to the rom file");
	CLI11_PARSE(app, argc, argv);

	try {
		if (rom_path.empty()) {
			std::cout << "must provide a rom file\n";
			return 0;
		}
		auto [rom, rom_size] = chip8pp::utils::load_file(rom_path);
		Memory memory;
		// load fontset into ram
		// store the font in 0x50 to 0x9F
		memory.load_rom(font, sizeof(font), 0x50);
		// load rom into ram
		memory.load_rom(rom.get(), rom_size, 0x200);

		// Keypad keypad;
		chip8pp::Keypad keypad;
		// define the cpu
		chip8pp::CPU cpu;

		// define the screen 64x32
		// screen constants (Width, Height)
		constexpr std::size_t screen_width = 64;
		constexpr std::size_t screen_height = 32;
		constexpr std::size_t screen_scale = 10;
		Screen screen(screen_width * screen_scale,
		              screen_height * screen_scale);

		if (!screen.init("Chip8 Emulator", screen_width, screen_height)) {
			return -1;
		}
		// launch the cpu thread
		std::jthread cpu_thread(cpu_thread_fn, std::ref(cpu), std::ref(memory),
		                        std::ref(screen), std::ref(keypad));
		// launch the timer thread
		std::jthread timer_thread(timer_thread_fn, std::ref(cpu));

		// launch the main thread
		main_thread_fn(std::ref(cpu), std::ref(memory), std::ref(screen),
		               std::ref(keypad));

		screen.close();
	} catch (const std::exception &e) {
		std::cerr << std::format("{}\n", e.what());
	}
}
