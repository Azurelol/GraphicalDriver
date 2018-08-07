#pragma once

namespace Oxana
{
	struct Vector2
	{
		float x;
		float y;

		Vector2() : x(0), y(0) {}
		Vector2(float x, float y) : x(x), y(y) {}

		void TransposeInPlace()
		{
			std::swap(x, y);
		}

		static Vector2 Tranpose(Vector2 a)
		{
			a.TransposeInPlace();
			return a;
		}

		static Vector2 Lerp(Vector2 a, Vector2 b, float t)
		{
			int x = a.x * (1.0f - t) + b.x * t;
			int y = a.y * (1.0f - t) + b.y * t;
			return Vector2(x, y);
		}

		static bool Steep(Vector2 a, Vector2 b)
		{
			return std::abs(a.x - b.x) < std::abs(a.y - b.y);
		}

	};
}