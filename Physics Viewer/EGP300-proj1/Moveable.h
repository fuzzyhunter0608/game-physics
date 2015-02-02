#pragma once

#include <GLTools.h>

#include "PhysicsEngine/Vector3.h"

class Moveable
{
public:
	Moveable();
	~Moveable();

	void setPosition(const Vector3 &position) { mPosition = position; };
	void setPosition(float x, float y, float z);
	void setRotation(const Vector3 &rotation) { mRotation = rotation; };
	void setRotation(float x, float y, float z);

	void moveLeft(float distance);
	void moveRight(float distance);
	void moveForward(float distance);
	void moveBack(float distance);
	void moveUp(float distance);
	void moveDown(float distance);

	void rotateX(float distance);
	void rotateY(float distance);
	void rotateZ(float distance);

	Vector3 getPosition() { return mPosition; };
	Vector3 getRotation() { return mRotation; };

	Vector3 getForwardVector();
	Vector3 getRightVector();
	Vector3 getUpVector();

	void getRotationMatrix(M3DMatrix33f &matrix);

protected:
	Vector3 mPosition;
	Vector3 mRotation;
};

