#pragma once
#include <cstddef>
#include <memory>
#include <mutex>
#include <vector>

using pixelRGBA_t = std::uint32_t;

class Grid {
	// lock
	mutable std::mutex m_gridMutex;
	std::size_t width;
	std::size_t height;
	std::unique_ptr<pixelRGBA_t[]> buffer;

  public:
	Grid(std::size_t width, std::size_t height);
	void setPixel(std::size_t x, std::size_t y, pixelRGBA_t color);
	void clear();
	void setScreenSize(std::size_t width, std::size_t height);
	pixelRGBA_t getPixel(std::size_t x, std::size_t y) const;
	std::vector<pixelRGBA_t> getBuffer() const;
	std::size_t getWidth() const;
	std::size_t getHeight() const;

  private:
	void unlocked_setPixel(std::size_t x, std::size_t y, pixelRGBA_t color);
	void unlocked_clear();
	void unlocked_setScreenSize(std::size_t width, std::size_t height);
	pixelRGBA_t unlocked_getPixel(std::size_t x, std::size_t y) const;
	std::vector<pixelRGBA_t> unlocked_getBuffer() const;
	std::size_t unlocked_getWidth() const;
	std::size_t unlocked_getHeight() const;
};
