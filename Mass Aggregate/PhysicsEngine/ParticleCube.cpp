#include "ParticleCube.h"

#include "ParticleFactory.h"

ParticleCube::ParticleCube()
	:ParticleShape(8, 16)
{

}

void ParticleCube::init(const Vector3 &position)
{
	ParticleFactory factory;

	////// Particles //////
	mParticles[0] = factory.GetParticle(1, position + Vector3(0, 0, 0), Vector3(0, 0, 0), 0.99f);
	mParticles[1] = factory.GetParticle(1, position + Vector3(5, 0, 0), Vector3::zero, 0.99f);
	mParticles[2] = factory.GetParticle(1, position + Vector3(0, -5, 0), Vector3::zero, 0.99f);
	mParticles[3] = factory.GetParticle(1, position + Vector3(5, -5, 0), Vector3::zero, 0.99f);
	mParticles[4] = factory.GetParticle(1, position + Vector3(0, 0, -5), Vector3::zero, 0.99f);
	mParticles[5] = factory.GetParticle(1, position + Vector3(5, 0, -5), Vector3::zero, 0.99f);
	mParticles[6] = factory.GetParticle(1, position + Vector3(0, -5, -5), Vector3::zero, 0.99f);
	mParticles[7] = factory.GetParticle(1, position + Vector3(5, -5, -5), Vector3::zero, 0.99f);

	////// Rods //////
	// Front face
	mRods[0]->mParticles[0] = mParticles[0];
	mRods[0]->mParticles[1] = mParticles[1];
	mRods[0]->mLength = 5;

	mRods[1]->mParticles[0] = mParticles[0];
	mRods[1]->mParticles[1] = mParticles[2];
	mRods[1]->mLength = 5;

	mRods[2]->mParticles[0] = mParticles[1];
	mRods[2]->mParticles[1] = mParticles[3];
	mRods[2]->mLength = 5;

	mRods[3]->mParticles[0] = mParticles[2];
	mRods[3]->mParticles[1] = mParticles[3];
	mRods[3]->mLength = 5;

	// Back face
	mRods[4]->mParticles[0] = mParticles[4];
	mRods[4]->mParticles[1] = mParticles[5];
	mRods[4]->mLength = 5;

	mRods[5]->mParticles[0] = mParticles[4];
	mRods[5]->mParticles[1] = mParticles[6];
	mRods[5]->mLength = 5;

	mRods[6]->mParticles[0] = mParticles[5];
	mRods[6]->mParticles[1] = mParticles[7];
	mRods[6]->mLength = 5;

	mRods[7]->mParticles[0] = mParticles[6];
	mRods[7]->mParticles[1] = mParticles[7];
	mRods[7]->mLength = 5;

	// Sides
	mRods[8]->mParticles[0] = mParticles[0];
	mRods[8]->mParticles[1] = mParticles[4];
	mRods[8]->mLength = 5;

	mRods[9]->mParticles[0] = mParticles[1];
	mRods[9]->mParticles[1] = mParticles[5];
	mRods[9]->mLength = 5;

	mRods[10]->mParticles[0] = mParticles[2];
	mRods[10]->mParticles[1] = mParticles[6];
	mRods[10]->mLength = 5;

	mRods[11]->mParticles[0] = mParticles[3];
	mRods[11]->mParticles[1] = mParticles[7];
	mRods[11]->mLength = 5;

	// Diagonals
	mRods[12]->mParticles[0] = mParticles[0];
	mRods[12]->mParticles[1] = mParticles[7];
	mRods[12]->mLength = 5 * sqrt(3);
	
	mRods[13]->mParticles[0] = mParticles[1];
	mRods[13]->mParticles[1] = mParticles[6];
	mRods[13]->mLength = 5 * sqrt(3);

	mRods[14]->mParticles[0] = mParticles[2];
	mRods[14]->mParticles[1] = mParticles[5];
	mRods[14]->mLength = 5 * sqrt(3);

	mRods[15]->mParticles[0] = mParticles[3];
	mRods[15]->mParticles[1] = mParticles[4];
	mRods[15]->mLength = 5 * sqrt(3);
}