#pragma once

#include <glm.hpp>

struct Color {
	float r, g, b, a;

	glm::vec4 AsVec4() {
		return glm::vec4(r, g, b, a);
	}
};

struct Colors{
	// Initialise color presets
	static Color SolidRed;
	static Color SolidGreen;
	static Color SolidBlue;
	static Color LightRed;
	static Color LightGreen;
	static Color LightBlue;
	static Color Black;
	static Color White;
};