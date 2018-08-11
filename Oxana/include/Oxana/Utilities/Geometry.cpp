#include "Geometry.h"

namespace Oxana
{
	void Geometry::Line(Vector2i a, Vector2i b, Color color)
	{
		bool steep = Vector2i::Steep(a, b);
		if (steep)
		{
			a.Tranpose();
			b.Tranpose();
		}

		if (a.x > b.x)
			std::swap(a, b);

		//int dx = b.x - a.x;
		//int dy = b.y - a.y;
		//float error = 0;
		//float derror = std::abs((float)dy / dx);
		//int y = a.y;

		for (int x = a.x; x <= b.x; x++)
		{
			float t = (x - a.x) / static_cast<float>(b.x - a.x);
			int y = a.y * (1.0f - t) + b.y * t;
			
			if (steep)
				TrySetPixel(y, x, color);
			else
				TrySetPixel(x, y, color);

			//error += derror;
			//if (error > 0.5f)
			//{
			//	y += (b.x > a.x ? 1 : -1);
			//	error -= 1;
			//}
		}
	}
}