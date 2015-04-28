#include "gtest/gtest.h"

#include "PhysicsEngine/ParticleGravity.h"
#include "PhysicsEngine/ParticleFactory.h"
#include "PhysicsEngine/ParticleForceRegistry.h"

TEST(ParticleForceTest, TestNumber1)
{
	ParticleFactory factory;
	ParticleForceRegistry registry;

	Particle* particle1 = factory.GetParticle(2, Vector3(0), Vector3(0), 1);
	Particle* particle2 = factory.GetParticle(1, Vector3(0, 1, 0), Vector3(0), 1);

	ParticleGravity* gravity = new ParticleGravity(particle1);
	registry.add(particle2, gravity);

	registry.updateForces(1);

	const float G = 6.67 * pow(10, -11);
	ASSERT_EQ(particle2->getAccumForce(), Vector3(0, -2*G, 0));

	system("pause");

	delete particle1;
	delete particle2;
	delete gravity;
}