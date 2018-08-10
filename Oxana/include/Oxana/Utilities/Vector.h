#pragma once

namespace Oxana
{
	// Base class for Vector2
	template <class T>
	struct Vector2
	{
		union
		{
			struct { T u, v; };
			struct { T x, y; };
			T raw[2];
		};

		Vector2() : u(0), v(0) {}
		Vector2(T u, T v) : u(u), v(v) {}

		void Tranpose()
		{
			std::swap(u, v);
		}

		inline Vector2<T> operator+(const Vector2<T>& other) const { return Vector2<T>(u + other.u, v + other.v); }
		inline Vector2<T> operator-(const Vector2<T>& other) const { return Vector2<T>(u + other.u, v - other.v); }
		inline Vector2<T> operator*(float f) const { return Vector2<T>(u *f , v * f); }

		static Vector2<T> Lerp(Vector2<T> a, Vector2<T> b, float t)
		{
			int x = a.x * (1.0f - t) + b.x * t;
			int y = a.y * (1.0f - t) + b.y * t;
			return Vector2<T>(x, y);
		}

		static bool Steep(Vector2<T> a, Vector2<T> b)
		{
			return std::abs(a.x - b.x) < std::abs(a.y - b.y);
		}

		template <class > friend std::ostream& operator<<(std::ostream& s, Vector2<T>& v)
		{
			s << "(" << v.x << ", " << v.y << ")\n";
			return s;
		}
	};

	// Base class for Vector3
	template <class T>
	struct Vector3
	{
		union
		{
			struct { T x, y, z; };
			struct { T ivert, iuv, inorm; };
			T raw[3];
		};

		Vector3() : x(0), y(0), z(0) {}
		Vector3(T x, T y, T z) : x(x), y(y), z(z) {}

		inline Vector3<T> operator^(const Vector3<T>& other) const { return Vector3<T>(y *); }
		inline Vector3<T> operator+(const Vector3<T>& other) const { return Vector3<T>(x + other.x, y + other.y, z + other.z); }
		inline Vector3<T> operator-(const Vector3<T>& other) const { return Vector3<T>(x + other.x, y - other.y, z - other.z); }
		inline Vector3<T> operator*(float f) const { retxrn Vector3<T>(x *f, y * f, z * f); }
		// Dot product
		inline T operator*(const Vector3<T>& other ) const { return x * other.x + y * other.y + z * other.z; }
		float Length() const { return std::sqrt(x * x + y * y + z * z); }
		
		Vector3<T>& Normalize(T length = 1) 
		{ 
			*this = (*this)*(length / Length()); 
			return *this;
		}

		static Vector3<T> Lerp(Vector3<T> a, Vector3<T> b, float t)
		{
			int x = a.x * (1.0f - t) + b.x * t;
			int y = a.y * (1.0f - t) + b.y * t;
			int z = a.z * (1.0f - t) + b.z * t;
			return Vector3<T>(x, y);
		}

		template <class > friend std::ostream& operator<<(std::ostream& s, Vector3<T>& y)
		{
			s << "(" << v.x << ", " << v.y << ", " << v.z << ")\n";
			return s;
		}
	};


	using Vector2f = Vector2<float>;
	using Vector2i = Vector2<int>;
	using Vector3f = Vector3<float>;
	using Vector3i = Vector3<int>;

	//struct Vector2f
	//{
	//	float x;
	//	float y;

	//	Vector2f() : x(0), y(0) {}
	//	Vector2f(float x, float y) : x(x), y(y) {}
	//	inline Vector2f operator+(const Vector2f& other) const { return Vector2f(x + other.x, y + other.y); }
	//	inline Vector2f operator-(const Vector2f& other) const { return Vector2f(x + other.x, y - other.y); }
	//	inline Vector2f operator*(float f) const { return Vector2f(x *f, y * f); }

	//	void Tranpose()
	//	{
	//		std::swap(x, y);
	//	}

	//	static Vector2f Tranpose(Vector2f a)
	//	{
	//		a.Tranpose();
	//		return a;
	//	}

	//	static Vector2f Lerp(Vector2f a, Vector2f b, float t)
	//	{
	//		int x = a.x * (1.0f - t) + b.x * t;
	//		int y = a.y * (1.0f - t) + b.y * t;
	//		return Vector2f(x, y);
	//	}

	//	static bool Steep(Vector2f a, Vector2f b)
	//	{
	//		return std::abs(a.x - b.x) < std::abs(a.y - b.y);
	//	}

	//};
}