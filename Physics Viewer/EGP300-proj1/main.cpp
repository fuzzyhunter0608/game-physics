#include <GLTools.h>
#include <GLShaderManager.h>
#include <GL/glui.h>
#include <GLFrustum.h>
#include <string>

#include "Camera.h"
#include "Model.h"
#include "ModelFactory.h"
#include "PhysicsEngine/ParticleFactory.h"
#include "PhysicsEngine/ParticleForceRegistry.h"
#include "PhysicsEngine/ParticleGravity.h"
#include "GameObject.h"
#include "SolarSystem.h"

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

ModelFactory modelFactory;
ParticleFactory particleFactory;

ParticleForceRegistry registry;
ParticleGravity* gravityGenerator;

string filename;

int windowId;

int oldTimeFromStart;

void ChangeSize(int w, int h)
{
	width = w;
	height = h;

	glViewport(0,0,width, height);

	viewFrustum.SetPerspective(35.0f, (float)(width/height), 1.0f, 1000.0f);
}

void setup()
{
}

void myInit()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	shaderManager.InitializeStockShaders();

	solarSystem = new SolarSystem();

	/*planetModel = modelFactory.GetModel(filename);
	sunObject.SetModel(planetModel);
	earthObject.SetModel(planetModel);

	sunParticle = particleFactory.GetParticle(SUN);
	earthParticle = particleFactory.GetParticle(EARTH);
	
	sunObject.SetPhysics(sunParticle);
	earthObject.SetPhysics(earthParticle);

	sunObject.SetScale(Vector3(10)); 
	earthObject.SetScale(Vector3(1));*/

	//gravityGenerator = new ParticleGravity(sunParticle);
	//registry.add(earthParticle, gravityGenerator); // Sun's gravity affecting Earth

	//Projection
	viewFrustum.SetPerspective(35.0f, (float)(width/height), 1.0f, 1000.0f);

	glEnable(GL_DEPTH_TEST);

	setup();
}

void RenderScene(void)
{
	//Swap Colors
	GLfloat vColor[]=  { 1.0f, 1.0f, 1.0f, 1.0f };
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	float posLight1[] = { 1.0f, 1.0f, 1.0f, 0.0f };

	M3DMatrix44f mModel, mView, mModelView;

	//View
	camera.getViewMatrix(mView);

	//Sun
	solarSystem->GetSun()->GetModelMatrix(mModel);
	m3dMatrixMultiply44(mModelView, mView, mModel);
	shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, mModelView, viewFrustum.GetProjectionMatrix(), posLight1, colorYellow);
	solarSystem->GetSun()->Draw();

	//Mercury
	solarSystem->GetMercury()->GetModelMatrix(mModel);
	m3dMatrixMultiply44(mModelView, mView, mModel);
	shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, mModelView, viewFrustum.GetProjectionMatrix(), posLight1, colorYellow);
	solarSystem->GetMercury()->Draw();

	//Earth
	solarSystem->GetEarth()->GetModelMatrix(mModel);
	m3dMatrixMultiply44(mModelView, mView, mModel);
	shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, mModelView, viewFrustum.GetProjectionMatrix(), posLight1, colorBlue);
	solarSystem->GetEarth()->Draw();

	glutSwapBuffers();
}

void Keys(unsigned char key, int x, int y)
{
	if(key == 27)
		exit(0);

	if((key == 'R') || (key == 'r'))
	{
		glutWarpPointer(width/2, height/2);

		camera.reset();

		planetModel->setRotation(Vector3::zero);
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
	/*static int lastX = width / 2;
	static int lastY = height / 2;

	float deltaX = (x - lastX) * 0.1f;
	float deltaY = (y - lastY) * 0.1f;

	rotateAroundViewYaxis += deltaX;
	rotateAroundViewXaxis += deltaY;

	lastX = x;
	lastY = y;

	if(x > width  - 100 || x < 0 + 100 ||
	   y > height - 100 || y < 0 + 100)
	{
		lastX = width/2;
		lastY = height/2;
		glutWarpPointer(lastX, lastY);
	}*/
}

void myUpdate()
{
	glutSetWindow(windowId);

	int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	float deltaTime = (timeSinceStart - oldTimeFromStart) / 1000.0f;
	oldTimeFromStart = timeSinceStart;

	solarSystem->Update(deltaTime);

	glutPostRedisplay();
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
	GLUI *glui = GLUI_Master.create_glui("GUI");
	glui->set_main_gfx_window(windowId);
	glui->add_button("Test");

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