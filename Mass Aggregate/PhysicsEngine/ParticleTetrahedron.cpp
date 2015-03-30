#include "ParticleTetrahedron.h"
#include "ParticleFactory.h"

ParticleTetrahedron::ParticleTetrahedron()
	:ParticleShape(4, 6)
{
}

void ParticleTetrahedron::init(const Vector3 &position)
{
	ParticleFactory factory;

	////// Particles //////
	mParticles[0] = factory.GetParticle(1, position + Vector3(0, 0, 0), Vector3(0, 0, 0), 0.99f);
	mParticles[1] = factory.GetParticle(1, position + Vector3(2, -4, 2), Vector3(0, 0, 0), 0.99f);
	mParticles[2] = factory.GetParticle(1, position + Vector3(-2, -4, 2), Vector3::zero, 0.99f);
	mParticles[3] = factory.GetParticle(1, position + Vector3(0, -4, -2), Vector3::zero, 0.99f);

	////// Rods //////
	// Base
	mRods[0]->mParticles[0] = mParticles[1];
	mRods[0]->mParticles[1] = mParticles[2];
	mRods[0]->mLength = (mParticles[1]->getPosition() - mParticles[2]->getPosition()).Length();

	mRods[1]->mParticles[0] = mParticles[1];
	mRods[1]->mParticles[1] = mParticles[3];
	mRods[1]->mLength = (mParticles[1]->getPosition() - mParticles[3]->getPosition()).Length();

	mRods[2]->mParticles[0] = mParticles[2];
	mRods[2]->mParticles[1] = mParticles[3];
	mRods[2]->mLength = (mParticles[2]->getPosition() - mParticles[3]->getPosition()).Length();

	// Sides
	mRods[3]->mParticles[0] = mParticles[0];
	mRods[3]->mParticles[1] = mParticles[1];
	mRods[3]->mLength = (mParticles[0]->getPosition() - mParticles[1]->getPosition()).Length();

	mRods[4]->mParticles[0] = mParticles[0];
	mRods[4]->mParticles[1] = mParticles[2];
	mRods[4]->mLength = (mParticles[0]->getPosition() - mParticles[2]->getPosition()).Length();

	mRods[5]->mParticles[0] = mParticles[0];
	mRods[5]->mParticles[1] = mParticles[3];
	mRods[5]->mLength = (mParticles[0]->getPosition() - mParticles[3]->getPosition()).Length();
}