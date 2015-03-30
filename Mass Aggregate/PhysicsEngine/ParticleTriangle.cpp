#include "ParticleTriangle.h"
#include "ParticleFactory.h"

ParticleTriangle::ParticleTriangle()
	:ParticleShape(3, 3)
{

}

void ParticleTriangle::init(const Vector3 &position)
{
	ParticleFactory factory;

	////// Particles ///////
	mParticles[0] = factory.GetParticle(1, position + Vector3(0, 1, 0), Vector3::zero, 0.99f);
	mParticles[1] = factory.GetParticle(1, position + Vector3(-2, -2, 0), Vector3::zero, 0.99f);
	mParticles[2] = factory.GetParticle(1, position + Vector3(2, -2, 0), Vector3::zero, 0.99f);

	////// Rods //////
	mRods[0]->mParticles[0] = mParticles[0];
	mRods[0]->mParticles[1] = mParticles[1];
	mRods[0]->mLength = (mParticles[0]->getPosition() - mParticles[1]->getPosition()).Length();

	mRods[1]->mParticles[0] = mParticles[0];
	mRods[1]->mParticles[1] = mParticles[2];
	mRods[1]->mLength = (mParticles[0]->getPosition() - mParticles[2]->getPosition()).Length();

	mRods[2]->mParticles[0] = mParticles[1];
	mRods[2]->mParticles[1] = mParticles[2];
	mRods[2]->mLength = (mParticles[1]->getPosition() - mParticles[2]->getPosition()).Length();
}