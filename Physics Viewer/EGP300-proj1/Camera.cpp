#include "Camera.h"

using namespace std;

Camera::Camera(void)
{
	reset();
}

Camera::~Camera(void)
{
}

void Camera::reset()
{
	setPosition(0.0f, -2.0f, -100.0f);
	setRotation(0.0f, 0.0f, 0.0f);
}

void Camera::getViewMatrix(M3DMatrix44f &matrix)
{
	M3DMatrix44f translation, rotation;

	m3dLoadIdentity44(matrix);

	m3dRotationMatrix44(rotation, (float)m3dDegToRad(mRotation.x), 1.0f, 0.0f, 0.0f);
	m3dMatrixMultiply44(matrix, matrix, rotation);
	m3dRotationMatrix44(rotation, (float)m3dDegToRad(mRotation.y), 0.0f, 1.0f, 0.0f);
	m3dMatrixMultiply44(matrix, matrix, rotation);
	m3dTranslationMatrix44(translation, mPosition.x, mPosition.y, mPosition.z);
	m3dMatrixMultiply44(matrix, matrix, translation);
}

void Camera::getViewMatrixReverseY(M3DMatrix44f &matrix)
{
	M3DMatrix44f translation, rotation;

	m3dLoadIdentity44(matrix);

	m3dRotationMatrix44(rotation, (float)m3dDegToRad(mRotation.x), 1.0f, 0.0f, 0.0f);
	m3dMatrixMultiply44(matrix, matrix, rotation);
	m3dRotationMatrix44(rotation, (float)m3dDegToRad(mRotation.y), 0.0f, 1.0f, 0.0f);
	m3dMatrixMultiply44(matrix, matrix, rotation);
	m3dTranslationMatrix44(translation, mPosition.x, mPosition.y, mPosition.z);
	m3dMatrixMultiply44(matrix, matrix, translation);
}