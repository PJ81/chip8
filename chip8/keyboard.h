#pragma once

#include "raylib.h"

class Keyboard {
public:
	Keyboard() : keyCodes{ 'X', '1', '2', '3', 'Q', 'W', 'E', 'A', 'S', 'D', 'Y', 'C', '4', 'R', 'F', 'V' } {
		memset(keys, 0, sizeof(keys));
	}

	bool isPressed(unsigned char k) const {
		return keys[k];
	}

	void readKey() {
		for (uint8_t i = 0; i < 16; i++)
			keys[i] = IsKeyDown(keyCodes[i]);
	}

	unsigned char getAnyKey() const {
		for (unsigned char i = 0; i < 16; i++)
			if (keys[i]) return i;
		return 255;
	}

	~Keyboard() {}

private:
	unsigned char keyCodes[16];
	bool keys[16];
};