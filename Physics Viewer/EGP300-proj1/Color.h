#pragma once
class Color
{
public:
	Color();
	Color(float r, float g, float b, float a);

	float* GetColor() { return mColor; }

	static Color Yellow;
	static Color Blue;

protected:
	float mColor[4];
};

