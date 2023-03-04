#pragma once
#include <array>
#include <cstdint>

namespace chip8pp {
class Keypad {
  public:
	enum class Key {
		// 1 2 3 C
		// 4 5 6 D
		// 7 8 9 E
		// A 0 B F
		KEY_0,
		KEY_1,
		KEY_2,
		KEY_3,
		KEY_4,
		KEY_5,
		KEY_6,
		KEY_7,
		KEY_8,
		KEY_9,
		KEY_A,
		KEY_B,
		KEY_C,
		KEY_D,
		KEY_E,
		KEY_F,
		COUNT
	};

	bool is_pressed(Key key) const;
	void press(Key key);
	void release(Key key);
	void set(Key key, bool pressed);
	void clear();

  private:
	std::array<bool, static_cast<std::size_t>(Key::COUNT)> m_keys{};
};

} // namespace chip8pp
