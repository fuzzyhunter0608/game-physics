#pragma once

#include "Source.h"

struct Polylist
{
	string material;
	int count;

	Source vertexSource;
	Source normalSource;
	Source texCoordSource;

	int totalOffset;

	int vertexOffset;
	int normalOffset;
	int texCoordOffset;

	int vCountTotal;
	int* vCountArray;

	int* vertexIndexArray;
	int* normalIndexArray;
	int* texCoordIndexArray;
};