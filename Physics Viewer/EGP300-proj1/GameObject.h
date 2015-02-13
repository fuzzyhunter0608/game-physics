#pragma once

#include "PhysicsEngine/Particle.h"
#include "Model.h"

class GameObject
{
public:
	GameObject();
	~GameObject();

	void SetPhysics(Particle* physics) { mPhysicsEngine = physics; }
	void SetModel(Model* model) { mModel = model; }
	void SetScale(const Vector3 &scale) { mScale = scale; }
	void SetScale(const real x, const real y, const real z) { mScale.x = x; mScale.y = y; mScale.z = z; }

	void Update(float deltaTime);
	void GetModelMatrix(M3DMatrix44f &matrix);
	void Draw();

private:
	Particle* mPhysicsEngine;
	Model* mModel;

	Vector3 mScale;
	Vector3 mDisplayPosition;
};

