#pragma once
class Color
{
public:
	Color();
	Color(float r, float g, float b, float a);

	float* GetColor() { return mColor; }
	
	static Color Yellow;
	static Color Gray;
	static Color Tan;
	static Color Blue;
	static Color Red;

protected:
	float mColor[4];
};

