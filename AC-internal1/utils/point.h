#pragma once

template<typename T>
class Point {
public:
	Point(T x, T y) : x{ x }, y{ y } {

	}

	T x;
	T y;
};