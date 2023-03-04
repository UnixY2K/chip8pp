#pragma once
#include <cstddef>
#include <libcanvas/grid.hpp>
#include <memory>
#include <string>
#include <string_view>

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;

class Screen {

	SDL_Window *m_window;
	SDL_Renderer *m_renderer;
	SDL_Texture *m_texture;
	std::unique_ptr<pixelRGBA_t> m_mainBuffer;
	std::size_t screenWidth;
	std::size_t screenHeight;
	std::size_t pixelWidth;
	std::size_t pixelHeight;
	Grid grid;

  public:
	Screen(std::size_t screenWidth, std::size_t screenHeight);

	bool init(std::string_view title, std::size_t gridWidth,
	          std::size_t gridHeight);

	void update();

	void setPixel(std::size_t x, std::size_t y, pixelRGBA_t color);
	pixelRGBA_t getPixel(std::size_t x, std::size_t y);
	std::size_t getGridWidth();
	std::size_t getGridHeight();

	void clear();

	void close();

	~Screen();
};
