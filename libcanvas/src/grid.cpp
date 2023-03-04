#include <libcanvas/grid.hpp>

Grid::Grid(std::size_t width, std::size_t height)
    : width(width), height(height), buffer(new pixelRGBA_t[width * height]) {}

void Grid::setPixel(std::size_t x, std::size_t y, pixelRGBA_t color) {
	// lock the mutex
	std::lock_guard<std::mutex> lock{m_gridMutex};
	unlocked_setPixel(x, y, color);
}

void Grid::clear() {
	// lock the mutex
	std::lock_guard<std::mutex> lock{m_gridMutex};
	unlocked_clear();
}

void Grid::setScreenSize(std::size_t width, std::size_t height) {
	// lock the mutex
	std::lock_guard<std::mutex> lock{m_gridMutex};
	unlocked_setScreenSize(width, height);
}

pixelRGBA_t Grid::getPixel(std::size_t x, std::size_t y) const {
	// lock the mutex
	std::lock_guard<std::mutex> lock{m_gridMutex};
	return unlocked_getPixel(x, y);
}

std::vector<pixelRGBA_t> Grid::getBuffer() const {
	// lock the mutex
	std::lock_guard<std::mutex> lock{m_gridMutex};
	return unlocked_getBuffer();
}

std::size_t Grid::getWidth() const {
	// lock the mutex
	std::lock_guard<std::mutex> lock{m_gridMutex};
	return unlocked_getWidth();
}

std::size_t Grid::getHeight() const {
	// lock the mutex
	std::lock_guard<std::mutex> lock{m_gridMutex};
	return unlocked_getHeight();
}

void Grid::unlocked_setPixel(std::size_t x, std::size_t y, pixelRGBA_t color) {
	if (x < width && y < height) {
		buffer[y * width + x] = color;
	}
}

void Grid::unlocked_clear() {
	// set the array to 0s
	std::fill(buffer.get(), buffer.get() + width * height, 0);
}

void Grid::unlocked_setScreenSize(std::size_t width, std::size_t height) {
	// check that the new size is different from the old one
	if (width == this->width && height == this->height) {
		return;
	}
	this->width = width;
	this->height = height;
	// delete the old buffer and create a new one with 0s
	buffer.reset(new pixelRGBA_t[width * height]);
	unlocked_clear();
}

std::vector<pixelRGBA_t> Grid::unlocked_getBuffer() const {
	std::vector<pixelRGBA_t> bufferVector;
	bufferVector.reserve(width * height);
	for (std::size_t i = 0; i < width * height; i++) {
		bufferVector.push_back(buffer[i]);
	}
	return bufferVector;
}

pixelRGBA_t Grid::unlocked_getPixel(std::size_t x, std::size_t y) const {
	if (x < width && y < height) {
		return buffer[y * width + x];
	}
	return 0;
}

std::size_t Grid::unlocked_getWidth() const { return width; }

std::size_t Grid::unlocked_getHeight() const { return height; }
