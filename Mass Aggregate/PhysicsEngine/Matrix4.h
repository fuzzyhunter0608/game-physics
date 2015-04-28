#ifndef MATRIX4_H
#define MATRIX4_H

#include "Vector3.h"
#include "Quaternion.h"

class Matrix4
{
public:
	Matrix4();

	void SetDiagonal(real a, real b, real c);

	Matrix4 operator*(const Matrix4 &rhs) const;
	Vector3 operator*(const Vector3 &rhs) const;

	Vector3 Transform(const Vector3 &vector) const;

	real GetDeterminant() const;

	void SetInverse(const Matrix4 &m);
	Matrix4 Inverse() const;
	void Invert();

	Vector3 TransformDirection(const Vector3 &vector) const;
	Vector3 TransformInverseDirection(const Vector3 &vector) const;
	Vector3 TransformInverse(const Vector3 &vector) const;

	Vector3 GetAxisVector(int i) const { return Vector3(data[i], data[i + 4], data[1 + 8]); }

	void SetOrientationAndPos(const Quaternion &q, const Vector3 &position);
	void FillGLArray(float array[16]) const;

private:
	real data[12];
};

#endif