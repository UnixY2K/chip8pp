#include <SDL2/SDL.h>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <libcanvas/grid.hpp>
#include <libcanvas/screen.hpp>
#include <sstream>
#include <string_view>

Screen::Screen(std::size_t screenWidth, std::size_t screenHeight)
    : m_window(nullptr), m_renderer(nullptr), m_texture(nullptr),
      m_mainBuffer(nullptr), screenWidth(screenWidth),
      screenHeight(screenHeight), pixelWidth(screenWidth),
      pixelHeight(screenHeight), grid(1, 1) {}

bool Screen::init(std::string_view title, std::size_t gridWidth,
                  std::size_t gridHeight) {
	pixelWidth = screenWidth / gridWidth;
	pixelHeight = screenHeight / gridHeight;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "Could not initialize SDL graphics" << std::endl;
		return false;
	}

	m_window = SDL_CreateWindow(title.data(), SDL_WINDOWPOS_UNDEFINED,
	                            SDL_WINDOWPOS_UNDEFINED, screenWidth,
	                            screenHeight, SDL_WINDOW_SHOWN);

	if (!m_window) {
		SDL_Log("Could not create the window. ");
		SDL_Log("%s", SDL_GetError());
		SDL_Quit();
		return false;
	}

	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_PRESENTVSYNC);

	if (!m_renderer) {
		SDL_Log("Could not create the renderer. ");
		SDL_Log("%s", SDL_GetError());
		SDL_DestroyWindow(m_window);
		SDL_Quit();
		return false;
	}

	m_texture =
	    SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888,
	                      SDL_TEXTUREACCESS_STATIC, screenWidth, screenHeight);

	if (!m_texture) {
		SDL_Log("Could not create the texture. ");
		SDL_Log("%s", SDL_GetError());
		SDL_DestroyRenderer(m_renderer);
		SDL_DestroyWindow(m_window);
		SDL_Quit();
		return false;
	}

	grid.setScreenSize(gridWidth, gridHeight);
	// initialize the main buffer
	size_t pixelSize = pixelWidth * pixelHeight;
	size_t pixelCount = gridWidth * gridHeight;
	m_mainBuffer.reset(new Uint32[pixelCount * pixelSize]);
	return true;
}
void Screen::update() {
	std::vector<pixelRGBA_t> gridBuffer = grid.getBuffer();
	pixelRGBA_t *mainBuffer = m_mainBuffer.get();
	// update the texture with the grid data
	for (size_t i = 0; i < grid.getWidth(); i++) {
		for (size_t j = 0; j < grid.getHeight(); j++) {
			for (size_t k = 0; k < pixelWidth; k++) {
				for (size_t l = 0; l < pixelHeight; l++) {
					mainBuffer[(i * pixelWidth + k) +
					           (j * pixelHeight + l) * screenWidth] =
					    gridBuffer[i + j * grid.getWidth()];
				}
			}
		}
	}

	SDL_UpdateTexture(m_texture, nullptr, m_mainBuffer.get(),
	                  screenWidth * sizeof(Uint32));
	SDL_RenderClear(m_renderer);
	SDL_RenderCopy(m_renderer, m_texture, nullptr, nullptr);
	SDL_RenderPresent(m_renderer);
}

void Screen::setPixel(std::size_t x, std::size_t y, pixelRGBA_t color) {
	grid.setPixel(x, y, color);
}

pixelRGBA_t Screen::getPixel(std::size_t x, std::size_t y) {
	return grid.getPixel(x, y);
}

std::size_t Screen::getGridWidth() { return grid.getWidth(); }

std::size_t Screen::getGridHeight() { return grid.getHeight(); }

void Screen::clear() { grid.clear(); }

void Screen::close() {
	SDL_DestroyTexture(m_texture);
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

Screen::~Screen() { close(); }
