#pragma once

#include "tinyxml2.h"
#include "Model.h"
#include "ModelInfo.h"

class ModelFactory
{
public:
	Model* GetModel(string filename);

private:
	ModelInfo parseFile(string filename);

	// File parsing helper functions
	void floatArrayFromString(float* arr, int size, string theString);
	void intArrayFromString(int* arr, int size, string theString);
	Source getSourceById(vector<Source> sources, string id);
};

