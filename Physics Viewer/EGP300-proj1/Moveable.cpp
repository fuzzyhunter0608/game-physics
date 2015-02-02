#include "Moveable.h"

Moveable::Moveable(void)
{
}


Moveable::~Moveable(void)
{
}

void Moveable::setPosition(float x, float y, float z)
{
	mPosition.x = x;
	mPosition.y = y;
	mPosition.z = z;
}

void Moveable::setRotation(float x, float y, float z)
{
	mRotation.x = x;
	mRotation.y = y;
	mRotation.z = z;
}

void Moveable::moveLeft(float distance)
{
	mPosition -= getRightVector() * distance;
}

void Moveable::moveRight(float distance)
{
	mPosition += getRightVector() * distance;
}

void Moveable::moveForward(float distance)
{
	mPosition += getForwardVector() * distance;
}

void Moveable::moveBack(float distance)
{
	mPosition -= getForwardVector() * distance;
}

void Moveable::moveUp(float distance)
{
	mPosition += getUpVector() * distance;
}

void Moveable::moveDown(float distance)
{
	mPosition -= getUpVector() * distance;
}

void Moveable::rotateX(float distance)
{
	mRotation.x += distance;
}

void Moveable::rotateY(float distance)
{
	mRotation.y += distance;
}

void Moveable::rotateZ(float distance)
{
	mRotation.z += distance;
}

Vector3 Moveable::getForwardVector()
{
	M3DMatrix33f rotationMatrix;
	getRotationMatrix(rotationMatrix);

	M3DVector3f out, forward = { 0, 0, 1 };

	m3dRotateVector(out, forward, rotationMatrix);

	return Vector3((real)-out[0], (real)-out[1], (real)out[2]);
}

Vector3 Moveable::getRightVector()
{
	M3DMatrix33f rotationMatrix;
	getRotationMatrix(rotationMatrix);

	M3DVector3f out, forward = { 1, 0, 0 };

	m3dRotateVector(out, forward, rotationMatrix);

	return Vector3(-out[0], -out[1], out[2]);
}

Vector3 Moveable::getUpVector()
{
	return Vector3::Cross(getForwardVector(), getRightVector()).Normalized();
}

void Moveable::getRotationMatrix(M3DMatrix33f &matrix)
{
	M3DMatrix33f rotation;

	m3dLoadIdentity33(matrix);
	m3dRotationMatrix33(rotation, (float)m3dDegToRad(mRotation.y), 0.0f, 1.0f, 0.0f);
	m3dMatrixMultiply33(matrix, matrix, rotation);
	m3dRotationMatrix33(rotation, (float)m3dDegToRad(mRotation.x), 1.0f, 0.0f, 0.0f);
	m3dMatrixMultiply33(matrix, matrix, rotation);

	m3dRotationMatrix33(rotation, (float)m3dDegToRad(mRotation.z), 0.0f, 0.0f, 1.0f);
	m3dMatrixMultiply33(matrix, matrix, rotation);
}