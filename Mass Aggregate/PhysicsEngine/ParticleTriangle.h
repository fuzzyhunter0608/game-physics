#ifndef PARTICLE_TRIANGLE_H
#define PARTICLE_TRIANGLE_H

#include "ParticleShape.h"

class ParticleTriangle : public ParticleShape
{
public:
	ParticleTriangle();
	
	virtual void init(const Vector3 &position);
};

#endif