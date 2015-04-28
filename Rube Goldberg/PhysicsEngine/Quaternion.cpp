#include "Quaternion.h"

Quaternion Quaternion::identity = Quaternion(1, 0, 0, 0);

Quaternion::Quaternion()
	:r(1)
	,i(0)
	,j(0)
	,k(0)
{
}

Quaternion::Quaternion(const real r, const real i, const real j, const real k)
	:r(r)
	,i(i)
	,j(j)
	,k(k)
{
}

void Quaternion::Normalize()
{
	real d = r*r + i*i + j*j + k*k;

	// Check for zero length quaternion
	if (d < real_epsilon)
	{
		r = 1;
		return;
	}

	d = ((real)1.0) / sqrt(d);
	r *= d;
	i *= d;
	j *= d;
	k *= d;
}

void Quaternion::operator *=(const Quaternion &rhs)
{
	Quaternion q = *this;
	r = q.r * rhs.r - q.i * rhs.i - q.j * rhs.j - q.k * rhs.k;
	i = q.r * rhs.i + q.i * rhs.r + q.j * rhs.k - q.k * rhs.j;
	j = q.r * rhs.j + q.j * rhs.r + q.k * rhs.i - q.i * rhs.k;
	r = q.r * rhs.k + q.k * rhs.r + q.i * rhs.j - q.j * rhs.i;
}

void Quaternion::AddScaledVector(const Vector3 &vector, real scale)
{
	Quaternion q(0, vector.x * scale, vector.y * scale, vector.z * scale);
	q *= *this;
	
	r += q.r * ((real)0.5);
	i += q.i * ((real)0.5);
	j += q.j * ((real)0.5);
	k += q.k * ((real)0.5);
}

void Quaternion::RotateByVector(const Vector3 &vector)
{
	Quaternion q(0, vector.x, vector.y, vector.z);
	(*this) *= q;
}