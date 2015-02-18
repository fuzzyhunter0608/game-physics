#include <GLTools.h>
#include <GLShaderManager.h>
#include <GL/glui.h>
#include <GLFrustum.h>
#include <string>
#include <iostream>

#include "Camera.h"
#include "Model.h"
#include "ModelFactory.h"
#include "PhysicsEngine/ParticleFactory.h"
#include "PhysicsEngine/ParticleForceRegistry.h"
#include "PhysicsEngine/ParticleGravity.h"
#include "GameObject.h"
#include "SolarSystem.h"
#include "Color.h"
#include "PlanetGUI.h"

using namespace std;

bool LoadTGATexture(const char *szFileName, GLenum minFilter, GLenum magFilter, GLenum wrapMode);

float colorWhite[]  = {1.0f,1.0f,1.0f,1.0f};
float colorYellow[] = { 1.0f, 1.0f, 0.0f, 1.0f };
float colorBlue[] = { 0.0f, 0.0f, 1.0f, 1.0f };

GLShaderManager	shaderManager;
M3DMatrix44f	mvpMatrix;
GLFrustum		viewFrustum;
GLint			width, height;

Camera camera;
Model* planetModel;
Particle* sunParticle, *earthParticle;
GameObject sunObject, earthObject;
SolarSystem* solarSystem;

GLUI* glui;

ModelFactory modelFactory;
ParticleFactory particleFactory;

ParticleForceRegistry registry;
ParticleGravity* gravityGenerator;

PlanetGUI mercuryGUI, venusGUI, earthGUI, marsGUI, jupiterGUI, saturnGUI, uranusGUI, neptuneGUI, moonGUI;

string filename;

int windowId;

int oldTimeFromStart;

void ChangeSize(int w, int h)
{
	width = w;
	height = h;

	glViewport(0,0,width, height);

	viewFrustum.SetPerspective(35.0f, (float)(width/height), 1.0f, 10000.0f);
}

void setup()
{
}

void myInit()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	shaderManager.InitializeStockShaders();

	solarSystem = new SolarSystem();

	//Projection
	viewFrustum.SetPerspective(35.0f, (float)(width/height), 1.0f, 1000.0f);

	glEnable(GL_DEPTH_TEST);

	setup();
}

void DrawDebugText()
{
	
}

void RenderScene(void)
{
	//Swap Colors
	GLfloat vColor[]=  { 1.0f, 1.0f, 1.0f, 1.0f };
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	float posLight1[] = { 1.0f, 1.0f, 1.0f, 0.0f };

	M3DMatrix44f mModel, mView, mModelView;

	DrawDebugText();

	//View
	camera.getViewMatrix(mView);

	//Sun
	solarSystem->GetSun()->GetModelMatrix(mModel);
	m3dMatrixMultiply44(mModelView, mView, mModel);
	shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, mModelView, viewFrustum.GetProjectionMatrix(), posLight1, Color::Yellow.GetColor());
	solarSystem->GetSun()->Draw();

	//Mercury
	solarSystem->GetMercury()->GetModelMatrix(mModel);
	m3dMatrixMultiply44(mModelView, mView, mModel);
	shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, mModelView, viewFrustum.GetProjectionMatrix(), posLight1, Color::Gray.GetColor());
	solarSystem->GetMercury()->Draw();

	//Venus
	solarSystem->GetVenus()->GetModelMatrix(mModel);
	m3dMatrixMultiply44(mModelView, mView, mModel);
	shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, mModelView, viewFrustum.GetProjectionMatrix(), posLight1, Color::Tan.GetColor());
	solarSystem->GetVenus()->Draw();

	//Earth
	solarSystem->GetEarth()->GetModelMatrix(mModel);
	m3dMatrixMultiply44(mModelView, mView, mModel);
	shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, mModelView, viewFrustum.GetProjectionMatrix(), posLight1, Color::Blue.GetColor());
	solarSystem->GetEarth()->Draw();

	//Mars
	solarSystem->GetMars()->GetModelMatrix(mModel);
	m3dMatrixMultiply44(mModelView, mView, mModel);
	shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, mModelView, viewFrustum.GetProjectionMatrix(), posLight1, Color::Red.GetColor());
	solarSystem->GetMars()->Draw();

	//Jupiter
	solarSystem->GetJupiter()->GetModelMatrix(mModel);
	m3dMatrixMultiply44(mModelView, mView, mModel);
	shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, mModelView, viewFrustum.GetProjectionMatrix(), posLight1, Color::Gray.GetColor());
	solarSystem->GetJupiter()->Draw();

	//Saturn
	solarSystem->GetSaturn()->GetModelMatrix(mModel);
	m3dMatrixMultiply44(mModelView, mView, mModel);
	shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, mModelView, viewFrustum.GetProjectionMatrix(), posLight1, Color::Tan.GetColor());
	solarSystem->GetSaturn()->Draw();

	//Uranus
	solarSystem->GetUranus()->GetModelMatrix(mModel);
	m3dMatrixMultiply44(mModelView, mView, mModel);
	shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, mModelView, viewFrustum.GetProjectionMatrix(), posLight1, Color::Blue.GetColor());
	solarSystem->GetUranus()->Draw();

	//Neptune
	solarSystem->GetNeptune()->GetModelMatrix(mModel);
	m3dMatrixMultiply44(mModelView, mView, mModel);
	shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, mModelView, viewFrustum.GetProjectionMatrix(), posLight1, Color::Red.GetColor());
	solarSystem->GetNeptune()->Draw();

	//Moon
	solarSystem->GetMoon()->GetModelMatrix(mModel);
	m3dMatrixMultiply44(mModelView, mView, mModel);
	shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, mModelView, viewFrustum.GetProjectionMatrix(), posLight1, Color::Gray.GetColor());
	solarSystem->GetMoon()->Draw();

	glutSwapBuffers();
}

void Keys(unsigned char key, int x, int y)
{
	if(key == 27) //Escape
		exit(0);

	if((key == 'R') || (key == 'r'))
	{
		glutWarpPointer(width/2, height/2);

		camera.reset();
		solarSystem->Reset();
	}

	if ((key == 'P') || (key == 'p'))
	{
		cout << camera.getPosition().ToString() << endl;
	}

	if((key == 'W') || (key == 'w'))
	{
		camera.moveForward(0.8f);
	}
	if((key == 'S') || (key == 's'))
	{
		camera.moveBack(0.8f);
	}
	
	if((key == 'A') || (key == 'a'))
	{
		camera.moveLeft(0.8f);
	}
	if((key == 'D') || (key == 'd'))
	{
		camera.moveRight(0.8f);
	}

	if ((key == '1'))
	{
		camera.setPosition(Vector3(-solarSystem->GetMercury()->GetDisplayPosition().x, -solarSystem->GetMercury()->GetDisplayPosition().y, camera.getPosition().z));
	}
	if ((key == '2'))
	{
		camera.setPosition(Vector3(-solarSystem->GetVenus()->GetDisplayPosition().x, -solarSystem->GetVenus()->GetDisplayPosition().y, camera.getPosition().z));
	}
	if ((key == '3'))
	{
		camera.setPosition(Vector3(-solarSystem->GetEarth()->GetDisplayPosition().x, -solarSystem->GetEarth()->GetDisplayPosition().y, camera.getPosition().z));
	}
	if ((key == '4'))
	{
		camera.setPosition(Vector3(-solarSystem->GetMars()->GetDisplayPosition().x, -solarSystem->GetMars()->GetDisplayPosition().y, camera.getPosition().z));
	}
	if ((key == '5'))
	{
		camera.setPosition(Vector3(-solarSystem->GetJupiter()->GetDisplayPosition().x, -solarSystem->GetJupiter()->GetDisplayPosition().y, camera.getPosition().z));
	}
	if ((key == '6'))
	{
		camera.setPosition(Vector3(-solarSystem->GetSaturn()->GetDisplayPosition().x, -solarSystem->GetSaturn()->GetDisplayPosition().y, camera.getPosition().z));
	}
	if ((key == '7'))
	{
		camera.setPosition(Vector3(-solarSystem->GetUranus()->GetDisplayPosition().x, -solarSystem->GetUranus()->GetDisplayPosition().y, camera.getPosition().z));
	}
	if ((key == '8'))
	{
		camera.setPosition(Vector3(-solarSystem->GetNeptune()->GetDisplayPosition().x, -solarSystem->GetNeptune()->GetDisplayPosition().y, camera.getPosition().z));
	}
	if ((key == '9'))
	{
		camera.setPosition(Vector3(-solarSystem->GetMoon()->GetDisplayPosition().x, -solarSystem->GetMoon()->GetDisplayPosition().y, camera.getPosition().z));
	}
	if ((key == '0'))
	{
		camera.setPosition(Vector3(-solarSystem->GetSun()->GetDisplayPosition().x, -solarSystem->GetSun()->GetDisplayPosition().y, camera.getPosition().z));
	}
}

void SpecialKeys(int key, int x, int y)
{
	if (key == GLUT_KEY_LEFT)
	{
		camera.rotateY(-2);
	}

	if (key == GLUT_KEY_RIGHT)
	{
		camera.rotateY(2);
	}

	if (key == GLUT_KEY_UP)
	{
		camera.rotateX(1);
	}

	if (key == GLUT_KEY_DOWN)
	{
		camera.rotateX(-1);
	}
}

void MouseMovement(int x, int y)
{
	static int lastX = width / 2;
	static int lastY = height / 2;

	float deltaX = (x - lastX) * 0.1f;
	float deltaY = (y - lastY) * 0.1f;

	camera.rotateX(deltaY);
	camera.rotateY(deltaX);

	lastX = x;
	lastY = y;

	if(x > width  - 100 || x < 0 + 100 ||
	   y > height - 100 || y < 0 + 100)
	{
		lastX = width/2;
		lastY = height/2;
		glutWarpPointer(lastX, lastY);
	}
}

void UpdateGUI()
{
	mercuryGUI.setMassText(solarSystem->GetMercury()->GetPhysics()->getMass());
	mercuryGUI.setVelocityText(solarSystem->GetMercury()->GetPhysics()->getVelocity());

	venusGUI.setMassText(solarSystem->GetVenus()->GetPhysics()->getMass());
	venusGUI.setVelocityText(solarSystem->GetVenus()->GetPhysics()->getVelocity());

	earthGUI.setMassText(solarSystem->GetEarth()->GetPhysics()->getMass());
	earthGUI.setVelocityText(solarSystem->GetEarth()->GetPhysics()->getVelocity());

	marsGUI.setMassText(solarSystem->GetMars()->GetPhysics()->getMass());
	marsGUI.setVelocityText(solarSystem->GetMars()->GetPhysics()->getVelocity());

	jupiterGUI.setMassText(solarSystem->GetJupiter()->GetPhysics()->getMass());
	jupiterGUI.setVelocityText(solarSystem->GetJupiter()->GetPhysics()->getVelocity());

	saturnGUI.setMassText(solarSystem->GetSaturn()->GetPhysics()->getMass());
	saturnGUI.setVelocityText(solarSystem->GetSaturn()->GetPhysics()->getVelocity());

	uranusGUI.setMassText(solarSystem->GetUranus()->GetPhysics()->getMass());
	uranusGUI.setVelocityText(solarSystem->GetUranus()->GetPhysics()->getVelocity());

	neptuneGUI.setMassText(solarSystem->GetNeptune()->GetPhysics()->getMass());
	neptuneGUI.setVelocityText(solarSystem->GetNeptune()->GetPhysics()->getVelocity());

	moonGUI.setMassText(solarSystem->GetMoon()->GetPhysics()->getMass());
	moonGUI.setVelocityText(solarSystem->GetMoon()->GetPhysics()->getVelocity());
}

void myUpdate()
{
	glutSetWindow(windowId);

	int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	float deltaTime = (timeSinceStart - oldTimeFromStart) / 1000.0f;
	oldTimeFromStart = timeSinceStart;

	solarSystem->Update(deltaTime);

	UpdateGUI();

	glutPostRedisplay();
}

void InitGUI()
{
	glui->add_statictext("Mercury");
	mercuryGUI.mpMassText = glui->add_statictext("0");
	mercuryGUI.mpVelocityText = glui->add_statictext("0");

	glui->add_statictext("Venus");
	venusGUI.mpMassText = glui->add_statictext("0");
	venusGUI.mpVelocityText = glui->add_statictext("0");

	glui->add_statictext("Earth");
	earthGUI.mpMassText = glui->add_statictext("0");
	earthGUI.mpVelocityText = glui->add_statictext("0");

	glui->add_statictext("Mars");
	marsGUI.mpMassText = glui->add_statictext("0");
	marsGUI.mpVelocityText = glui->add_statictext("0");

	glui->add_statictext("Jupiter");
	jupiterGUI.mpMassText = glui->add_statictext("0");
	jupiterGUI.mpVelocityText = glui->add_statictext("0");

	glui->add_statictext("Saturn");
	saturnGUI.mpMassText = glui->add_statictext("0");
	saturnGUI.mpVelocityText = glui->add_statictext("0");

	glui->add_statictext("Uranus");
	uranusGUI.mpMassText = glui->add_statictext("0");
	uranusGUI.mpVelocityText = glui->add_statictext("0");

	glui->add_statictext("Neptune");
	neptuneGUI.mpMassText = glui->add_statictext("0");
	neptuneGUI.mpVelocityText = glui->add_statictext("0");

	glui->add_statictext("Moon");
	moonGUI.mpMassText = glui->add_statictext("0");
	moonGUI.mpVelocityText = glui->add_statictext("0");
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);

	if(argc == 1)
		filename = "sphere.dae";
	else
		filename = argv[1];	

	gltSetWorkingDirectory(argv[0]);
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);

	width = 800;
	height = 800;
	glutInitWindowSize(width, height);

	windowId = glutCreateWindow("Physics Viewer");
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);
	glutSpecialFunc(SpecialKeys);
	glutKeyboardFunc(Keys);
	//glutIdleFunc(myUpdate);
	glutPassiveMotionFunc(MouseMovement);

	GLUI_Master.set_glutIdleFunc(myUpdate);
	glui = GLUI_Master.create_glui("GUI");
	glui->set_main_gfx_window(windowId);
	InitGUI();

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
		return 1;
	}

	glutSetCursor(GLUT_CURSOR_NONE);
	glutWarpPointer(width/2, height/2);

	myInit();

	oldTimeFromStart = glutGet(GLUT_ELAPSED_TIME);

	glutMainLoop();

	//delete planetModel;
	//delete sunParticle;
	//delete earthParticle;
	//delete gravityGenerator;

	delete solarSystem;

	return 0;
}

bool LoadTGATexture(const char *szFileName, GLenum minFilter, GLenum magFilter, GLenum wrapMode)
{
	GLbyte *pBits;
	int nWidth, nHeight, nComponents;
	GLenum eFormat;

	pBits = gltReadTGABits(szFileName, &nWidth, &nHeight, &nComponents, &eFormat);
	if (pBits == NULL)
		return NULL;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, nComponents, nWidth, nHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBits);

	free(pBits);

	//TODO: Add mipmap stuff
	return true;
}