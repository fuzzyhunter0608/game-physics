#pragma once

#include "Moveable.h"

class Camera : public Moveable
{
public:
	Camera();
	~Camera();

	void reset();

	void getRotationMatrix(M3DMatrix33f &matrix);
	void getViewMatrix(M3DMatrix44f &matrix);
	void getViewMatrixReverseY(M3DMatrix44f &matrix);
};