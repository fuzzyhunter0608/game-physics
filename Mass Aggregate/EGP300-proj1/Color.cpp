#include "Color.h"

Color Color::Yellow = Color(1, 1, 0, 1);
Color Color::Gray = Color(0.5f, 0.5f, 0.5f, 1);
Color Color::Tan = Color(210/255.f, 180/255.f, 140/255.f, 1);
Color Color::Blue = Color(0, 0, 1, 1);
Color Color::Red = Color(1, 0, 0, 1);

Color::Color()
{
	Color(0, 0, 0, 1);
}

Color::Color(float r, float g, float b, float a)
{
	mColor[0] = r;
	mColor[1] = g;
	mColor[2] = b;
	mColor[3] = a;
}