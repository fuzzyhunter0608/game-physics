#pragma once

#include <string>

using namespace std;

class Debug
{
public:
	Debug();
	~Debug();

	static void Log(string text);
};

