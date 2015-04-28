#include "Model.h"

#include "ModelInfo.h"

Model::Model(void)
{
}

Model::~Model(void)
{
	cleanup();
}

void Model::load(vector<Geometry> &geometries, int tris, int quads, string orientation, bool isTextured)
{
	mGeometries = geometries;
	mTris = tris;
	mQuads = quads;
	mOrientation = orientation;
	mIsTextured = isTextured;
}

void Model::load(ModelInfo info)
{
	load(info.geometries, info.tris, info.quads, info.zUp, info.isTextured);
}

void Model::setup()
{
	mTriBatch.Begin(GL_TRIANGLES, mTris * 3, 1);
	mQuadBatch.Begin(GL_QUADS, mQuads * 4, 1);

	int xAdd = 0, yAdd = 1, zAdd = 2;
	if (mOrientation.compare("Z_UP") == 0)
	{
		yAdd = 2;
		zAdd = 1;
	}

	vector<Geometry>::iterator iter;
	for (iter = mGeometries.begin(); iter != mGeometries.end(); ++iter)
	{
		vector<Polylist>::iterator polyIter;
		for (polyIter = iter->polylists.begin(); polyIter != iter->polylists.end(); ++polyIter)
		{
			int i = 0, j = 0;
			while (i < polyIter->vCountTotal)
			{
				if (polyIter->vCountArray[j] == 3)
				{
					mTriBatch.Color4f(1, 1, 1, 1);
					mTriBatch.Normal3f(polyIter->normalSource.floatArray[polyIter->normalIndexArray[i] * 3], polyIter->normalSource.floatArray[polyIter->normalIndexArray[i] * 3 + yAdd], polyIter->normalSource.floatArray[polyIter->normalIndexArray[i] * 3 + zAdd]);
					if (mIsTextured)
						mTriBatch.MultiTexCoord2f(0, polyIter->texCoordSource.floatArray[polyIter->texCoordIndexArray[i] * 2], polyIter->texCoordSource.floatArray[polyIter->texCoordIndexArray[i] * 2 + 1]);
					mTriBatch.Vertex3f(polyIter->vertexSource.floatArray[polyIter->vertexIndexArray[i] * 3], polyIter->vertexSource.floatArray[polyIter->vertexIndexArray[i] * 3 + yAdd], polyIter->vertexSource.floatArray[polyIter->vertexIndexArray[i] * 3 + zAdd]);

					mTriBatch.Color4f(1, 1, 1, 1);
					mTriBatch.Normal3f(polyIter->normalSource.floatArray[polyIter->normalIndexArray[i + 1] * 3], polyIter->normalSource.floatArray[polyIter->normalIndexArray[i + 1] * 3 + yAdd], polyIter->normalSource.floatArray[polyIter->normalIndexArray[i + 1] * 3 + zAdd]);
					if (mIsTextured)
						mTriBatch.MultiTexCoord2f(0, polyIter->texCoordSource.floatArray[polyIter->texCoordIndexArray[i + 1] * 2], polyIter->texCoordSource.floatArray[polyIter->texCoordIndexArray[i + 1] * 2 + 1]);
					mTriBatch.Vertex3f(polyIter->vertexSource.floatArray[polyIter->vertexIndexArray[i + 1] * 3], polyIter->vertexSource.floatArray[polyIter->vertexIndexArray[i + 1] * 3 + yAdd], polyIter->vertexSource.floatArray[polyIter->vertexIndexArray[i + 1] * 3 + zAdd]);

					mTriBatch.Color4f(1, 1, 1, 1);
					mTriBatch.Normal3f(polyIter->normalSource.floatArray[polyIter->normalIndexArray[i + 2] * 3], polyIter->normalSource.floatArray[polyIter->normalIndexArray[i + 2] * 3 + yAdd], polyIter->normalSource.floatArray[polyIter->normalIndexArray[i + 2] * 3 + zAdd]);
					if (mIsTextured)
						mTriBatch.MultiTexCoord2f(0, polyIter->texCoordSource.floatArray[polyIter->texCoordIndexArray[i + 2] * 2], polyIter->texCoordSource.floatArray[polyIter->texCoordIndexArray[i + 2] * 2 + 1]);
					mTriBatch.Vertex3f(polyIter->vertexSource.floatArray[polyIter->vertexIndexArray[i + 2] * 3], polyIter->vertexSource.floatArray[polyIter->vertexIndexArray[i + 2] * 3 + yAdd], polyIter->vertexSource.floatArray[polyIter->vertexIndexArray[i + 2] * 3 + zAdd]);

					i += 3;
				}
				else if (polyIter->vCountArray[j] == 4)
				{
					mQuadBatch.Color4f(1, 1, 1, 1);
					mQuadBatch.Normal3f(polyIter->normalSource.floatArray[polyIter->normalIndexArray[i] * 3], polyIter->normalSource.floatArray[polyIter->normalIndexArray[i] * 3 + yAdd], polyIter->normalSource.floatArray[polyIter->normalIndexArray[i] * 3 + zAdd]);
					if (mIsTextured)
						mQuadBatch.MultiTexCoord2f(0, polyIter->texCoordSource.floatArray[polyIter->texCoordIndexArray[i] * 2], polyIter->texCoordSource.floatArray[polyIter->texCoordIndexArray[i] * 2 + 1]);
					mQuadBatch.Vertex3f(polyIter->vertexSource.floatArray[polyIter->vertexIndexArray[i] * 3], polyIter->vertexSource.floatArray[polyIter->vertexIndexArray[i] * 3 + yAdd], polyIter->vertexSource.floatArray[polyIter->vertexIndexArray[i] * 3 + zAdd]);

					mQuadBatch.Color4f(1, 1, 1, 1);
					mQuadBatch.Normal3f(polyIter->normalSource.floatArray[polyIter->normalIndexArray[i + 1] * 3], polyIter->normalSource.floatArray[polyIter->normalIndexArray[i + 1] * 3 + yAdd], polyIter->normalSource.floatArray[polyIter->normalIndexArray[i + 1] * 3 + zAdd]);
					if (mIsTextured)
						mQuadBatch.MultiTexCoord2f(0, polyIter->texCoordSource.floatArray[polyIter->texCoordIndexArray[i + 1] * 2], polyIter->texCoordSource.floatArray[polyIter->texCoordIndexArray[i + 1] * 2 + 1]);
					mQuadBatch.Vertex3f(polyIter->vertexSource.floatArray[polyIter->vertexIndexArray[i + 1] * 3], polyIter->vertexSource.floatArray[polyIter->vertexIndexArray[i + 1] * 3 + yAdd], polyIter->vertexSource.floatArray[polyIter->vertexIndexArray[i + 1] * 3 + zAdd]);

					mQuadBatch.Color4f(1, 1, 1, 1);
					mQuadBatch.Normal3f(polyIter->normalSource.floatArray[polyIter->normalIndexArray[i + 2] * 3], polyIter->normalSource.floatArray[polyIter->normalIndexArray[i + 2] * 3 + yAdd], polyIter->normalSource.floatArray[polyIter->normalIndexArray[i + 2] * 3 + zAdd]);
					if (mIsTextured)
						mQuadBatch.MultiTexCoord2f(0, polyIter->texCoordSource.floatArray[polyIter->texCoordIndexArray[i + 2] * 2], polyIter->texCoordSource.floatArray[polyIter->texCoordIndexArray[i + 2] * 2 + 1]);
					mQuadBatch.Vertex3f(polyIter->vertexSource.floatArray[polyIter->vertexIndexArray[i + 2] * 3], polyIter->vertexSource.floatArray[polyIter->vertexIndexArray[i + 2] * 3 + yAdd], polyIter->vertexSource.floatArray[polyIter->vertexIndexArray[i + 2] * 3 + zAdd]);

					mQuadBatch.Color4f(1, 1, 1, 1);
					mQuadBatch.Normal3f(polyIter->normalSource.floatArray[polyIter->normalIndexArray[i + 3] * 3], polyIter->normalSource.floatArray[polyIter->normalIndexArray[i + 3] * 3 + yAdd], polyIter->normalSource.floatArray[polyIter->normalIndexArray[i + 3] * 3 + zAdd]);
					if (mIsTextured)
						mQuadBatch.MultiTexCoord2f(0, polyIter->texCoordSource.floatArray[polyIter->texCoordIndexArray[i + 3] * 2], polyIter->texCoordSource.floatArray[polyIter->texCoordIndexArray[i + 3] * 2 + 1]);
					mQuadBatch.Vertex3f(polyIter->vertexSource.floatArray[polyIter->vertexIndexArray[i + 3] * 3], polyIter->vertexSource.floatArray[polyIter->vertexIndexArray[i + 3] * 3 + yAdd], polyIter->vertexSource.floatArray[polyIter->vertexIndexArray[i + 3] * 3 + zAdd]);

					i += 4;
				}
				else
				{
					i += polyIter->vCountArray[j];
				}

				j++;
			}
		}
	}

	mTriBatch.End();
	mQuadBatch.End();
}

void Model::cleanup()
{
	vector<Geometry>::iterator iter;
	for (iter = mGeometries.begin(); iter != mGeometries.end(); ++iter)
	{
		vector<Source>::iterator sourceIter;
		vector<Polylist>::iterator polyIter;
		for (polyIter = iter->polylists.begin(); polyIter != iter->polylists.end(); ++iter)
		{
			delete[] polyIter->vCountArray;

			delete[] polyIter->vertexIndexArray;
			delete[] polyIter->normalIndexArray;
			delete[] polyIter->texCoordIndexArray;
		}

		for (sourceIter = iter->sources.begin(); sourceIter != iter->sources.end(); ++sourceIter)
		{
			delete[] sourceIter->floatArray;
		}
	}
}

void Model::getModelMatrix(M3DMatrix44f &matrix, Vector3 position, Vector3 scale)
{
	M3DMatrix44f translationMatrix, rotationMatrix, scaleMatrix;

	m3dLoadIdentity44(matrix);
	
	m3dTranslationMatrix44(translationMatrix, position.x, position.y, position.z);
	m3dMatrixMultiply44(matrix, matrix, translationMatrix);
	m3dRotationMatrix44(rotationMatrix, (float)m3dDegToRad(mRotation.y), 0.0f, 1.0f, 0.0f);
	m3dMatrixMultiply44(matrix, matrix, rotationMatrix);
	m3dRotationMatrix44(rotationMatrix, (float)m3dDegToRad(mRotation.x), 1.0f, 0.0f, 0.0f);
	m3dMatrixMultiply44(matrix, matrix, rotationMatrix);
	m3dRotationMatrix44(rotationMatrix, (float)m3dDegToRad(mRotation.z), 0.0f, 0.0f, 1.0f);
	m3dMatrixMultiply44(matrix, matrix, rotationMatrix);
	
	//m3dScaleMatrix44(scaleMatrix, scale.x, scale.y, scale.z);
	//m3dMatrixMultiply44(matrix, matrix, scaleMatrix);
}

void Model::draw()
{
	mTriBatch.Draw();
	mQuadBatch.Draw();
}