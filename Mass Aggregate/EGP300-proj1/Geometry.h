#pragma once

#include <vector>

#include "Source.h"
#include "Polylist.h"

struct Geometry
{
	string id;

	vector<Source> sources;
	vector<Polylist> polylists;
};