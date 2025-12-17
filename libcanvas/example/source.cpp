
#define SDL_MAIN_HANDLED
#include <SDL3/SDL.h>

#include <libcanvas/screen.hpp>



int main() {
	constexpr size_t width = 800;
	constexpr size_t height = 600;
	Screen screen(width, height);
	if (!screen.init("Grid", 10, 10)) {
		return -1;
	}
	bool quit = false;
	SDL_Event event;
	size_t currentX = 0;
	size_t currentY = 0;
	while (!quit) {
		if (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				quit = true;
			}
		}
		screen.clear();
		// get the next pixel
		currentX++;
		if (currentX >= screen.getGridWidth()) {
			currentX = 0;
			currentY++;
			if (currentY >= screen.getGridHeight()) {
				currentY = 0;
			}
		}
		screen.setPixel(currentX, currentY, 0xFF0000FF);
		screen.update();
	}

	screen.close();
}
