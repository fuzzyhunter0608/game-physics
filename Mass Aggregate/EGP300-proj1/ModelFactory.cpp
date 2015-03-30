#include "ModelFactory.h"

#include <sstream>

Model* ModelFactory::GetModel(string filename)
{
	ModelInfo info = parseFile(filename);

	Model* model = new Model();
	model->load(info);
	model->setup();
	return model;
}

ModelInfo ModelFactory::parseFile(string filename)
{
	ModelInfo info;

	info.tris = 0, info.quads = 0;
	bool isPolylist = true, isTriangles = false;
	info.isTextured = false;

	// Load Document
	tinyxml2::XMLDocument doc;
	doc.LoadFile(filename.c_str());

	// Get orientation
	info.zUp = doc.RootElement()->FirstChildElement("asset")->FirstChildElement("up_axis")->GetText();

	// Get texture
	tinyxml2::XMLElement* imgElement = doc.RootElement()->FirstChildElement("library_images");
	if (imgElement != NULL)
	{
		imgElement = imgElement->FirstChildElement("image");
		if (imgElement != NULL)
		{
			imgElement = imgElement->FirstChildElement("init_from");
			info.imgPath = imgElement->GetText();

			stringstream imgStream;
			imgStream << ".\\" << info.imgPath;
			info.imgPath = imgStream.str();

			info.isTextured = true;
		}
	}

	//For each geometry
	isPolylist = true; isTriangles = false;
	tinyxml2::XMLElement* geometryElement = doc.RootElement()->FirstChildElement("library_geometries")->FirstChildElement("geometry");
	while (geometryElement != NULL)
	{
		Geometry newGeometry;

		// Get Geometry ID
		newGeometry.id = geometryElement->Attribute("id");

		//For each source
		tinyxml2::XMLElement* sourceElement = geometryElement->FirstChildElement("mesh")->FirstChildElement("source");
		while (sourceElement != NULL)
		{
			Source newSource;

			newSource.id = sourceElement->Attribute("id");

			newSource.arraySize = sourceElement->FirstChildElement("float_array")->IntAttribute("count");

			string arrayString = sourceElement->FirstChildElement("float_array")->GetText();
			newSource.floatArray = new float[newSource.arraySize];
			floatArrayFromString(newSource.floatArray, newSource.arraySize, arrayString);

			newSource.count = sourceElement->FirstChildElement("technique_common")->FirstChildElement("accessor")->IntAttribute("count");
			newSource.stride = sourceElement->FirstChildElement("technique_common")->FirstChildElement("accessor")->IntAttribute("stride");

			newGeometry.sources.push_back(newSource);

			sourceElement = sourceElement->NextSiblingElement("source");
		}

		//Set position source
		Source positionSource;
		tinyxml2::XMLElement* verticesElement = geometryElement->FirstChildElement("mesh")->FirstChildElement("vertices")->FirstChildElement("input");
		while (verticesElement != NULL)
		{
			string semantic = verticesElement->Attribute("semantic");
			if (semantic.compare("POSITION") == 0)
			{
				string sourceID = verticesElement->Attribute("source");
				sourceID.erase(0, 1); //Delete '#'
				positionSource = getSourceById(newGeometry.sources, sourceID);
				break;
			}

			verticesElement->NextSiblingElement("input");
		}

		//For each polylist
		isPolylist = true; isTriangles = false;
		tinyxml2::XMLElement* polylistElement = geometryElement->FirstChildElement("mesh")->FirstChildElement("polylist");
		if (polylistElement == NULL)
		{
			polylistElement = geometryElement->FirstChildElement("mesh")->FirstChildElement("triangles");
			isPolylist = false;
			isTriangles = true;
		}
		while (polylistElement != NULL)
		{
			Polylist newPolylist;

			if (polylistElement->Attribute("materal") != NULL)
				newPolylist.material = polylistElement->Attribute("material");
			newPolylist.count = polylistElement->IntAttribute("count");

			if (newPolylist.count == 0)
			{
				tinyxml2::XMLElement* testElement = polylistElement->NextSiblingElement("polylist");
				isPolylist = true;
				isTriangles = false;
				if (testElement == NULL)
				{
					testElement = polylistElement->NextSiblingElement("triangles");
					isPolylist = false;
					isTriangles = true;
				}

				polylistElement = testElement;

				continue;
			}

			newPolylist.totalOffset = polylistElement->LastChildElement("input")->IntAttribute("offset") + 1;

			//For each input, get source
			tinyxml2::XMLElement* inputElement = polylistElement->FirstChildElement("input");
			while (inputElement != NULL)
			{
				string semantic = inputElement->Attribute("semantic");
				string sourceID = inputElement->Attribute("source");
				sourceID.erase(0, 1); //Delete '#'

				if (semantic.compare("VERTEX") == 0)
				{
					newPolylist.vertexSource = positionSource;
					newPolylist.vertexOffset = inputElement->IntAttribute("offset");
				}
				else if (semantic.compare("NORMAL") == 0)
				{
					newPolylist.normalSource = getSourceById(newGeometry.sources, sourceID);
					newPolylist.normalOffset = inputElement->IntAttribute("offset");
				}
				else if (semantic.compare("TEXCOORD") == 0)
				{
					newPolylist.texCoordSource = getSourceById(newGeometry.sources, sourceID);
					newPolylist.texCoordOffset = inputElement->IntAttribute("offset");
				}

				inputElement = inputElement->NextSiblingElement("input");
			}

			//Get <vcount>
			if (isPolylist)
			{
				string vCountString = polylistElement->FirstChildElement("vcount")->GetText();
				newPolylist.vCountArray = new int[newPolylist.count];
				intArrayFromString(newPolylist.vCountArray, newPolylist.count, vCountString);
			}
			else if (isTriangles)
			{
				newPolylist.vCountArray = new int[newPolylist.count];
				for (int i = 0; i < newPolylist.count; ++i)
					newPolylist.vCountArray[i] = 3;
			}

			//Get number of triangles and quads
			int totalVertices = 0;
			for (int i = 0; i < newPolylist.count; ++i)
			{
				totalVertices += newPolylist.vCountArray[i];

				if (newPolylist.vCountArray[i] == 3)
					info.tris++;
				else if (newPolylist.vCountArray[i] == 4)
					info.quads++;
			}

			newPolylist.vCountTotal = totalVertices;

			//Get index array
			tinyxml2::XMLElement* pElement = polylistElement->FirstChildElement("p");
			string indexString = pElement->GetText();
			int* indexArray = new int[totalVertices * newPolylist.totalOffset];
			intArrayFromString(indexArray, totalVertices * newPolylist.totalOffset, indexString);

			newPolylist.vertexIndexArray = new int[totalVertices];
			newPolylist.normalIndexArray = new int[totalVertices];
			newPolylist.texCoordIndexArray = new int[totalVertices];

			for (int i = 0; i < totalVertices * newPolylist.totalOffset; i += newPolylist.totalOffset)
			{
				newPolylist.vertexIndexArray[i / newPolylist.totalOffset] = indexArray[i + newPolylist.vertexOffset];
				newPolylist.normalIndexArray[i / newPolylist.totalOffset] = indexArray[i + newPolylist.normalOffset];

				if (info.isTextured)
					newPolylist.texCoordIndexArray[i / newPolylist.totalOffset] = indexArray[i + newPolylist.texCoordOffset];
			}


			newGeometry.polylists.push_back(newPolylist);

			tinyxml2::XMLElement* testElement = polylistElement->NextSiblingElement("polylist");
			isPolylist = true;
			isTriangles = false;
			if (testElement == NULL)
			{
				testElement = polylistElement->NextSiblingElement("triangles");
				isPolylist = false;
				isTriangles = true;
			}

			polylistElement = testElement;
		}


		info.geometries.push_back(newGeometry);

		geometryElement = geometryElement->NextSiblingElement("geometry");
	}

	return info;
}

void ModelFactory::floatArrayFromString(float* arr, int size, string theString)
{
	istringstream iss(theString);

	for (int i = 0; i < size; i++)
	{
		iss >> arr[i];
	}
}

void ModelFactory::intArrayFromString(int* arr, int size, string theString)
{
	istringstream iss(theString);

	for (int i = 0; i < size; i++)
	{
		iss >> arr[i];
	}
}

Source ModelFactory::getSourceById(vector<Source> sources, string id)
{
	vector<Source>::iterator iter;
	for (iter = sources.begin(); iter != sources.end(); ++iter)
	{
		if (iter->id == id)
			return *iter;
	}
}