#pragma once

#include <string>

using namespace std;

struct Source
{
	string id;

	int arraySize;
	int count;
	int stride;

	float* floatArray;
};