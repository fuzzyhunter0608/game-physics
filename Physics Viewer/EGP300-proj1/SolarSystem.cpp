#include "SolarSystem.h"

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

#include "ModelFactory.h"
#include "PhysicsEngine/ParticleFactory.h"
#include "Color.h"
#include "PlanetData.h"

SolarSystem::SolarSystem()
{
	mTimer = 0;
	mShaderManager.InitializeStockShaders();

	ModelFactory modelFactory;
	mModel = modelFactory.GetModel("sphere.dae");

	// Set Model
	mSun.SetModel(mModel);
	mMercury.SetModel(mModel);
	mVenus.SetModel(mModel);
	mEarth.SetModel(mModel);
	mMars.SetModel(mModel);
	mJupiter.SetModel(mModel);
	mSaturn.SetModel(mModel);
	mUranus.SetModel(mModel);
	mNeptune.SetModel(mModel);

	// Load in planetary data
	LoadData("solarsystem.txt");

	// Create Particles
	ParticleFactory particleFactory;
	mSunParticle = particleFactory.GetParticle(mData[0].mass, Vector3::zero, Vector3::zero, 1);
	mMercuryParticle = particleFactory.GetParticle(mData[1].mass, Vector3(mData[1].distance, 0, 0), Vector3(0, mData[1].velocity, 0), 1);
	mVenusParticle = particleFactory.GetParticle(mData[2].mass, Vector3(mData[2].distance, 0, 0), Vector3(0, mData[2].velocity, 0), 1);
	mEarthParticle = particleFactory.GetParticle(mData[3].mass, Vector3(mData[3].distance, 0, 0), Vector3(0, mData[3].velocity, 0), 1);
	mMarsParticle = particleFactory.GetParticle(mData[4].mass, Vector3(mData[4].distance, 0, 0), Vector3(0, mData[4].velocity, 0), 1);
	mJupiterParticle = particleFactory.GetParticle(mData[5].mass, Vector3(mData[5].distance, 0, 0), Vector3(0, mData[5].velocity, 0), 1);
	mSaturnParticle = particleFactory.GetParticle(mData[6].mass, Vector3(mData[6].distance, 0, 0), Vector3(0, mData[6].velocity, 0), 1);
	mUranusParticle = particleFactory.GetParticle(mData[7].mass, Vector3(mData[7].distance, 0, 0), Vector3(0, mData[7].velocity, 0), 1);
	mNeptuneParticle = particleFactory.GetParticle(mData[8].mass, Vector3(mData[8].distance, 0, 0), Vector3(0, mData[8].velocity, 0), 1);

	// Set Scale
	mSun.SetScale(Vector3(10));
	mMercury.SetScale(Vector3(1));
	mVenus.SetScale(Vector3(1));
	mEarth.SetScale(Vector3(1));
	mMars.SetScale(Vector3(1));
	mJupiter.SetScale(Vector3(5));
	mSaturn.SetScale(Vector3(1));
	mUranus.SetScale(Vector3(1));
	mNeptune.SetScale(Vector3(1));

	// Set Particle
	mSun.SetPhysics(mSunParticle);
	mMercury.SetPhysics(mMercuryParticle);
	mVenus.SetPhysics(mVenusParticle);
	mEarth.SetPhysics(mEarthParticle);
	mMars.SetPhysics(mMarsParticle);
	mJupiter.SetPhysics(mJupiterParticle);
	mSaturn.SetPhysics(mSaturnParticle);
	mUranus.SetPhysics(mUranusParticle);
	mNeptune.SetPhysics(mNeptuneParticle);

	// Gravity Generators
	mMercuryGravity = new ParticleGravity(mSunParticle);
	mRegistry.add(mMercuryParticle, mMercuryGravity);
	mVenusGravity = new ParticleGravity(mSunParticle);
	mRegistry.add(mVenusParticle, mVenusGravity);
	mEarthGravity = new ParticleGravity(mSunParticle);
	mRegistry.add(mEarthParticle, mEarthGravity);
	mMarsGravity = new ParticleGravity(mSunParticle);
	mRegistry.add(mMarsParticle, mMarsGravity);
	mJupiterGravity = new ParticleGravity(mSunParticle);
	mRegistry.add(mJupiterParticle, mJupiterGravity);
	mSaturnGravity = new ParticleGravity(mSunParticle);
	mRegistry.add(mSaturnParticle, mSaturnGravity);
	mUranusGravity = new ParticleGravity(mSunParticle);
	mRegistry.add(mUranusParticle, mUranusGravity);
	mNeptuneGravity = new ParticleGravity(mSunParticle);
	mRegistry.add(mNeptuneParticle, mNeptuneGravity);
}

SolarSystem::~SolarSystem()
{
	delete mModel;

	delete mSunParticle;
	delete mMercuryParticle;
	delete mVenusParticle;
	delete mEarthParticle;
	//delete mMoonParticle;
	delete mMarsParticle;
	delete mJupiterParticle;
	delete mSaturnParticle;
	delete mUranusParticle;
	delete mNeptuneParticle;

	delete mMercuryGravity;
	delete mVenusGravity;
	delete mEarthGravity;
	//delete mMoonGravity;
	delete mMarsGravity;
	delete mJupiterGravity;
	delete mSaturnGravity;
	delete mUranusGravity;
	delete mNeptuneGravity;
}

void SolarSystem::Update(float deltaTime)
{
	auto newTime = deltaTime * 60 * 60 * 60 * 24;

	mRegistry.updateForces(newTime);

	//mSun.Update(newTime);
	mMercury.Update(newTime);
	mVenus.Update(newTime);
	mEarth.Update(newTime);
	mMars.Update(newTime);
	mJupiter.Update(newTime);
	mSaturn.Update(newTime);
	mUranus.Update(newTime);
	mNeptune.Update(newTime);

	mTimer += deltaTime;
	if (mTimer >= 0.1)
	{
		mTimer = 0;

		//mMercuryParticle->setVelocity(mMercuryParticle->getVelocity().Normalized() * mData[1].velocity);
		//mEarthParticle->setVelocity(mEarthParticle->getVelocity().Normalized() * mData[2].velocity);
	}
}

void SolarSystem::Draw(M3DMatrix44f &view, const M3DMatrix44f &projection, GLShaderManager &shaderManager)
{

}

void SolarSystem::LoadData(string filename)
{
	ifstream file;
	file.open("solarsystem.txt");

	std::string line;
	while (std::getline(file, line))
	{
		if (line == "") continue;
		if (line[0] == '#') continue;

		PlanetData data;

		//Split line by spaces
		std::istringstream stream(line);
		std::istream_iterator<std::string> begin(stream), end;
		std::vector<std::string> tokens(begin, end);
		
		//Parse words as floats
		data.mass = std::stof(tokens[0]);
		data.distance = std::stof(tokens[1]);
		data.velocity = std::stof(tokens[2]);

		mData.push_back(data);
	}

	file.close();
}
