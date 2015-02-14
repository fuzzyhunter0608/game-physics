#pragma once

#include "GameObject.h"
#include "GLShaderManager.h"
#include "PlanetData.h"

#include <vector>

#include "PhysicsEngine/ParticleForceRegistry.h"
#include "PhysicsEngine/ParticleGravity.h"

class SolarSystem
{
public:
	SolarSystem();
	~SolarSystem();

	void LoadData(string filename);

	void Update(float deltaTime);
	void Draw(M3DMatrix44f &view, const M3DMatrix44f &projection, GLShaderManager &shaderManager);

	GameObject* GetSun() { return &mSun; }
	GameObject* GetMercury() { return &mMercury; }
	GameObject* GetVenus() { return &mVenus; }
	GameObject* GetEarth() { return &mEarth; }
	GameObject* GetMoon() { return &mMoon; }
	GameObject* GetMars() { return &mMars; }
	GameObject* GetJupiter() { return &mJupiter; }
	GameObject* GetSaturn() { return &mSaturn; }
	GameObject* GetUranus() { return &mUranus; }
	GameObject* GetNeptune() { return &mNeptune; }

private:
	GLShaderManager mShaderManager;
	ParticleForceRegistry mRegistry;
	real mTimer;

	std::vector<PlanetData> mData;

	GameObject mSun,
		mMercury,
		mVenus,
		mEarth,
		mMoon,
		mMars,
		mJupiter,
		mSaturn,
		mUranus,
		mNeptune;

	Model* mModel;

	Particle* mSunParticle,
		*mMercuryParticle,
		*mVenusParticle,
		*mEarthParticle,
		*mMoonParticle,
		*mMarsParticle,
		*mJupiterParticle,
		*mSaturnParticle,
		*mUranusParticle,
		*mNeptuneParticle;

	ParticleGravity *mMercuryGravity,
		*mVenusGravity,
		*mEarthGravity,
		*mMoonGravity,
		*mMarsGravity,
		*mJupiterGravity,
		*mSaturnGravity,
		*mUranusGravity,
		*mNeptuneGravity;
};

