#pragma once

#include "Point.h"
#include "Vec3.h"

#include <Windows.h>
#include <gl/GL.h>
#include <cassert>
#include <cstdio>
#include <iostream>

class Drafter {
public:
	void initialize(HDC handle);
	void drawRectangle(Point<float> topLeft, float width, float height, Vec3<float> color);
	void drawOutlineAround(Point<float> point, float width, float height, Vec3<float> color);
	void drawText(Point<float> coords, Vec3<float> color, const char* fmt, ...);
	void drawTextCentered(Point<float> coords, Vec3<float> color, float width, const char* fmt, ...);
	void getViewport(int* viewport);

	/*
		Call preCall() before drawing anything, and afterCall() afterwards. 
		Those functions set Drafter's own device context.
	*/
	bool preCall();
	void afterCall();
	~Drafter();
private:

	class Font {
	public:
		Font(int height);
		void print(const char* text);

		float centerText(float x, float width, char* text);

		~Font();
	private:
		int characterWidth;
		unsigned int base;
		HDC handle;
	};

	bool initialized{ false };
	HGLRC context{ NULL };
	HGLRC gameContext{ NULL };
	HDC handle{ NULL };
	Font* f{nullptr};
};