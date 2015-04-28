#include <GLTools.h>
#include <GLShaderManager.h>
#include <freeglut.h>
#include <GLFrustum.h>
#include <string>
#include <iostream>

#include "Camera.h"
#include "ModelFactory.h"

#include <PhysicsEngine/World.h>
#include <PhysicsEngine/Block.h>
#include <PhysicsEngine/Gravity.h>
#include "RigidBox.h"
#include "Color.h"
#include <PhysicsEngine/GroundContactGenerator.h>
#include <PhysicsEngine/CollisionData.h>
#include <PhysicsEngine/CollisionDetector.h>

using namespace std;

bool LoadTGATexture(const char *szFileName, GLenum minFilter, GLenum magFilter, GLenum wrapMode);

GLShaderManager	shaderManager;
M3DMatrix44f	mvpMatrix;
GLFrustum		viewFrustum;
GLint			width, height;

Camera camera;

ModelFactory modelFactory;

World *world;

RigidBox *block;

Gravity gravity(Vector3(0, -10, 0));
CollisionData cData;
ContactResolver resolver(5);
Contact contacts[256];

int windowId;

int oldTimeFromStart;

GLBatch lineBatch;

bool isPaused = false;

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

	world = new World(200);

	block = new RigidBox();
	block->SetState(Vector3(0, 20, 0), Quaternion::identity, Vector3(1, 1, 1), Vector3::zero);
	block->GetBody()->AddRotation(Vector3(-5, 1, 7));
	
	world->GetRigidbodies().push_back(block->GetBody());
	world->GetForceRegistry().Add(block->GetBody(), &gravity);

	cData.contactArray = contacts;

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

	block->GetModelMatrix(mModel);
	m3dMatrixMultiply44(mModelView, mView, mModel);
	shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, mModelView, viewFrustum.GetProjectionMatrix(), posLight1, Color::Red.GetColor());
	block->Draw();

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

void generateContacts()
{
	cData.Reset(200);
	cData.friction = (real)0.9;
	cData.restitution = (real)0.2;
	cData.tolerance = (real)0.1;

	CollisionBox ground;
	ground.halfSize = Vector3(50, 1, 50);
}

void myUpdate()
{
	int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	float deltaTime = (timeSinceStart - oldTimeFromStart) / 1000.0f;
	oldTimeFromStart = timeSinceStart;

	if (isPaused) return;

	glutSetWindow(windowId);

	world->StartFrame();
	world->RunPhysics(deltaTime);
	generateContacts();
	resolver.ResolveContacts(cData.contactArray, cData.contactCount, deltaTime);

	UpdateGUI();

	glutPostRedisplay();
}

void InitGUI()
{

}

void cleanUp()
{
	delete world;
	delete block;
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);

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
		//fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
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