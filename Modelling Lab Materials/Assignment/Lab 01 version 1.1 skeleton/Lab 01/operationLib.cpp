#include "operationLib.h"
#include <iostream>

#define M_PI 3.141592654

using std::cout;
using std::endl;

namespace operationLib
{
	/// <summary>
	/// A fucntion that returns a smallest tIdx (from 1 to triangleCount) that is not discovered yet.
	/// </summary>
	/// <param name="triangleCount">Total Counts of Triangles</param>
	/// <param name="tFound">Set of tIdx that has been discovered</param>
	/// <returns>min tIdx not discovered in tFound</returns>
	int getIndexNotDiscovered(int triangleCount, const std::set<int> tFound)
	{
		for (int i = 1; i <= triangleCount; i++)
		{
			if (tFound.find(i) == tFound.end())
			{
				return i;
			}
		}
		return -1;
	}

	/// <summary>
	/// Main Task: Compute Angle Statistics (Supporting Function to compute the individual angle)
	/// A function to compute the angle between two vectors
	/// </summary>
	/// <param name="vector1">First Vector</param>
	/// <param name="vector2">Second Vector</param>
	/// <returns>(double) Angle in degree</returns>
	double computeAngle(const Eigen::Vector3d vector1, const Eigen::Vector3d vector2)
	{
		// Dot Product
		// a • b = |a| |b| cos(degree)
		// degree = cos^-1 ( a • b / |a||b|)
		double dot = vector1.dot(vector2);

		// acos returns in radian, hence need to convert back to degree
		return acos(dot / vector1.norm() / vector2.norm()) * 180 / M_PI;
	}

	/// <summary>
	/// A function to return the first two vertices (dominate edge) of a given triangle and its version.
	/// i.e. Assume triangle 1 version 0 is 'abc', 'ab' will be the first 2 vertices of the given triangle.
	/// </summary>
	/// <param name="triangleList">List of triangles</param>
	/// <param name="tIdx">triangle index</param>
	/// <param name="tVersion">triangle version</param>
	/// <returns>pair(vertex1, vertex2)</returns>
	std::pair<int, int> getVerticesFromtIdxAndtVersion(int triangleList[MAXT][3], const int tIdx, const int tVersion)
	{
		int vertex0, vertex1;
		if (tVersion == 0)
		{
			vertex0 = triangleList[tIdx][0];
			vertex1 = triangleList[tIdx][1];
		}
		else if (tVersion == 1)
		{
			vertex0 = triangleList[tIdx][1];
			vertex1 = triangleList[tIdx][2];
		}
		else
		{
			vertex0 = triangleList[tIdx][2];
			vertex1 = triangleList[tIdx][0];
		}
		return std::make_pair(vertex0, vertex1);
	}

	/// <summary>
	/// A function to check whether the object contains boundaries
	/// </summary>
	/// <param name="fnextList">fnext DS</param>
	/// <param name="triangleList">Triangle List</param>
	/// <param name="triangleCount">Count of Triangles</param>
	/// <returns>boolean</returns>
	bool isObjectContainsEdges(int fnextList[MAXT][3], int triangleList[MAXT][3], int triangleCount)
	{
		std::set<std::pair<int, int>> tempEdgeVerticesSet; // a set to store the boundary if found

		// For each triangle
		for (int i = 1; i <= triangleCount; i++)
		{
			for (int version = 0; version < 3; version++)
			{
				int orTriNeighbor = fnextList[i][version]; // get the neighboring triangle by querying the fnext DS
				if (orTriNeighbor == 0) // if orTriNeighor is 0, this is a boundary triangle
				{
					std::pair<int, int> edgeVertices = operationLib::getVerticesFromtIdxAndtVersion(
						triangleList, i, version);
					tempEdgeVerticesSet.insert(std::make_pair(edgeVertices.first, edgeVertices.second));
					// append into the set
				}
			}
		}
		return !tempEdgeVerticesSet.empty(); // if not empty, then the object is a boundary object.
	}

	/// <summary>
	/// A function to verify whether 2 given triangles have the same orientation
	/// </summary>
	/// <param name="triangleList">Triangle List</param>
	/// <param name="tIdx1">First Triangle Index</param>
	/// <param name="tVersion1">First Triangle Version</param>
	/// <param name="tIdx2">Second Triangle Index</param>
	/// <param name="tVersion2">Second Triangle Version</param>
	/// <returns></returns>
	bool isSameOrientation(int triangleList[MAXT][3], const int tIdx1, const int tVersion1, const int tIdx2,
	                       const int tVersion2)
	{
		std::pair<int, int> triangle1Vertices = getVerticesFromtIdxAndtVersion(triangleList, tIdx1, tVersion1);
		std::pair<int, int> triangle2vertices = getVerticesFromtIdxAndtVersion(triangleList, tIdx2, tVersion2);
		return triangle1Vertices == triangle2vertices;
		// if two triangle has the same orientation, their dominating edge will be the same
	}

	/// <summary>
	/// Main Task: Compute Angle Statistics (Driver function for computation of all angles)
	/// A function to compute the number of triangles, vertices and angles in all triangles and count the number of times they fall in each 10-degree angle slot
	/// </summary>
	/// <param name="vertexList">Vertex List</param>
	/// <param name="vertexCount">Total Vertex Count</param>
	/// <param name="triangleList">Triangle List</param>
	/// <param name="triangleCount">Total Triangle Count</param>
	void generateStatistics(double vertexList[MAXV][3], int vertexCount, int triangleList[MAXT][3], int triangleCount)
	{
		double minAngle = 360;
		double maxAngle = 0;

		int statMinAngle[18] = {0};
		int statMaxAngle[18] = {0};

		// for each triangle
		for (int i = 1; i <= triangleCount; i++)
		{
			// get the 3 vertex for the triangle
			Eigen::Vector3d v1(vertexList[triangleList[i][0]]);
			Eigen::Vector3d v2(vertexList[triangleList[i][1]]);
			Eigen::Vector3d v3(vertexList[triangleList[i][2]]);

			// get the 3 vector of the triangle
			Eigen::Vector3d v1_v2(v2 - v1);
			Eigen::Vector3d v1_v3(v3 - v1);
			Eigen::Vector3d v2_v3(v3 - v2);

			// get the 3 angles in the triangle
			// take note that when calculating the angle, need to ensure that the vectors are originating from the same origin
			double angle1 = operationLib::computeAngle(v1_v2, v1_v3);
			double angle2 = operationLib::computeAngle(-v1_v2, v2_v3);
			double angle3 = 180.0 - angle2 - angle1;

			double min = std::min(std::min(angle1, angle2), angle3); // find the minimum angle
			double max = std::max(std::max(angle1, angle2), angle3); // find the maximum angle

			// increment accordingly
			statMinAngle[int(floor(min / 10))] += 1;
			statMaxAngle[int(floor(max / 10))] += 1;

			// Maintenance of [min, max] angle 
			minAngle = minAngle < min ? minAngle : min;
			maxAngle = maxAngle > max ? maxAngle : max;
		}

		cout << endl;
		for (int i = 0; i < 30; i++) cout << "=";

		cout << endl;
		cout << "In all triangles, the minimum angle found = " << minAngle << endl;
		cout << "In all triangles, the maximum angle found = " << maxAngle << endl;

		cout << endl;
		cout << "Statistics for Maximum Angles" << endl;
		for (int i = 0; i < 18; i++) cout << statMaxAngle[i] << "|";

		cout << endl;
		cout << "Statistics for Minimum Angles" << endl;
		for (int i = 0; i < 18; i++) cout << statMinAngle[i] << "|";

		cout << endl;
		cout << "Total Count of vertices: " << vertexCount << endl;
		cout << "Total Count of triangles: " << triangleCount << endl;

		cout << endl;
		for (int i = 0; i < 30; i++) cout << "=";

		cout << endl;
	}

	/// <summary>
	/// Main Task: Computing Normal Vectors - Of Vertex
	/// A function to generate all vertex normals using the average of all adjacent faces, then store in the vertexNormalList
	/// </summary>
	/// <param name="vertexNormalList">Empty vertex normal list to be filled</param>
	/// <param name="triangleNormalList">List of normals of all triangles</param>
	/// <param name="getVertexFromAdjFace">DS (Set) to host all adjacent faces for each vertex</param>
	/// <param name="vertexCount">Total number of vertices</param>
	void generateVertexNormals(double vertexNormalList[MAXV][3], double triangleNormalList[MAXT][3],
	                           std::map<int, std::set<int>> getVertexFromAdjFace, int vertexCount)
	{
		// for each vertex
		for (int i = 1; i <= vertexCount; i++)
		{
			// generate a set of triangle (index) that surrounds the vertex i
			std::set<int> adjTriIndices = getVertexFromAdjFace[i];

			// Compute a normalize vector using the sum of the surface normal from the surrounding triangles
			Eigen::Vector3d sumVector(0, 0, 0);
			for (auto const& j : adjTriIndices)
			{
				sumVector[0] += triangleNormalList[j][0];
				sumVector[1] += triangleNormalList[j][1];
				sumVector[2] += triangleNormalList[j][2];
			}
			sumVector.normalize();

			// store the vertex normal into vertex normal list
			vertexNormalList[i][0] = sumVector[0];
			vertexNormalList[i][1] = sumVector[1];
			vertexNormalList[i][2] = sumVector[2];
		}
	}

	/// <summary>
	/// Main Task: Computing Normal Vectors - Of Triangle
	/// A function that computes all the face normals by using cross product, then store in triangleNormalList
	/// </summary>
	/// <param name="triangleNormalList">Triangle Normal List</param>
	/// <param name="vertexList">List of vertex</param>
	/// <param name="triangleList">List of triangle</param>
	/// <param name="triangleCount">Count of triangle</param>
	void generateFaceNormals(double triangleNormalList[MAXT][3], double vertexList[MAXV][3], int triangleList[MAXT][3],
	                         int triangleCount)
	{
		// for each triangle
		for (int i = 1; i <= triangleCount; i++)
		{
			// get the 3 vertex making up the triangle
			Eigen::Vector3d v1(vertexList[triangleList[i][0]]);
			Eigen::Vector3d v2(vertexList[triangleList[i][1]]);
			Eigen::Vector3d v3(vertexList[triangleList[i][2]]);

			// compute the 2 vectors needed for cross product
			Eigen::Vector3d v1_v2(v2 - v1);
			Eigen::Vector3d v1_v3(v3 - v1);

			// compute normalized cross product
			Eigen::Vector3d crossProduct = v1_v2.cross(v1_v3);
			crossProduct.normalize();

			// store into the triangle normal list
			triangleNormalList[i][0] = crossProduct[0];
			triangleNormalList[i][1] = crossProduct[1];
			triangleNormalList[i][2] = crossProduct[2];
		}
	}
}
