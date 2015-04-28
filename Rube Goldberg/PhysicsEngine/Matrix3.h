#ifndef MATRIX3_H
#define MATRIX3_H

#include "Vector3.h"
#include "Quaternion.h"

class Matrix3
{
public:
	Matrix3();
	Matrix3(float c);
	Matrix3(float c0, float c1, float c2, float c3, float c4, float c5, float c6, float c7, float c8);
	Matrix3(const Vector3 &col1, const Vector3 &col2, const Vector3 &col3);

	void SetDiagonal(real a, real b, real c);
	void SetInertialTensorCoeffs(real ix, real iy, real iz, real ixy = 0, real ixz = 0, real iyz = 0);
	void SetBlockInertiaTensor(const Vector3 &halfSizes, real mass);
	void SetSkewSymmetric(const Vector3 &vector);
	void SetComponents(const Vector3 &comp1, const Vector3 &comp2, const Vector3 &comp3);

	Vector3 operator*(const Vector3 &vector) const;

	Vector3 transform(const Vector3 &vector) const;
	Vector3 transformTranspose(const Vector3 &vector) const;

	Vector3 GetRowVector(int i) const { return Vector3(data[i * 3], data[i * 3 + 1], data[i * 3 + 2]); }
	Vector3 GetAxisVector(int i) const { return Vector3(data[i], data[i + 3], data[i + 6]); }

	void SetInverse(const Matrix3 &m);
	Matrix3 Inverse() const;
	void Invert();

	void SetTranspose(const Matrix3 &m);
	Matrix3 Transpose() const;

	Matrix3 operator*(const Matrix3 &o) const;
	void operator*=(const Matrix3 &o);
	void operator*=(const real scalar);
	void operator+=(const Matrix3 &o);

	void SetOrientation(const Quaternion &q);

	static Matrix3 Lerp(const Matrix3 &a, const Matrix3 &b, real t);
	
	real data[9];
};

#endif