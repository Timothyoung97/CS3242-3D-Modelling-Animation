#ifdef _WIN32
#include <Windows.h>
#include "GL\glut.h"
#define M_PI 3.141592654
#elif __APPLE__
#include <OpenGL/gl.h>
#include <GLUT/GLUT.h>
#endif

#include "math.h"
#include "mesh.h"
#include "operationLib.h"
#include "subdivisionLoop.h"
#include <string>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <iomanip>
#include <Eigen/Dense>

using std::cout;
using std::endl;

/// <summary>
/// A function that draw the object with respect to the toggles set
/// </summary>
/// <param name="toggleSmooth">if true draw smooth else !smooth</param>
/// <param name="toggleEdges">if true draw edge else !edge</param>
/// <param name="toggleColorComponets">if true show color else !color</param>
void myObjType::draw(bool toggleSmooth, bool toggleEdges, bool toggleColorComponets) {

	glEnable(GL_LIGHTING);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glEnable(GL_COLOR_MATERIAL);

	glPushMatrix();
	double longestSide = 0.0;
	for (int i = 0; i < 3; i++)
		if ((lmax[i] - lmin[i]) > longestSide)
			longestSide = (lmax[i] - lmin[i]);
	glScalef(4.0 / longestSide, 4.0 / longestSide, 4.0 / longestSide);
	glTranslated(-(lmin[0] + lmax[0]) / 2.0, -(lmin[1] + lmax[1]) / 2.0, -(lmin[2] + lmax[2]) / 2.0);

	// Ensure that the below strings are only printed once
	static bool isMessageGenerated;
	static std::string edgeAbsentMessage = "No edge in Obj.";
	static std::string edgeDrawnCompleteMassage = "Edge Drawn.";

	if (toggleEdges && operationLib::isObjectContainsEdges(fnextList, triangleList, triangleCount))
	{
		drawEdges();
		glDisable(GL_LIGHTING);
		glPopMatrix();
		if (!isMessageGenerated || !toggleDrawnSubdivisionEdge)
		{
			cout << edgeDrawnCompleteMassage << endl;
			toggleDrawnSubdivisionEdge = true;
			isMessageGenerated = true;
		}
		return;
	}
	else if (toggleEdges && !operationLib::isObjectContainsEdges(fnextList, triangleList, triangleCount))
	{
		if (!isMessageGenerated || !toggleDrawnSubdivisionEdge)
		{
			cout << edgeAbsentMessage << endl;
			isMessageGenerated = true;
			toggleDrawnSubdivisionEdge = true;
		}
	}

	for (int i = 1; i <= triangleCount; i++)
	{
		glBegin(GL_POLYGON);
		if (!toggleSmooth)
		{
			glNormal3dv(triangleNormalList[i]);
		}
		if (toggleColorComponets)
		{
			glColor3f(colors[getComponentID[i]][0], colors[getComponentID[i]][1], colors[getComponentID[i]][2]);
		}
		else 
		{
			glColor3f(0.5, 0.1, 0.3);
		}

  
		for (int j = 0; j < 3; j++) 
		{
			if (toggleSmooth) 
			{
				glNormal3dv(vertexNormalList[triangleList[i][j]]);
			}
			glVertex3dv(vertexList[triangleList[i][j]]);
		}
		glEnd();
	
	}
	glDisable(GL_LIGHTING);
	glutPostRedisplay();

	glPopMatrix();
}

/// <summary>
/// Main Task: Write an OBJ file
/// Write the curr displayed data into a obj file
/// </summary>
/// <param name="filename"></param>
void myObjType::writeFile(std::string filename)
{
	if (filename.find(".obj") != std::string::npos)
	{
		std::ostringstream lines;
		for (int i = 1; i <= vertexCount; i++)
		{
			lines << "v " << std::to_string(vertexList[i][0]) << " " << std::to_string(vertexList[i][1]) << " " << std::to_string(vertexList[i][2]) << endl;
		}
		for (int i = 1; i <= triangleCount; i++)
		{
			lines << "f " << std::to_string(triangleList[i][0]) << " " << std::to_string(triangleList[i][1]) << " " << std::to_string(triangleList[i][2]) << endl;
		}

		std::ofstream outfile(filename);
		outfile << lines.str() << endl;
		outfile.close();
	}
	else if (filename.find(".off") != std::string::npos)
	{
		std::ostringstream lines;
		lines << "OFF" << endl;

		lines << vertexCount << " " << triangleCount << endl;
		for (int i = 1; i <= vertexCount; i++)
		{
			lines << std::to_string(vertexList[i][0]) << " " << std::to_string(vertexList[i][1]) << " " << std::to_string(vertexList[i][2]) << endl;
		}
		for (int i = 1; i <= triangleCount; i++)
		{
			lines << "3 " << std::to_string(triangleList[i][0] - 1) << " " << std::to_string(triangleList[i][1] - 1) << " " << std::to_string(triangleList[i][2] - 1) << endl;
		}

		std::ofstream outfile(filename);
		outfile << lines.str() << endl;

		outfile.close();
	} 
	else 
	{
		cout << "The file format is not supported " << filename << endl;
		exit(1);
	}

	cout << "Obejct has been successfully written to disk as '" << filename << "'" << endl;
}

/// <summary>
/// Optional Task: Read some other file type - Main driver for file reading
/// </summary>
/// <param name="filename"></param>
void myObjType::readFile(std::string filename)
{
	if (filename.find(".obj") != std::string::npos)
	{
		readObjFile(filename);
	}
	else if (filename.find(".off") != std::string::npos)
	{
		readOffFile(filename);
	}
	else 
	{
		cout << "File format is not supported " << filename << endl;
		exit(1);
	}
}

/// <summary>
/// Provided source code for reading obj files
/// </summary>
/// <param name="filename"></param>
void myObjType::readObjFile(std::string filename)
{
	vertexCount = 0;
	triangleCount = 0;
	cout << endl;
	for (int i = 0; i < 100; i++)
	{
		cout << "=";
	}
	cout << endl;
	cout << "Opening " << filename << endl;
	std::ifstream inFile;
	inFile.open(filename);
	if (!inFile.is_open()) 
	{
		cout << "We cannot find your file " << filename << endl;
		exit(1);
	}

	std::string line;
	int i, j;
	bool firstVertex = 1;
	double currCood;

	while (getline(inFile, line))
	{
		if ((line[0] == 'v' || line[0] == 'f') && line[1] == ' ')
		{
			if (line[0] == 'v')
			{
				vertexCount++;
				i = 1;
				const char* linec = line.data();
				for (int k = 0; k < 3; k++) { // k is 0,1,2 for x,y,z
					while (linec[i] == ' ') i++;
					j = i;
					while (linec[j] != ' ') j++;
					currCood = vertexList[vertexCount][k] = atof(line.substr(i, j - i).c_str());
					if (firstVertex) 
						lmin[k] = lmax[k] = currCood;
					else {
						if (lmin[k] > currCood)
							lmin[k] = currCood;
						if (lmax[k] < currCood)
							lmax[k] = currCood;
					}
					i = j;
				}

				firstVertex = 0;
			}
			if (line[0] == 'f')
			{
				triangleCount++;
				i = 1;
				const char* linec = line.data();
				for (int k = 0; k < 3; k++) 
				{
					while (linec[i] == ' ') i++;
					j = i;
					while (linec[j] != ' ' && linec[j] != '\\') j++;
					triangleList[triangleCount][k] = atof(line.substr(i, j - i).c_str());
					i = j;
					fnextList[triangleCount][k] = 0;
					while (linec[j] != ' ') j++;
				}
			}
		}
	}

	cout << "Initializing adjacency lists and fnext list" << endl;
	setupAdjList();

	cout << "Compute face normal list" << endl;
	operationLib::generateFaceNormals(triangleNormalList, vertexList, triangleList, triangleCount);

	cout << "Compute vertex normal list" << endl;
	operationLib::generateVertexNormals(vertexNormalList, triangleNormalList, getAdjFacesFromVertex, vertexCount);

	cout << "Processing orientation of triangles..." << endl;
	orientTriangles();

	cout << endl;
	for (int i = 0; i < 100; i++)
	{
		cout << "=";
	}

	cout << endl;
	cout << "Calculating number of components..." << endl;
	processNumOfComponents();
	operationLib::generateStatistics(vertexList, vertexCount, triangleList, triangleCount);

	// Assignment of random color
	colors = std::vector<std::vector<double>>();
	for (int c = 0; c < uniqueCompCount; c++)
	{
		colors.push_back(
			{ 
				((double)rand() / (RAND_MAX)), 
				((double)rand() / (RAND_MAX)), 
				((double)rand() / (RAND_MAX)) 
			}
		);
	}

	cout << endl;
	for (int i = 0; i < 100; i++)
	{
		cout << "=";
	}

	cout << endl;
}

void myObjType::readOffFile(std::string filename)
{
	vertexCount = 0;
	triangleCount = 0;
	cout << endl;
	for (int i = 0; i < 50; i++)
	{
		cout << "_";
	}
	cout << endl;
	cout << "Opening " << filename << endl;
	std::ifstream inFile;
	inFile.open(filename);
	if (!inFile.is_open())
	{
		cout << "We cannot find your file " << filename << endl;
		exit(1);
	}

	std::string line;
	int i, j;
	bool firstVertex = 1;
	double currCood;
	int lineCount = 0;
	int numVertices = 0;
	int numFaces = 0;

	while (getline(inFile, line))
	{
		if (lineCount == 0)
		{
			if (line.substr(0, 3) != "OFF")
			{
				cout << "The folloing file does not follow a .off format " << filename << endl;
				exit(1);
			}
		}
		else if (lineCount == 1)
		{
			std::stringstream ss(line.c_str());
			int a, b;
			ss >> a >> b;
			numVertices = a;
			numFaces = b;
		}
		else
		{
			if (lineCount < numFaces + numVertices + 2) {
				if (lineCount <= numVertices + 1) { // Vertex
					vertexCount++;
					i = 0;
					const char* linec = line.data();
					for (int k = 0; k < 3; k++)
					{ // k is 0,1,2 for x,y,z
						while (linec[i] == ' ')
							i++;
						j = i;
						while (linec[j] != ' ')
							j++;
						currCood = vertexList[vertexCount][k] = atof(line.substr(i, j - i).c_str());
						if (firstVertex)
							lmin[k] = lmax[k] = currCood;
						else
						{
							if (lmin[k] > currCood)
								lmin[k] = currCood;
							if (lmax[k] < currCood)
								lmax[k] = currCood;
						}
						i = j;
					}
					firstVertex = 0;

				}
				else { // Faces
					if (line.substr(0, 1) != "3") {
						cout << "Program only accepts triangles!" << filename << endl;
						exit(1);
					}
					triangleCount++;
					i = 1;
					const char* linec = line.data();
					for (int k = 0; k < 3; k++)
					{
						while (linec[i] == ' ')
							i++;
						j = i;
						while (linec[j] != ' ' && linec[j] != '\\')
							j++;

						triangleList[triangleCount][k] = atof(line.substr(i, j - i).c_str()) + 1; // !! In .off file, vertex indices start at 1
						i = j;
						fnextList[triangleCount][k] = 0;
						while (linec[j] != ' ')
							j++;
					}
				}
			}
		}
		lineCount++;
	}

	cout << "Initializing adjacency lists and fnext list " << endl;
	setupAdjList();

	cout << "Compute face normal Lists " << endl;
	operationLib::generateFaceNormals(triangleNormalList, vertexList, triangleList, triangleCount);

	cout << "Compute vertex normal Lists " << endl;
	operationLib::generateVertexNormals(vertexNormalList, triangleNormalList, getAdjFacesFromVertex, vertexCount);

	cout << "Processing orientation of triangles..." << endl;
	orientTriangles();

	cout << endl;
	for (int i = 0; i < 100; i++)
	{
		cout << "=";
	}

	cout << endl;
	cout << "Calculating number of components..." << endl;
	processNumOfComponents();

	operationLib::generateStatistics(vertexList, vertexCount, triangleList, triangleCount);

	// Assignment of random color
	colors = std::vector<std::vector<double>>();
	for (int c = 0; c < uniqueCompCount; c++)
	{
		colors.push_back(
			{
				((double)rand() / (RAND_MAX)),
				((double)rand() / (RAND_MAX)),
				((double)rand() / (RAND_MAX))
			}
		);
	}

	cout << endl;
	for (int i = 0; i < 100; i++)
	{
		cout << "=";
	}

	cout << endl;
}

/// <summary>
/// Main Task: enext()
/// </summary>
/// <param name="orTri">Orientated Triangle Idx</param>
/// <returns></returns>
int myObjType::enext(const int orTri)
{
	int version = orTri & ((1 << 2) - 1);
	int triangleIndex = orTri >> 3;

	std::map<int, int> myMap =
	{
		{0, 1},
		{1, 2},
		{2, 0},
		{3, 5},
		{4, 3},
		{5, 4}
	};

	return (triangleIndex << 3) | myMap[version];
}

/// <summary>
/// Main Task: sym()
/// </summary>
/// <param name="orTri">Oriented Triangle Idx</param>
/// <returns></returns>
int myObjType::sym(const int orTri)
{
	int version = orTri & ((1 << 2) - 1);
	int triangleIndex = orTri >> 3;

	std::map<int, int> myMap =
	{
		{0, 3},
		{1, 4},
		{2, 5},
		{3, 0},
		{4, 1},
		{5, 2}
	};
	return (triangleIndex << 3) | myMap[version];
}

/// <summary>
/// Main Task: org()
/// </summary>
/// <param name="orTri">Oriented Triangle Idx</param>
/// <returns></returns>
int myObjType::org(const int orTri) 
{
	int version = orTri & ((1 << 2) - 1);
	int triangleIndex = orTri >> 3;

	std::map<int, int> myMap =
	{
		{0, 0},
		{1, 1},
		{2, 2},
		{3, 1},
		{4, 2},
		{5, 0}
	};
	return triangleList[triangleIndex][myMap[version]];
}

/// <summary>
/// Main Task: dest()
/// </summary>
/// <param name="orTri"></param>
/// <returns></returns>
int myObjType::dest(const int orTri)
{
	return org(sym(orTri));
}

/// <summary>
/// Optional Task: Find the number of components (Independent Surface)
/// Start from a random triangle and explore its surrounding triangle until exhausted. -> Keep in discoveredIndices set to track the number of discovered triangles
/// Then select another random triangle from the undiscovered queue and repeat the process above. -> Increment the count of component each time we cconduct the check.
/// </summary>
void myObjType::processNumOfComponents()
{
	getComponentID = {};
	uniqueCompCount = 0;
	std::set<int> discoveredIndices;
	std::queue<int> undiscoveredIndices;

	while (discoveredIndices.size() < triangleCount)
	{
		int undiscoveredIndex = operationLib::getIndexNotDiscovered(triangleCount, discoveredIndices); // just select one triangle that is not explored
		undiscoveredIndices.push(undiscoveredIndex);

		while (!undiscoveredIndices.empty()) // explore until all neighors are exhaust
		{
			int idx = undiscoveredIndices.front();
			undiscoveredIndices.pop();
			getComponentID[idx] = uniqueCompCount;

			if (discoveredIndices.find(idx) == discoveredIndices.end())
			{
				discoveredIndices.insert(idx);

				for (auto& neighbor : getAdjFacesFromFace[idx])
				{
					undiscoveredIndices.push(neighbor);
				}
			}
		}
		undiscoveredIndices = {};
		uniqueCompCount++; // increment the number of components we found so far
	}
	cout << "No. of Components: " << uniqueCompCount << endl;
}

/// <summary>
/// Temporary Storage kept for easy access
/// </summary>
namespace subdivisionOpsStorage
{
	int vertexCount = 0;
	int triangleCount = 0;
	int triangleList[MAXT][3];
	double vertexList[MAXV][3];
	std::map<int, int> getComponentID;
}

void myObjType::barycentrixSubdivide()
{
	cout << endl;
	for (int i = 0; i < 50; i++)
	{
		cout << "#";
	}
	cout << endl;
	cout << "Barycentric Subdivision in operation..." << endl;
	for (int i = 1; i <= triangleCount; i++) 
	{
		std::vector<Eigen::Vector3d> newVertex;

		for (int version = 0; version < 3; version++)
		{
			int edgeVertexIdx1 = triangleList[i][version];
			int edgeVertexIdx2 = triangleList[i][(version + 1) % 3];

			Eigen::Vector3d edgeVertex1(vertexList[edgeVertexIdx1][0], vertexList[edgeVertexIdx1][1], vertexList[edgeVertexIdx1][2]);
			Eigen::Vector3d edgeVertex2(vertexList[edgeVertexIdx2][0], vertexList[edgeVertexIdx2][1], vertexList[edgeVertexIdx2][2]);
			newVertex.push_back((edgeVertex1 + edgeVertex2) / 2.0);
		}

		Eigen::Vector3d v1(vertexList[triangleList[i][0]][0], vertexList[triangleList[i][0]][1], vertexList[triangleList[i][0]][2]);
		Eigen::Vector3d v2(vertexList[triangleList[i][1]][0], vertexList[triangleList[i][1]][1], vertexList[triangleList[i][1]][2]);
		Eigen::Vector3d v3(vertexList[triangleList[i][2]][0], vertexList[triangleList[i][2]][1], vertexList[triangleList[i][2]][2]);

		Eigen::Vector3d centroid = (v1 + v2 + v3) / 3.0;

		newVertex.push_back(v1);
		newVertex.push_back(v2);
		newVertex.push_back(v3);
		newVertex.push_back(centroid);

		std::vector<int> newVertexIndices;
		for (int j = 0; j < 7; j++)
		{
			std::pair<bool, int> result = subdivisionLoop::appendVertexToVertexList(subdivisionOpsStorage::vertexList, subdivisionOpsStorage::vertexCount, newVertex[j]);
			newVertexIndices.push_back(result.second);
			if (result.first)
			{
				subdivisionOpsStorage::vertexCount++;
			}
		}

		Eigen::Vector3i t1(newVertexIndices[6], newVertexIndices[3], newVertexIndices[0]);
		subdivisionLoop::appendTriangleToTriangleList(subdivisionOpsStorage::triangleList, subdivisionOpsStorage::triangleCount++, t1);
		//newSubdivision::componentIDs[newSubdivision::tcount] = componentIDs[i];
		Eigen::Vector3i t2(newVertexIndices[6], newVertexIndices[0], newVertexIndices[4]);
		subdivisionLoop::appendTriangleToTriangleList(subdivisionOpsStorage::triangleList, subdivisionOpsStorage::triangleCount++, t2);
		//newSubdivision::componentIDs[newSubdivision::tcount] = componentIDs[i];
		Eigen::Vector3i t3(newVertexIndices[6], newVertexIndices[4], newVertexIndices[1]);
		subdivisionLoop::appendTriangleToTriangleList(subdivisionOpsStorage::triangleList, subdivisionOpsStorage::triangleCount++, t3);
		//newSubdivision::componentIDs[newSubdivision::tcount] = componentIDs[i];
		Eigen::Vector3i t4(newVertexIndices[6], newVertexIndices[1], newVertexIndices[5]);
		subdivisionLoop::appendTriangleToTriangleList(subdivisionOpsStorage::triangleList, subdivisionOpsStorage::triangleCount++, t4);
		//newSubdivision::componentIDs[newSubdivision::tcount] = componentIDs[i];
		Eigen::Vector3i t5(newVertexIndices[6], newVertexIndices[5], newVertexIndices[2]);
		subdivisionLoop::appendTriangleToTriangleList(subdivisionOpsStorage::triangleList, subdivisionOpsStorage::triangleCount++, t5);
		//newSubdivision::componentIDs[newSubdivision::tcount] = componentIDs[i];
		Eigen::Vector3i t6(newVertexIndices[6], newVertexIndices[2], newVertexIndices[3]);
		subdivisionLoop::appendTriangleToTriangleList(subdivisionOpsStorage::triangleList, subdivisionOpsStorage::triangleCount++, t6);
		//newSubdivision::componentIDs[newSubdivision::tcount] = componentIDs[i];

	}

	triangleCount = subdivisionOpsStorage::triangleCount;
	vertexCount = subdivisionOpsStorage::vertexCount;

	std::copy(&subdivisionOpsStorage::triangleList[0][0], &subdivisionOpsStorage::triangleList[0][0] + triangleCount * 3 + 3, &triangleList[0][0]);
	std::copy(&subdivisionOpsStorage::vertexList[0][0], &subdivisionOpsStorage::vertexList[0][0] + vertexCount * 3 + 3, &vertexList[0][0]);

	cout << "Operation Complete... Processing Statistics... " << endl;

	setupAdjList();
	operationLib::generateFaceNormals(triangleNormalList, vertexList, triangleList, triangleCount);
	operationLib::generateVertexNormals(vertexNormalList, triangleNormalList, getAdjFacesFromVertex, vertexCount);
	processNumOfComponents();

	cout << "No. of vertices: " << vertexCount << endl;
	cout << "No. of triangles: " << triangleCount << endl;

	subdivisionOpsStorage::triangleCount = 0;
	subdivisionOpsStorage::vertexCount = 0;
	toggleSubdivision = true;
	toggleDrawnSubdivisionEdge = false;

	cout << endl;
	for (int i = 0; i < 50; i++)
		cout << "#";
	cout << endl;
}

void myObjType::loopSubdivide(int version)
{
	cout << endl;
	for (int i = 0; i < 50; i++)
		cout << "#";
	cout << endl;

	cout << "Loop Subdivision in opertaion..." << endl;

	for (int i = 1; i <= triangleCount; i++)
	{
		std::vector<Eigen::Vector3d> oddVertices;
		std::vector<Eigen::Vector3d> evenVertices;

		for (int version = 0; version < 3; version++)
		{
			std::pair<int, int> edgeVertices = std::make_pair(triangleList[i][version], triangleList[i][(version + 1) % 3]);
			int edgeVertexIdx1 = edgeVertices.first;
			int edgeVertexIdx2 = edgeVertices.second;

			std::set<int> adjacentEdgeVertices = getAdjVerticesFromEdge[{triangleList[i][version], triangleList[i][(version + 1) % 3]}];

			// 2. Compute one new odd vertex from the two edge vertices
			Eigen::Vector3d average;

			if (adjacentEdgeVertices.size() == 1)
			{ // We have an adge with only one neighboring face
				average = subdivisionLoop::generateOddLoopVertexBoundary(vertexList, edgeVertexIdx1, edgeVertexIdx2);
			}
			else
			{ // We have an edge with two neighboring faces
				int adjIdx1 = *std::next(adjacentEdgeVertices.begin(), 0);
				int adjIdx2 = *std::next(adjacentEdgeVertices.begin(), 1);
				average = subdivisionLoop::generateOddLoopVertexInterior(vertexList, edgeVertexIdx1, edgeVertexIdx2, adjIdx1, adjIdx2);
			}

			oddVertices.push_back(average);

			std::set<int> adjacentVertexVertices = getAdjVerticesFromVertex[edgeVertexIdx1];
			Eigen::Vector3d newVertex;
			if (adjacentVertexVertices.size() == 2)
			{ // We have an adge with only one
				newVertex = subdivisionLoop::generateEvenLoopVertexBoundary(vertexList, edgeVertexIdx1, *std::next(adjacentVertexVertices.begin(), 0), *std::next(adjacentVertexVertices.begin(), 1));
			}
			else
			{
				newVertex = subdivisionLoop::generateEvenLoopVertexInterior(vertexList, edgeVertexIdx1, adjacentVertexVertices, version);
			}
			evenVertices.push_back(newVertex);
		}

		// 4. Rebuild mesh / Connect vertices to create new faces
		std::vector<int> newVertexIndices;
		for (int j = 0; j < 3; j++)
		{
			std::pair<bool, int> result = subdivisionLoop::appendVertexToVertexList(subdivisionOpsStorage::vertexList, subdivisionOpsStorage::vertexCount, oddVertices[j]);
			newVertexIndices.push_back(result.second);
			if (result.first)
			{
				subdivisionOpsStorage::vertexCount++;
			}
		}

		for (int j = 0; j < 3; j++)
		{
			std::pair<bool, int> result = subdivisionLoop::appendVertexToVertexList(subdivisionOpsStorage::vertexList, subdivisionOpsStorage::vertexCount, evenVertices[j]);
			newVertexIndices.push_back(result.second);
			if (result.first)
			{
				subdivisionOpsStorage::vertexCount++;
			}
			getComponentID[result.second] = getComponentID[i];
		}

		Eigen::Vector3i t1(newVertexIndices[3], newVertexIndices[0], newVertexIndices[2]);
		subdivisionLoop::appendTriangleToTriangleList(subdivisionOpsStorage::triangleList, subdivisionOpsStorage::triangleCount++, t1);
		//newSubdivision::componentIDs[newSubdivision::tcount] = componentIDs[i];
		Eigen::Vector3i t2(newVertexIndices[0], newVertexIndices[4], newVertexIndices[1]);
		subdivisionLoop::appendTriangleToTriangleList(subdivisionOpsStorage::triangleList, subdivisionOpsStorage::triangleCount++, t2);
		//newSubdivision::componentIDs[newSubdivision::tcount] = componentIDs[i];
		Eigen::Vector3i t3(newVertexIndices[2], newVertexIndices[0], newVertexIndices[1]);
		subdivisionLoop::appendTriangleToTriangleList(subdivisionOpsStorage::triangleList, subdivisionOpsStorage::triangleCount++, t3);
		//newSubdivision::componentIDs[newSubdivision::tcount] = componentIDs[i];
		Eigen::Vector3i t4(newVertexIndices[2], newVertexIndices[1], newVertexIndices[5]);
		subdivisionLoop::appendTriangleToTriangleList(subdivisionOpsStorage::triangleList, subdivisionOpsStorage::triangleCount++, t4);
		//newSubdivision::componentIDs[newSubdivision::tcount] = componentIDs[i];
	}

	triangleCount = subdivisionOpsStorage::triangleCount;
	vertexCount = subdivisionOpsStorage::vertexCount;
	//componentIDs = newSubdivision::componentIDs;

	std::copy(&subdivisionOpsStorage::triangleList[0][0], &subdivisionOpsStorage::triangleList[0][0] + triangleCount * 3 + 3, &triangleList[0][0]);
	std::copy(&subdivisionOpsStorage::vertexList[0][0], &subdivisionOpsStorage::vertexList[0][0] + vertexCount * 3 + 3, &vertexList[0][0]);

	cout << "Subdivision completed... Recalculating data structures, normals etc. " << endl;

	setupAdjList();
	operationLib::generateFaceNormals(triangleNormalList, vertexList, triangleList, triangleCount);
	operationLib::generateVertexNormals(vertexNormalList, triangleNormalList, getAdjFacesFromVertex, vertexCount);
	orientTriangles();
	//processNumOfComponents(); // Don't add it!!!!
	cout << "No. of vertices: " << vertexCount << endl;
	cout << "No. of triangles: " << triangleCount << endl;

	subdivisionOpsStorage::triangleCount = 0;
	subdivisionOpsStorage::vertexCount = 0;
	toggleSubdivision = true;
	toggleDrawnSubdivisionEdge = false;

	cout << endl;
	for (int i = 0; i < 50; i++)
		cout << "#";
	cout << endl;
}

bool myObjType::orientTriangles()
{
	std::set<int> discoveredIndices;
	std::queue<int> undiscoveredIndices;
	int numOfTriOriented = 0;
	while (discoveredIndices.size() < triangleCount)
	{
		int undiscoveredIndex = operationLib::getIndexNotDiscovered(triangleCount, discoveredIndices);
		undiscoveredIndices.push(undiscoveredIndex);
		discoveredIndices.insert(undiscoveredIndex);
		
		while (!undiscoveredIndices.empty())
		{
			int idx = undiscoveredIndices.front();
			undiscoveredIndices.pop();

			for (int version = 0; version < 3; version++)
			{ // Check each neighbor
				int orTri_neighbor = fnextList[idx][version];
				if (orTri_neighbor != 0)
				{ // If no edge vertex
					int neighbor_index = orTri_neighbor >> 3;
					int neighbor_version = orTri_neighbor & ((1 << 2) - 1);
					bool hasConflict = operationLib::isSameOrientation(triangleList, idx, version, neighbor_index, neighbor_version);
					if (discoveredIndices.find(neighbor_index) != discoveredIndices.end())
					{ // Already seen
						if (hasConflict)
						{
							cout << "Failure in orienting triangles...!" << endl;

							return false;
						}
					}
					else
					{
						if (hasConflict)
						{
							// Element not yet seen but conflict -> Orient it
							int oldValue = triangleList[neighbor_index][1];
							triangleList[neighbor_index][1] = triangleList[neighbor_index][2];
							triangleList[neighbor_index][2] = oldValue;

							// Also update fnext
							int oldValueFnext = fnextList[neighbor_index][0];
							fnextList[neighbor_index][0] = fnextList[neighbor_index][2];
							fnextList[neighbor_index][2] = oldValueFnext;

							numOfTriOriented += 1;
						}
						undiscoveredIndices.push(neighbor_index);
						discoveredIndices.insert(neighbor_index);
					}
				}
			}
		}
		undiscoveredIndices = {};
	}

	if (numOfTriOriented == 0)
	{
		cout << "No triangles had to be oriented!" << endl;
	}
	else
	{
		// Since triangles had to be flipped, we need to recompute the normals and also fnext

		cout << "Successfully oriented " << numOfTriOriented << " triangles!" << endl;
		cout << "Datastructures have to be recomputed..." << endl;
		setupAdjList();
		operationLib::generateFaceNormals(triangleNormalList, vertexList, triangleList, triangleCount);
		operationLib::generateVertexNormals(vertexNormalList, triangleNormalList, getAdjFacesFromVertex, vertexCount);
	}

	return true;
}

void myObjType::drawEdges()
{
	glDisable(GL_LIGHTING);
	for (int i = 1; i <= triangleCount; i++)
	{
		// Check each triangle
		for (int version = 0; version < 3; version++)
		{
			int orTri_neighbor = fnextList[i][version];
			if (orTri_neighbor == 0)
			{ // Edge vertices!
				std::pair<int, int> edgeVertices = operationLib::getVerticesFromtIdxAndtVersion(triangleList, i, version);
				glBegin(GL_LINES);
				glColor3f(1.0f, 0.0f, 0.0f); // make this vertex red
				glVertex3dv(vertexList[edgeVertices.first]);
				glVertex3dv(vertexList[edgeVertices.second]);
				glEnd();
			}
		}
	}
}

void myObjType::setupAdjList()
{
	cout << "Generating adjacency lists... " << endl;
	getAdjFacesFromVertex = {};
	getAdjFacesFromEdge = {};
	getAdjFacesFromFace = {};
	getAdjVerticesFromEdge = {};
	getAdjVerticesFromVertex = {};

	// 1. Init adjFacesToEdge, adjVerticesToVertex and adjFacesToVertex
	// For an edge given by two vertices, store the adjacent faces
	for (int i = 1; i <= triangleCount; i++)
	{
		for (int version = 0; version < 3; version++)
		{
			int vIdx0 = triangleList[i][version];
			int vIdx1 = triangleList[i][(version + 1) % 3];
			int vIdx2 = triangleList[i][(version + 2) % 3];
			std::set<int> key = { vIdx0, vIdx1 };
			int orTri = i << 3 | version;
			getAdjFacesFromEdge[key].insert(orTri);
			getAdjVerticesFromEdge[key].insert(vIdx2);

			getAdjVerticesFromVertex[vIdx0].insert(vIdx1);
			getAdjVerticesFromVertex[vIdx0].insert(vIdx2);
			getAdjFacesFromVertex[vIdx0].insert(i);
		}
	}
	
	//for(std::map<std::set<int>, std::set<int>>::const_iterator it = getAdjFacesFromEdge.begin();
	//	it != getAdjFacesFromEdge.end(); ++it)
	//{
	//	int v0 = *std::next(it->first.begin(), 0);
	//	int v1 = *std::next(it->first.begin(), 1);
	//	int f0 = *std::next(it->second.begin(), 0);
	//	int f1 = *std::next(it->second.begin(), 1);
	//	cout << "Edge: {" << v0 << ",  " << v1 <<"} -> fnext:{idx: "
	//	<<  (f0 >> 3) << " , v: " << (f0 & ((1 << 2) - 1) ) << "}\n";
	//	cout << "Edge: {" << v0 << ",  " << v1 <<"} -> fnext:{idx: "
	//	<<  (f1 >> 3) << " , v: " << (f1 & ((1 << 2) - 1) ) << "}\n";
	//}
  
  // 2. Init adjFacesToFace
	for (int i = 1; i <= triangleCount; i++)
	{
		for (int version = 0; version < 3; version++)
		{
			std::set<int> key = { triangleList[i][version], triangleList[i][(version + 1) % 3] };

			std::set<int> opposite_faces = getAdjFacesFromEdge[key];
			int face0 =*std::next(opposite_faces.begin(), 0);
			int face1 =*std::next(opposite_faces.begin(), 1);
			// If face1 index is not <=tcount, then this is not a face, but an edge face! -> Store 0
			if (opposite_faces.size() == 1)
			{
				face1 = 0;
			}

			int fnext = i == (face0 >> 3) ? face1 : face0; // Opposite face is the one that is not the current_face
			fnextList[i][version] = fnext;

			if (fnext != 0)
			{ // If no edge vertex
				getAdjFacesFromFace[i].insert(fnext >> 3);
			}
		}
	}
}
