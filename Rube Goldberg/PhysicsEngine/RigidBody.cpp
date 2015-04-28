#include "RigidBody.h"

static inline void calculateTransformMatrix(Matrix4 &transformMatrix, const Vector3 &position, const Quaternion &orientation)
{
	transformMatrix.data[0] = 1 - (2 * orientation.j * orientation.j) - (2 * orientation.k * orientation.k);
	transformMatrix.data[1] = (2 * orientation.i * orientation.j) - (2 * orientation.r * orientation.k);
	transformMatrix.data[2] = (2 * orientation.i * orientation.k) + (2 * orientation.r * orientation.j);
	transformMatrix.data[3] = position.x;

	transformMatrix.data[4] = (2 * orientation.i * orientation.j) + (2 * orientation.r * orientation.k);
	transformMatrix.data[5] = 1 - (2 * orientation.i * orientation.i) - (2 * orientation.k * orientation.k);
	transformMatrix.data[6] = (2 * orientation.j * orientation.k) - (2 * orientation.r * orientation.i);
	transformMatrix.data[7] = position.y;

	transformMatrix.data[8] = (2 * orientation.i * orientation.k) - (2 * orientation.r * orientation.j);
	transformMatrix.data[9] = (2 * orientation.j * orientation.k) - (2 * orientation.r * orientation.i);
	transformMatrix.data[10] = 1 - (2 * orientation.i * orientation.i) - (2 * orientation.j * orientation.j);
	transformMatrix.data[11] = position.z;
}

static inline void transformInertiaTensor(Matrix3 &iitWorld, const Quaternion &q, const Matrix3 iitBody, const Matrix4 &rotmat)
{
	real t4 = rotmat.data[0] * iitBody.data[0] +
		rotmat.data[1] * iitBody.data[3] +
		rotmat.data[2] * iitBody.data[6];
	real t9 = rotmat.data[0] * iitBody.data[1] +
		rotmat.data[1] * iitBody.data[4] +
		rotmat.data[2] * iitBody.data[7];
	real t14 = rotmat.data[0] * iitBody.data[2] +
		rotmat.data[1] * iitBody.data[5] +
		rotmat.data[2] * iitBody.data[8];
	real t28 = rotmat.data[4] * iitBody.data[0] +
		rotmat.data[5] * iitBody.data[3] +
		rotmat.data[6] * iitBody.data[6];
	real t33 = rotmat.data[4] * iitBody.data[1] +
		rotmat.data[5] * iitBody.data[4] +
		rotmat.data[6] * iitBody.data[7];
	real t38 = rotmat.data[4] * iitBody.data[2] +
		rotmat.data[5] * iitBody.data[5] +
		rotmat.data[6] * iitBody.data[8];
	real t52 = rotmat.data[8] * iitBody.data[0] +
		rotmat.data[9] * iitBody.data[3] +
		rotmat.data[10] * iitBody.data[6];
	real t57 = rotmat.data[8] * iitBody.data[1] +
		rotmat.data[9] * iitBody.data[4] +
		rotmat.data[10] * iitBody.data[7];
	real t62 = rotmat.data[8] * iitBody.data[2] +
		rotmat.data[9] * iitBody.data[5] +
		rotmat.data[10] * iitBody.data[8];

	iitWorld.data[0] = t4*rotmat.data[0] +
		t9*rotmat.data[1] +
		t14*rotmat.data[2];
	iitWorld.data[1] = t4*rotmat.data[4] +
		t9*rotmat.data[5] +
		t14*rotmat.data[6];
	iitWorld.data[2] = t4*rotmat.data[8] +
		t9*rotmat.data[9] +
		t14*rotmat.data[10];
	iitWorld.data[3] = t28*rotmat.data[0] +
		t33*rotmat.data[1] +
		t38*rotmat.data[2];
	iitWorld.data[4] = t28*rotmat.data[4] +
		t33*rotmat.data[5] +
		t38*rotmat.data[6];
	iitWorld.data[5] = t28*rotmat.data[8] +
		t33*rotmat.data[9] +
		t38*rotmat.data[10];
	iitWorld.data[6] = t52*rotmat.data[0] +
		t57*rotmat.data[1] +
		t62*rotmat.data[2];
	iitWorld.data[7] = t52*rotmat.data[4] +
		t57*rotmat.data[5] +
		t62*rotmat.data[6];
	iitWorld.data[8] = t52*rotmat.data[8] +
		t57*rotmat.data[9] +
		t62*rotmat.data[10];
}

RigidBody::RigidBody()
{
}


RigidBody::~RigidBody()
{
}

void RigidBody::GetGLTransform(float matrix[16]) const
{
	matrix[0] = (float)mTransformMatrix.data[0];
	matrix[1] = (float)mTransformMatrix.data[4];
	matrix[2] = (float)mTransformMatrix.data[8];
	matrix[3] = 0;

	matrix[4] = (float)mTransformMatrix.data[1];
	matrix[5] = (float)mTransformMatrix.data[5];
	matrix[6] = (float)mTransformMatrix.data[9];
	matrix[7] = 0;

	matrix[8] = (float)mTransformMatrix.data[2];
	matrix[9] = (float)mTransformMatrix.data[6];
	matrix[10] = (float)mTransformMatrix.data[10];
	matrix[11] = 0;

	matrix[12] = (float)mTransformMatrix.data[3];
	matrix[13] = (float)mTransformMatrix.data[7];
	matrix[14] = (float)mTransformMatrix.data[11];
	matrix[15] = 1;
}

Vector3 RigidBody::GetPointInLocalSpace(const Vector3& point) const
{
	return mTransformMatrix.TransformInverse(point);
}

Vector3 RigidBody::GetPointInWorldSpace(const Vector3& point) const
{
	return mTransformMatrix.Transform(point);
}

Vector3 RigidBody::GetDirectionInLocalSpace(const Vector3& direction) const
{
	return mTransformMatrix.TransformInverseDirection(direction);
}

Vector3 RigidBody::GetDirectionInWorldSpace(const Vector3& direction) const
{
	return mTransformMatrix.TransformDirection(direction);
}

void RigidBody::SetAwake(bool awake)
{
	if (awake)
	{
		mIsAwake = true;

		// Add bit of motion
		mMotion += (real)0.3 * 2.0f;
	}
	else
	{
		mIsAwake = false;
		mForceAccum = Vector3::zero;
		mTorqueAccum = Vector3::zero;
	}
}

void RigidBody::SetCanSleep(bool canSleep)
{
	mCanSleep = canSleep;

	if (!canSleep && !mIsAwake) SetAwake();
}

void RigidBody::CalculateDerivedData()
{
	mOrientation.Normalize();

	// Calculate transformation matrix for body
	calculateTransformMatrix(mTransformMatrix, mPosition, mOrientation);

	// Calculate inertia tensor in world space
	transformInertiaTensor(mInverseInertiaTensorWorld, mOrientation, mInverseInertiaTensor, mTransformMatrix);
}

void RigidBody::AddForce(const Vector3& force)
{
	mForceAccum += force;
	mIsAwake = true;
}

void RigidBody::AddForceAtPoint(const Vector3& force, const Vector3& point)
{
	// Convert to coords relative to center of mass
	Vector3 pt = point;
	pt -= mPosition;

	mForceAccum += force;
	mTorqueAccum += Vector3::Cross(pt, force);

	mIsAwake = true;
}

void RigidBody::AddForceAtBodyPoint(const Vector3& force, const Vector3& point)
{
	// Convert to coords relative to center of mass
	Vector3 pt = GetPointInWorldSpace(point);
	AddForceAtPoint(force, pt);

	mIsAwake = true;
}

void RigidBody::AddTorque(const Vector3& torque)
{
	mTorqueAccum += torque;
	mIsAwake = true;
}

void RigidBody::Integrate(real duration)
{
	// Calculate linear acceleration from force inputs
	mLastFrameAcceleration = mAcceleration;
	mLastFrameAcceleration.AddScaledVector(mForceAccum, mInverseMass);

	// Calculate angular acceleration from torque inputs
	Vector3 angularAcceleration = mInverseInertiaTensorWorld.transform(mTorqueAccum);

	// Adjust velocities
	// Update linear velocity from acceleration and impulse
	mVelocity.AddScaledVector(mLastFrameAcceleration, duration);

	// Update angular velocity from acceleration and impulse
	mRotation.AddScaledVector(angularAcceleration, duration);

	// Impose drag
	mVelocity *= pow(mLinearDamping, duration);
	mRotation *= pow(mAngularDamping, duration);

	// Adjust positions
	// Linear position
	mPosition.AddScaledVector(mVelocity, duration);

	// Angular position
	mOrientation.AddScaledVector(mRotation, duration);

	// Normalize the orientation and update matrices with new position and orientation
	CalculateDerivedData();

	// Clear accumulators
	ClearAccumulators();
}

void RigidBody::ClearAccumulators()
{
	mForceAccum = Vector3::zero;
	mTorqueAccum = Vector3::zero;
}