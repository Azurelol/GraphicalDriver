#pragma once

#include "Image.h"

namespace Oxana
{
	class Geometry : public Image
	{
		public:
		Geometry(float width, float height, sf::Color color = sf::Color::White) : Image(width, height, color) {}
		void Line(Vector2i a, Vector2i b, Color color);
	};

}