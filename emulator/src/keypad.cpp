#include <chip8pp/keypad.hpp>

namespace chip8pp {
bool Keypad::is_pressed(Key key) const {
	return m_keys[static_cast<std::size_t>(key)];
}
void Keypad::press(Key key) { m_keys[static_cast<std::size_t>(key)] = true; }
void Keypad::release(Key key) { m_keys[static_cast<std::size_t>(key)] = false; }
void Keypad::clear() { m_keys.fill(false); }
void Keypad::set(Key key, bool pressed) {
	m_keys[static_cast<std::size_t>(key)] = pressed;
}

} // namespace chip8pp
