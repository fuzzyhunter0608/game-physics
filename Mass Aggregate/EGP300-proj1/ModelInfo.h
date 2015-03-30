#pragma once

#include <vector>

#include "Geometry.h"

using namespace std;

struct ModelInfo
{
	vector<Geometry> geometries;
	int tris, quads;
	bool isTextured;
	string imgPath;
	string zUp;
};

