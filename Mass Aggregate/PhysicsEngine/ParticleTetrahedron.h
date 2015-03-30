#ifndef PARTICLE_TETRA_H
#define PARTICLE_TETRA_H

#include "ParticleShape.h"

class ParticleTetrahedron : public ParticleShape
{
public:
	ParticleTetrahedron();

	virtual void init(const Vector3 &position);
};

#endif