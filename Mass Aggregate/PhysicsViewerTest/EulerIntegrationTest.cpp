#include "gtest/gtest.h"

#include "PhysicsEngine/Particle.h"
#include "PhysicsEngine/ParticleFactory.h"

TEST(EulerTest, TestNumber1)
{
	ParticleFactory factory;
	Particle* particle1 = factory.GetParticle(1, Vector3::zero, Vector3::zero, 1);

	particle1->addForce(Vector3(0, 1, 0));
	particle1->integrate(1);

	ASSERT_EQ(particle1->getVelocity(), Vector3(0, 1, 0));
	system("pause");

	delete particle1;
}