#ifndef PARTICLE_CUBE_H
#define PARTICLE_CUBE_H

#include "ParticleShape.h"

class ParticleCube : public ParticleShape
{
public:
	ParticleCube();

	virtual void init(const Vector3 &position);
};

#endif