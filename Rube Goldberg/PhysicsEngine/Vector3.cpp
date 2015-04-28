#include "Vector3.h"

#include <cmath>
#include <sstream>

// Static Variables
Vector3 Vector3::zero = Vector3();

// Constructors
Vector3::Vector3()
{
	x = y = z = 0;
}

Vector3::Vector3(real x)
{
	this->x = this->y = this->z = x;
}

Vector3::Vector3(real x, real y, real z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

// Functions
real Vector3::Length() const
{
	return sqrt(LengthSqr());
}

real Vector3::LengthSqr() const
{
	return x*x + y*y + z*z;
}

Vector3 Vector3::Normalized() const
{
	Vector3 newVec = *this;
	newVec.Normalize();
	return newVec;
}

void Vector3::Normalize()
{
	real mag = Length();
	x /= mag;
	y /= mag;
	z /= mag;
}

string Vector3::ToString() const
{
	ostringstream stream;
	stream << "X: " << x << " Y: " << y << " Z: " << z;
	return stream.str();
}

// Arithmetic Operator Overloads
Vector3 Vector3::operator=(const Vector3& rhs)
{
	this->x = rhs.x;
	this->y = rhs.y;
	this->z = rhs.z;

	return *this;
}

Vector3 Vector3::operator+(const Vector3& rhs) const
{
	Vector3 newVec = *this;

	newVec.x += rhs.x;
	newVec.y += rhs.y;
	newVec.z += rhs.z;

	return newVec;
}

Vector3 Vector3::operator-(const Vector3& rhs) const
{
	Vector3 newVec = *this;

	newVec.x -= rhs.x;
	newVec.y -= rhs.y;
	newVec.z -= rhs.z;

	return newVec;
}

Vector3 Vector3::operator*(const real rhs) const
{
	Vector3 newVec = *this;

	newVec.x *= rhs;
	newVec.y *= rhs;
	newVec.z *= rhs;

	return newVec;
}

Vector3 Vector3::operator/(const real rhs) const
{
	Vector3 newVec = *this;

	newVec.x /= rhs;
	newVec.y /= rhs;
	newVec.z /= rhs;

	return newVec;
}

// Arithmetic Assignment Operators
Vector3 Vector3::operator+=(const Vector3& rhs)
{
	this->x += rhs.x;
	this->y += rhs.y;
	this->z += rhs.z;

	return *this;
}

Vector3 Vector3::operator-=(const Vector3& rhs)
{
	this->x -= rhs.x;
	this->y -= rhs.y;
	this->z -= rhs.z;

	return *this;
}

Vector3 Vector3::operator*=(const real rhs)
{
	this->x *= rhs;
	this->y *= rhs;
	this->z *= rhs;

	return *this;
}

Vector3 Vector3::operator/=(const real rhs)
{
	this->x /= rhs;
	this->y /= rhs;
	this->z /= rhs;

	return *this;
}

// Comparison Operators
bool Vector3::operator==(const Vector3& rhs) const
{
	return (this->x == rhs.x &&
			this->y == rhs.y &&
			this->z == rhs.z);
}

bool Vector3::operator!=(const Vector3& rhs) const
{
	return !(*this == rhs);
}

real Vector3::operator[](unsigned index) const
{
	if (index == 0) return x;
	if (index == 1) return y;
	return z;
}

real& Vector3::operator[](unsigned index)
{
	if (index == 0) return x;
	if (index == 1) return y;
	return z;
}

// Static Functions
real Vector3::Dot(const Vector3& lhs, const Vector3& rhs)
{
	return (lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z);
}

Vector3 Vector3::Cross(const Vector3& lhs, const Vector3& rhs)
{
	return Vector3(lhs.y * rhs.z - lhs.z * rhs.y,
				   lhs.z * rhs.x - lhs.x * rhs.z,
				   lhs.x * rhs.y - lhs.y * rhs.x);
}

Vector3 Vector3::Lerp(const Vector3& from, const Vector3& to, const float t)
{
	return Vector3(from.x + (to.x - from.x) * t,
				   from.y + (to.y - from.y) * t,
				   from.z + (to.z - from.z) * t);
}

void Vector3::AddScaledVector(const Vector3& vector, real scale)
{
	x += vector.x * scale;
	y += vector.y * scale;
	z += vector.z * scale;
}

