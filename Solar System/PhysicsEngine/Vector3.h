#pragma once

#include "Core.h"

#include <string>

using namespace std;

class Vector3
{
public:
	real x, y, z;

	// Constructors
	Vector3();
	Vector3(real x);
	Vector3(real x, real y, real z);

	// Functions
	void AddScaledVector(const Vector3& vector, real scale);

	real Length() const;
	real LengthSqr() const;
	Vector3 Normalized() const;
	void Normalize();
	string ToString() const;

	// Arithmetic Operators
	Vector3 operator=(const Vector3& rhs);
	Vector3 operator+(const Vector3& rhs) const;
	Vector3 operator-(const Vector3& rhs) const;
	Vector3 operator*(const real rhs) const;
	Vector3 operator/(const real rhs) const;

	// Arithmetic Assignment Operators
	Vector3 operator+=(const Vector3& rhs);
	Vector3 operator-=(const Vector3& rhs);
	Vector3 operator*=(const real rhs);
	Vector3 operator/=(const real rhs);

	// Comparison Operators
	bool operator==(const Vector3& rhs) const;
	bool operator!=(const Vector3& rhs) const;

	// Static Functions
	static real Dot(const Vector3& lhs, const Vector3& rhs);
	static Vector3 Cross(const Vector3& lhs, const Vector3& rhs);
	static Vector3 Lerp(const Vector3& from, const Vector3& to, const float t);

	// Static Variables
	static Vector3 zero;
};

Vector3 operator*(const real lhs, const Vector3& rhs);