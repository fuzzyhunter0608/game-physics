#pragma once

#include "Moveable.h"

#include <vector>

#include "Geometry.h"
#include "ModelInfo.h"

using namespace std;

class Model : public Moveable
{
public:
	Model(void);
	~Model(void);

	void load(vector<Geometry> &geometries, int tris, int quads, string orientation, bool isTextured);
	void load(ModelInfo info);
	void setup();
	void cleanup();

	void getModelMatrix(M3DMatrix44f &matrix, Vector3 position, Vector3 scale);
	void draw();

private:
	vector<Geometry> mGeometries;
	int mTris;
	int mQuads;

	bool mIsTextured;
	string mOrientation;

	GLBatch mTriBatch;
	GLBatch mQuadBatch;
};

