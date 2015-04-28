#ifndef QUATERNION_H
#define QUATERNION_H

#include "Vector3.h"

class Quaternion
{
public:
	Quaternion();
	Quaternion(const real r, const real i, const real j, const real k);

	real r;
	real i;
	real j;
	real k;

	void Normalize();

	void operator *=(const Quaternion &rhs);
	void AddScaledVector(const Vector3 &vector, real scale);
	void RotateByVector(const Vector3 &vector);
};

#endif