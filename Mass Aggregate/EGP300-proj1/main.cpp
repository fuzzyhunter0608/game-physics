#include <GLTools.h>
#include <GLShaderManager.h>
#include <freeglut.h>
#include <GLFrustum.h>
#include <string>
#include <iostream>

#include "Camera.h"
#include "Model.h"
#include "ModelFactory.h"
#include "PhysicsEngine/ParticleFactory.h"
#include "PhysicsEngine/ParticleForceRegistry.h"
#include "PhysicsEngine/ParticleGravityEarth.h"
#include "PhysicsEngine/ParticleWorld.h"
#include "PhysicsEngine/ParticleGround.h"
#include "PhysicsEngine/ParticleRod.h"
#include "PhysicsEngine/ParticleCable.h"
#include "PhysicsEngine\ParticleCube.h"
#include "PhysicsEngine\ParticleTriangle.h"
#include "PhysicsEngine\ParticleTetrahedron.h"
#include "GameObject.h"
#include "Color.h"

using namespace std;

bool LoadTGATexture(const char *szFileName, GLenum minFilter, GLenum magFilter, GLenum wrapMode);

GLShaderManager	shaderManager;
M3DMatrix44f	mvpMatrix;
GLFrustum		viewFrustum;
GLint			width, height;

Camera camera;

ModelFactory modelFactory;
ParticleFactory particleFactory;

ParticleForceRegistry registry;

ParticleWorld particleWorld(500);

ParticleGround particleGround;
ParticleTetrahedron cube;

ParticleGravityEarth gravity(Vector3(0, -9.7, 0));

vector<GameObject> objects;
GameObject particleObject, particleObject2;
Model* sphereModel;

string filename;

int windowId;

int oldTimeFromStart;

GLBatch lineBatch;

bool isPaused = true;

void ChangeSize(int w, int h)
{
	width = w;
	height = h;

	glViewport(0,0,width, height);

	viewFrustum.SetPerspective(35.0f, (float)(width/height), 1.0f, 10000.0f);
}

void setup()
{
	lineBatch.Begin(GL_QUADS, 4);
	lineBatch.Vertex3f(-100, 0, -100);
	lineBatch.Vertex3f(-100, 0, 100);
	lineBatch.Vertex3f(100, 0, 100);
	lineBatch.Vertex3f(100, 0, -100);
	lineBatch.End();
}

void myInit()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	shaderManager.InitializeStockShaders();

	//Projection
	viewFrustum.SetPerspective(35.0f, (float)(width/height), 1.0f, 1000.0f);

	glEnable(GL_DEPTH_TEST);

	gravity = ParticleGravityEarth(Vector3(0, -10, 0));

	cube.init(Vector3(-2, 15, 0));

	auto particles = cube.getParticles();
	for (vector<Particle*>::iterator iter = particles.begin(); iter != particles.end(); ++iter)
	{
		particleWorld.GetParticles().push_back(*iter);
		particleWorld.GetForceRegistry().add(*iter, &gravity);
	}

	particleGround.init(&particleWorld.GetParticles());
	particleWorld.GetContactGenerators().push_back(&particleGround);

	auto rods = cube.getRods();
	for (vector<ParticleRod*>::iterator r = rods.begin(); r != rods.end(); ++r)
	{
		particleWorld.GetContactGenerators().push_back(*r);
	}

	sphereModel = modelFactory.GetModel(filename);

	// Create a game object for each particle
	for (ParticleWorld::Particles::iterator iter = particleWorld.GetParticles().begin(); iter != particleWorld.GetParticles().end(); ++iter)
	{
		GameObject object;
		object.SetPhysics(*iter);
		object.SetModel(sphereModel);
		objects.push_back(object);
	}

	setup();
}

void RenderScene(void)
{
	//Swap Colors
	GLfloat vColor[]=  { 1.0f, 1.0f, 1.0f, 1.0f };
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	float posLight1[] = { 1.0f, 1.0f, 1.0f, 0.0f };

	M3DMatrix44f mModel, mView, mModelView, mViewProj, mvp;

	//View
	camera.getViewMatrix(mView);

	// Draw Rods
	m3dLoadIdentity44(mModel);
	m3dMatrixMultiply44(mModelView, mView, mModel);
	shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, mModelView, viewFrustum.GetProjectionMatrix(), posLight1, Color::Red.GetColor());
	auto rods = cube.getRods();
	glBegin(GL_LINES);
	glPushMatrix();
	for (vector<ParticleRod*>::iterator iter = rods.begin(); iter != rods.end(); ++iter)
	{
		glVertex3f((*iter)->mParticles[0]->getPosition().x, (*iter)->mParticles[0]->getPosition().y, (*iter)->mParticles[0]->getPosition().z);
		glVertex3f((*iter)->mParticles[1]->getPosition().x, (*iter)->mParticles[1]->getPosition().y, (*iter)->mParticles[1]->getPosition().z);
	}
	glPopMatrix();
	glEnd();

	// Draw all game objects
	for (vector<GameObject>::iterator iter = objects.begin(); iter != objects.end(); ++iter)
	{
		(*iter).GetModelMatrix(mModel);
		//m3dLoadIdentity44(mModel);
		m3dMatrixMultiply44(mModelView, mView, mModel);
		shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, mModelView, viewFrustum.GetProjectionMatrix(), posLight1, Color::Yellow.GetColor());
		(*iter).Draw();
	}

	// Draw ground
	m3dLoadIdentity44(mModel);
	m3dMatrixMultiply44(mModelView, mView, mModel);
	shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, mModelView, viewFrustum.GetProjectionMatrix(), posLight1, Color::Blue.GetColor());
	lineBatch.Draw();

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

	if ((key == 'J') || (key == 'j'))
	{
		isPaused = !isPaused;
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
}

void myUpdate()
{
	int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	float deltaTime = (timeSinceStart - oldTimeFromStart) / 1000.0f;
	oldTimeFromStart = timeSinceStart;

	if (isPaused) return;

	glutSetWindow(windowId);

	particleWorld.StartFrame();

	particleWorld.RunPhysics(deltaTime);

	UpdateGUI();

	glutPostRedisplay();
}

void InitGUI()
{

}

void cleanUp()
{
	delete sphereModel;
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
	glutIdleFunc(myUpdate);
	glutPassiveMotionFunc(MouseMovement);

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

	cleanUp();
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