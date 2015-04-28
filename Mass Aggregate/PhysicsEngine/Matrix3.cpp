#include "Matrix3.h"

Matrix3::Matrix3()
	:Matrix3(0)
{
}

Matrix3::Matrix3(float c)
{
	data[0] = data[1] = data[2] = data[3] = data[4] = data[5] = data[6] = data[7] = data[8] = c;
}

Matrix3::Matrix3(float c0, float c1, float c2, float c3, float c4, float c5, float c6, float c7, float c8)
{
	data[0] = c0; data[1] = c1; data[2] = c2;
	data[3] = c3; data[4] = c4; data[5] = c5;
	data[6] = c6; data[7] = c7; data[8] = c8;
}

Matrix3::Matrix3(const Vector3 &col1, const Vector3 &col2, const Vector3 &col3)
{
	
}

void Matrix3::SetDiagonal(real a, real b, real c)
{
	SetInertialTensorCoeffs(a, b, c);
}

void Matrix3::SetInertialTensorCoeffs(real ix, real iy, real iz, real ixy, real ixz, real iyz)
{
	data[0] = ix;
	data[1] = data[3] = -ixy;
	data[2] = data[6] = -ixz;
	data[4] = iy;
	data[5] = data[7] = -iyz;
	data[8] = iz;
}

void Matrix3::SetBlockInertiaTensor(const Vector3 &halfSizes, real mass)
{
	Vector3 squares = Vector3(halfSizes.x * halfSizes.x, halfSizes.y * halfSizes.y, halfSizes.z * halfSizes.z);
	SetInertialTensorCoeffs(0.3f * mass * (squares.y + squares.z), 0.3f * mass * (squares.x + squares.z), 0.3f * mass * (squares.x + squares.y));
}

void Matrix3::SetSkewSymmetric(const Vector3 &vector)
{
	data[0] = data[4] = data[8] = 0;
	data[1] = -vector.z;
	data[2] = vector.y;
	data[3] = vector.z;
	data[5] = -vector.x;
	data[6] = -vector.y;
	data[7] = vector.x;
}

void Matrix3::SetComponents(const Vector3 &comp1, const Vector3 &comp2, const Vector3 &comp3)
{
	data[0] = comp1.x;
	data[1] = comp2.x;
	data[2] = comp3.x;
	data[3] = comp1.y;
	data[4] = comp2.y;
	data[5] = comp3.y;
	data[6] = comp1.z;
	data[7] = comp2.z;
	data[8] = comp3.z;
}

Vector3 Matrix3::operator*(const Vector3 &vector) const
{
	return Vector3(
		vector.x * data[0] + vector.y * data[1] + vector.z * data[2],
		vector.x * data[3] + vector.y * data[4] + vector.z * data[5],
		vector.x * data[6] + vector.y * data[7] + vector.z * data[8]
		);
}

Vector3 Matrix3::transform(const Vector3 &vector) const
{
	return *this * vector;
}

Vector3 Matrix3::transformTranspose(const Vector3 &vector) const
{
	return Vector3(
		vector.x * data[0] + vector.y * data[3] + vector.z * data[6],
		vector.x * data[1] + vector.y * data[4] + vector.z * data[7],
		vector.x * data[2] + vector.y * data[5] + vector.z * data[8]
		);
}

void Matrix3::SetInverse(const Matrix3 &m)
{
	real t4 = m.data[0] * m.data[4];
	real t6 = m.data[0] * m.data[5];
	real t8 = m.data[1] * m.data[3];
	real t10 = m.data[2] * m.data[3];
	real t12 = m.data[1] * m.data[6];
	real t14 = m.data[2] * m.data[6];

	// Calculate determinant
	real t16 = (t4*m.data[8] - t6*m.data[7] - t8*m.data[8] + t10*m.data[7] + t12*m.data[5] - t14*m.data[4]);

	// Make sure determinant is non-zero
	if (t16 == (real)0.0f) return;
	real t17 = 1 / t16;

	data[0] = (m.data[4] * m.data[8] - m.data[5] * m.data[7])*t17;
	data[1] = -(m.data[1] * m.data[8] - m.data[2] * m.data[7])*t17;
	data[2] = (m.data[1] * m.data[5] - m.data[2] * m.data[4])*t17;
	data[3] = -(m.data[3] * m.data[8] - m.data[5] * m.data[6])*t17;
	data[4] = (m.data[0] * m.data[8] - t14)*t17;
	data[5] = -(t6 - t10)*t17;
	data[6] = (m.data[3] * m.data[7] - m.data[4] * m.data[6])*t17;
	data[7] = -(m.data[0] * m.data[7] - t12)*t17;
	data[8] = (t4 - t8)*t17;
}

Matrix3 Matrix3::Inverse() const
{
	Matrix3 result;
	result.SetInverse(*this);
	return result;
}

void Matrix3::Invert()
{
	SetInverse(*this);
}

void Matrix3::SetTranspose(const Matrix3 &m)
{
	data[0] = m.data[0];
	data[1] = m.data[3];
	data[2] = m.data[6];
	data[3] = m.data[1];
	data[4] = m.data[4];
	data[5] = m.data[7];
	data[6] = m.data[2];
	data[7] = m.data[5];
	data[8] = m.data[8];
}

Matrix3 Matrix3::Transpose() const
{
	Matrix3 result;
	result.SetTranspose(*this);
	return result;
}

Matrix3 Matrix3::operator*(const Matrix3 &o) const
{
	return Matrix3(
		data[0] * o.data[0] + data[1] * o.data[3] + data[2] * o.data[6],
		data[0] * o.data[1] + data[1] * o.data[4] + data[2] * o.data[7],
		data[0] * o.data[2] + data[1] * o.data[5] + data[2] * o.data[8],

		data[3] * o.data[0] + data[4] * o.data[3] + data[5] * o.data[6],
		data[3] * o.data[1] + data[4] * o.data[4] + data[5] * o.data[7],
		data[3] * o.data[2] + data[4] * o.data[5] + data[5] * o.data[8],

		data[6] * o.data[0] + data[7] * o.data[3] + data[8] * o.data[6],
		data[6] * o.data[1] + data[7] * o.data[4] + data[8] * o.data[7],
		data[6] * o.data[2] + data[7] * o.data[5] + data[8] * o.data[8]
		);
}

void Matrix3::operator*=(const Matrix3 &o)
{
	real t1;
	real t2;
	real t3;

	t1 = data[0] * o.data[0] + data[1] * o.data[3] + data[2] * o.data[6];
	t2 = data[0] * o.data[1] + data[1] * o.data[4] + data[2] * o.data[7];
	t3 = data[0] * o.data[2] + data[1] * o.data[5] + data[2] * o.data[8];
	data[0] = t1;
	data[1] = t2;
	data[2] = t3;

	t1 = data[3] * o.data[0] + data[4] * o.data[3] + data[5] * o.data[6];
	t2 = data[3] * o.data[1] + data[4] * o.data[4] + data[5] * o.data[7];
	t3 = data[3] * o.data[2] + data[4] * o.data[5] + data[5] * o.data[8];
	data[3] = t1;
	data[4] = t2;
	data[5] = t3;

	t1 = data[6] * o.data[0] + data[7] * o.data[3] + data[8] * o.data[6];
	t2 = data[6] * o.data[1] + data[7] * o.data[4] + data[8] * o.data[7];
	t3 = data[6] * o.data[2] + data[7] * o.data[5] + data[8] * o.data[8];
	data[6] = t1;
	data[7] = t2;
	data[8] = t3;
}

void Matrix3::operator*=(const real scalar)
{
	data[0] *= scalar; data[1] *= scalar; data[2] *= scalar;
	data[3] *= scalar; data[4] *= scalar; data[5] *= scalar;
	data[6] *= scalar; data[7] *= scalar; data[8] *= scalar;
}

void Matrix3::operator+=(const Matrix3 &o)
{
	data[0] += o.data[0]; data[1] += o.data[1]; data[2] += o.data[2];
	data[3] += o.data[3]; data[4] += o.data[4]; data[5] += o.data[5];
	data[6] += o.data[6]; data[7] += o.data[7]; data[8] += o.data[8];
}

void Matrix3::SetOrientation(const Quaternion &q)
{
	data[0] = 1 - (2 * q.j*q.j + 2 * q.k*q.k);
	data[1] = 2 * q.i*q.j + 2 * q.k*q.r;
	data[2] = 2 * q.i*q.k - 2 * q.j*q.r;
	data[3] = 2 * q.i*q.j - 2 * q.k*q.r;
	data[4] = 1 - (2 * q.i*q.i + 2 * q.k*q.k);
	data[5] = 2 * q.j*q.k + 2 * q.i*q.r;
	data[6] = 2 * q.i*q.k + 2 * q.j*q.r;
	data[7] = 2 * q.j*q.k - 2 * q.i*q.r;
	data[8] = 1 - (2 * q.i*q.i + 2 * q.j*q.j);
}

Matrix3 Matrix3::Lerp(const Matrix3 &a, const Matrix3 &b, real t)
{
	Matrix3 result;
	for (int i = 0; i < 9; ++i)
	{
		result.data[i] = a.data[i] * (1 - t) + b.data[i] * t;
	}

	return result;
}