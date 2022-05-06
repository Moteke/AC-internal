#pragma once

template<typename T>
class Vec3 {
public:
	T x;
	T y;
	T z;

	Vec3(T x, T y, T z) : x{x}, y{y}, z{z}{}
	Vec3() = default;

	Vec3 operator-() const {
		return { -x, -y, -z };
	}

	Vec3 operator+ (Vec3 other) const {
		return { x + other.x, y + other.y, z + other.z };
	}

	Vec3 operator- (Vec3 other) const {
		return { x - other.x, y - other.y, z - other.z };
	}
};