#include "Drafter.h"
#include "utils/utils.h"

Drafter::Drafter()
{
	HGLRC gameContext = wglGetCurrentContext();

	handle = GetDC(utils::getWindow());
	context = wglCreateContext(handle);

	wglMakeCurrent(handle, context);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// (left, bottom, right, top)
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	glOrtho(0.0, viewport[2], 0.0, viewport[3], 0.0, 0.1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(0, 0, 0, 1.0);

	f = new Font{ 20 };

	wglMakeCurrent(handle, gameContext);

	initialized = true;
	g_drafter = this;
}

//void Drafter::initialize(HDC h)
//{
//	if (initialized) return;
//
//	HGLRC gameContext = wglGetCurrentContext();
//
//	handle = h;
//	context = wglCreateContext(handle);
//
//	wglMakeCurrent(handle, context);
//
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//
//	// (left, bottom, right, top)
//	GLint viewport[4];
//	glGetIntegerv(GL_VIEWPORT, viewport);
//
//	glOrtho(0.0, viewport[2], 0.0, viewport[3], 0.0, 0.1);
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//	glClearColor(0, 0, 0, 1.0);
//
//	f = new Font{ 20 };
//
//	wglMakeCurrent(handle, gameContext);
//
//	initialized = true;
//}

void Drafter::drawOutlineAround(const Point<float> point, float width, float height, Vec3<float> color)
{
	glColor3f(color.x, color.y, color.z);

	Point<float> topLeft = Point(point.x - (width / 2), point.y + (height / 2));
	Point<float> topRight = Point(topLeft.x + width, topLeft.y);
	Point<float> bottomLeft = Point(topLeft.x, topLeft.y - height);
	Point<float> bottomRight = Point(bottomLeft.x + width, bottomLeft.y);

	glBegin(GL_LINE_LOOP);
	glVertex2f(topLeft.x, topLeft.y);
	glVertex2f(topRight.x, topRight.y);
	glVertex2f(bottomRight.x, bottomRight.y);
	glVertex2f(bottomLeft.x, bottomLeft.y);
	glEnd();
}

void Drafter::getViewport(int* viewport)
{
	glGetIntegerv(GL_VIEWPORT, viewport);
}

bool Drafter::preCall()
{
	assert(initialized || "preCall() called, but drafter not initialized!");

	gameContext = wglGetCurrentContext();
	wglMakeCurrent(handle, context);

	return true;
}

void Drafter::drawRectangle(Point<float> topLeft, float width, float height, Vec3<float> color)
{
	glColor3f(color.x, color.y, color.z);
	glRectf(topLeft.x, topLeft.y, topLeft.x + width, topLeft.y - height);
}

void Drafter::drawText(Point<float> coords, Vec3<float> color, const char* fmt, ...)
{
	if (fmt == NULL) return; 
	
	char text[255];
	va_list ap; // ptr to the list of arguments
	// parse arguments
	va_start(ap, fmt);
	vsprintf_s(text, 255, fmt, ap);
	va_end(ap);

	glColor3f(color.x, color.y, color.z);
	glRasterPos2f(coords.x, coords.y);

	f->print(text);
}

void Drafter::afterCall()
{
	wglMakeCurrent(handle, gameContext);
}

void Drafter::drawTextCentered(Point<float> coords, Vec3<float> color, float width, const char* fmt, ...)
{
	if (fmt == NULL) return;

	char text[255];
	va_list ap; // ptr to the list of arguments
	// parse arguments
	va_start(ap, fmt);
	vsprintf_s(text, 255, fmt, ap);
	va_end(ap);

	coords.x = f->centerText(coords.x, width, text);
	drawText(coords, color, text);
}

Drafter::~Drafter()
{
	wglDeleteContext(context);
	delete f;
}

Drafter::Font::Font(int height) {
	HFONT font;
	HFONT oldFont;

	handle = wglGetCurrentDC();
	base = glGenLists(96);

	font = CreateFont(-(height), 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE | DEFAULT_PITCH, L"Lucida Console");
	oldFont = (HFONT)SelectObject(handle, font);
	// save char size to later center text
	GetTextExtentPoint32A(handle, "a", 1, (LPSIZE)(&characterWidth));
	// for some reason the wglUseFontBitmaps must be called
	// after GetTextExtentPoint32A
	wglUseFontBitmaps(handle, 32, 96, base);

	SelectObject(handle, oldFont);
	DeleteObject(font);
}

void Drafter::Font::print(const char* text)
{
	glPushAttrib(GL_LIST_BIT);
	glListBase(base - 32);
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
	glPopAttrib();
}

float Drafter::Font::centerText(float x, float width, char* text)
{
	int textWidth = strlen(text) * characterWidth;

	if (width > textWidth) {
		return x + (width - textWidth) / 2;
	}

	return x - (textWidth - width) / 2;
}

Drafter::Font::~Font()
{
	glDeleteLists(base, 96);
}
