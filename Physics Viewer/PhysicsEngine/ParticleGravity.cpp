#include "ParticleGravity.h"

#include <iostream>

ParticleGravity::ParticleGravity(Particle* attractionParticle)
{
	mpAttractionParticle = attractionParticle;
}

void ParticleGravity::updateForce(Particle* particle, real duration)
{
	float gravConst = 6.67 * pow(10, -11);

	Vector3 direction = (mpAttractionParticle->getPosition() - particle->getPosition()).Normalized();
	real distanceSqr = (mpAttractionParticle->getPosition() - particle->getPosition()).LengthSqr();

	real scalar = gravConst * particle->getMass() * mpAttractionParticle->getMass() / distanceSqr;
	direction *= scalar;

	std::cout << direction.ToString() << endl;

	particle->addForce(direction);
}