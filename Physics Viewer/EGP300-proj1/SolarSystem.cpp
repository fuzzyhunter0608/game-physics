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
	mShaderManager.InitializeStockShaders();

	ModelFactory modelFactory;
	mModel = modelFactory.GetModel("sphere.dae");

	mSun.SetModel(mModel);
	mMercury.SetModel(mModel);
	mEarth.SetModel(mModel);

	//Load in planetary data
	LoadData("solarsystem.txt");

	ParticleFactory particleFactory;
	mSunParticle = particleFactory.GetParticle(mData[0].mass, Vector3::zero, Vector3::zero, 1);
	mMercuryParticle = particleFactory.GetParticle(mData[1].mass, Vector3(mData[1].distance, 0, 0), Vector3(0, mData[1].velocity, 0), 1);
	mEarthParticle = particleFactory.GetParticle(mData[2].mass, Vector3(mData[2].distance, 0, 0), Vector3(0, mData[2].velocity, 0), 1);

	mSun.SetScale(Vector3(10));
	mMercury.SetScale(Vector3(1));
	mEarth.SetScale(Vector3(1));

	mSun.SetPhysics(mSunParticle);
	mMercury.SetPhysics(mMercuryParticle);
	mEarth.SetPhysics(mEarthParticle);

	mMercuryGravity = new ParticleGravity(mSunParticle);
	mRegistry.add(mMercuryParticle, mMercuryGravity);

	mEarthGravity = new ParticleGravity(mSunParticle);
	mRegistry.add(mEarthParticle, mEarthGravity); // Sun's gravity affecting Earth
}

SolarSystem::~SolarSystem()
{
	delete mModel;

	delete mSunParticle;
	delete mMercuryParticle;
	//delete mVenusParticle;
	delete mEarthParticle;
	//delete mMoonParticle;
	//delete mMarsParticle;
	//delete mJupiterParticle;
	//delete mSaturnParticle;
	//delete mUranusParticle;
	//delete mNeptuneParticle;

	delete mMercuryGravity;
	//delete mVenusGravity;
	delete mEarthGravity;
	//delete mMoonGravity;
	//delete mMarsGravity;
	//delete mJupiterGravity;
	//delete mSaturnGravity;
	//delete mUranusGravity;
	//delete mNeptuneGravity;
}

void SolarSystem::Update(float deltaTime)
{
	auto newTime = deltaTime * 60 * 60 * 60 * 24;

	mRegistry.updateForces(newTime);

	//mSun.Update(newTime);
	mMercury.Update(newTime);
	mEarth.Update(newTime);
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
