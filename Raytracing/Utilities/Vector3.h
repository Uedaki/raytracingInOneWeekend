#pragma once

#include <math.h>
#include <stdlib.h>
#include <iostream>

namespace utils
{
	//typedef Vector3<float> Vector3f;

	template <typename T>
	class Vector3
	{
		T e[3];

	public:
		Vector3() = default;
		Vector3(T e0, T e1, T e2)
		{
			e[0] = e0;
			e[1] = e1;
			e[2] = e2;
		}

		~Vector3() = default;

		inline T x() const { return (e[0]); }
		inline T y() const { return (e[1]); }
		inline T z() const { return (e[2]); }
		inline T r() const { return (e[0]); }
		inline T g() const { return (e[1]); }
		inline T b() const { return (e[2]); }

		inline const Vector3<T>&	operator+() const		{ return (*this); }
		inline Vector3<T>			operator-() const		{ return (Vector3<T>(-e[0], -e[1], -e[2])); }
		inline const T&				operator[](int i) const { return (e[i]); }
		inline T&					operator[](int i)		{ return (e[i]); }

		inline float squaredLength() const
		{
			return (static_cast<float>(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]));
		}

		inline float length() const
		{
			return (sqrt(squaredLength()));
		}

		inline Vector3<float> unit()
		{
			return (Vector3<float>(
				static_cast<float>(e[0]) / length(),
				static_cast<float>(e[1]) / length(),
				static_cast<float>(e[2]) / length()));
		}

		inline Vector3<T> operator+(const Vector3<T>& v)
		{
			return (Vector3<T>(e[0] + v.e[0], e[1] + v.e[1], e[2] + v.e[2]));
		}

		inline Vector3<T> operator-(const Vector3<T>& v)
		{
			return (Vector3<T>(e[0] - v.e[0], e[1] - v.e[1], e[2] - v.e[2]));
		}

		inline Vector3<T> operator*(const Vector3<T>& v)
		{
			return (Vector3<T>(e[0] * v.e[0], e[1] * v.e[1], e[2] * v.e[2]));
		}

		inline Vector3<T> operator*(const T& s)
		{
			return (Vector3<T>(e[0] * s, e[1] * s, e[2] * s));
		}

		inline Vector3<T> operator/(const Vector3<T>& v)
		{
			return (Vector3<T>(e[0] / v.e[0], e[1] / v.e[1], e[2] / v.e[2]));
		}

		inline Vector3<T> operator/(const T& s)
		{
			return (Vector3<T>(e[0] / s, e[1] / s, e[2] / s));
		}

		inline T dot(const Vector3<T>& v)
		{
			return (e[0] * v.e[0] + e[1] * v.e[1] + e[2] * v.e[2]);
		}

		inline T cross(const Vector3<T>& v)
		{
			return (Vector3<T>(
				e[1] * v.e[2] - e[2] * v.e[1],
				e[2] * v.e[0] - e[0] * v.e[2],
				e[0] * v.e[1] - e[1] * v.e[0]));
		}

		inline Vector3<T>& operator+=(const Vector3<T>& v)
		{
			e[0] += v.e[0];
			e[1] += v.e[1];
			e[2] += v.e[2];
			return (*this);
		}

		inline Vector3<T>& operator-=(const Vector3<T>& v)
		{
			e[0] -= v.e[0];
			e[1] -= v.e[1];
			e[2] -= v.e[2];
			return (*this);
		}

		inline Vector3<T>& operator*=(const Vector3<T>& v)
		{
			e[0] *= v.e[0];
			e[1] *= v.e[1];
			e[2] *= v.e[2];
			return (*this);
		}

		inline Vector3<T>& operator*=(const T& s)
		{
			e[0] *= s;
			e[1] *= s;
			e[2] *= s;
			return (*this);
		}

		inline Vector3<T>& operator/=(const Vector3<T>& v)
		{
			e[0] /= v.e[0];
			e[1] /= v.e[1];
			e[2] /= v.e[2];
			return (*this);
		}

		inline Vector3<T>& operator/=(const T& s)
		{
			e[0] /= s;
			e[1] /= s;
			e[2] /= s;
			return (*this);
		}

		inline std::istream& operator>>(std::istream& is)
		{
			is >> e[0] >> e[1] >> e[2];
			return (is);
		}

		inline std::ostream& operator<<(std::ostream& os)
		{
			os << "{" << e[0] << ", " << e[1] << ", " << e[2] << "}";
			return (os);
		}
	};
}