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

	mPhysicsEngine->integrate(deltaTime);

	if (mPhysicsEngine->getPosition() != Vector3::zero)
		mDisplayPosition = mPhysicsEngine->getPosition() / 5000000000;
}

void GameObject::GetModelMatrix(M3DMatrix44f &matrix)
{
	mModel->getModelMatrix(matrix, mDisplayPosition, mScale);
}

void GameObject::Draw()
{
	mModel->draw();
}