#include "GameObject.h"
#include "Debug.h"

GameObject::GameObject()
{
	mScale = Vector3(1);
}

GameObject::~GameObject()
{
}

void GameObject::Update(float deltaTime)
{
	//mModel->rotateY(24 * deltaTime);

	//mPhysicsEngine->integrate(deltaTime);
}

void GameObject::GetModelMatrix(M3DMatrix44f &matrix)
{
	mModel->getModelMatrix(matrix, mPhysicsEngine->getPosition(), mScale);
}

void GameObject::Draw()
{
	mModel->draw();
}