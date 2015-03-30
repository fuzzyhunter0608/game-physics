#pragma once

#include "GL/glui.h"
#include <string>

#include "PhysicsEngine/Vector3.h"

using namespace std;

class PlanetGUI
{
public:
	void setMassText(float mass) { mpMassText->set_text(to_string(mass).c_str()); }
	void setVelocityText(Vector3 velocity) { mpVelocityText->set_text(velocity.ToString().c_str()); }

	GLUI_StaticText* mpMassText;
	GLUI_StaticText* mpVelocityText;
};

