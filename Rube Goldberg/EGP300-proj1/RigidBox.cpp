#include "RigidBox.h"

#include <GLTools.h>
#include <GLShaderManager.h>
#include <freeglut.h>
#include "ModelFactory.h"
#include "Model.h""

RigidBox::RigidBox()
{
	ModelFactory factory;

	box = new Block();
	model = factory.GetModel("untitled.dae");
}

RigidBox::~RigidBox()
{
	delete box;
}



void RigidBox::Draw()
{
	model->draw();
}