#pragma once

#include "Particle.h"

enum Planets
{
	SUN,
	MERCURY,
	VENUS,
	EARTH,
	MOON,
	MARS,
	JUPITER,
	SATURN,
	URANUS,
	NEPTUNE
};

class ParticleFactory
{
public:
	Particle* GetParticle(Planets planet);
};

