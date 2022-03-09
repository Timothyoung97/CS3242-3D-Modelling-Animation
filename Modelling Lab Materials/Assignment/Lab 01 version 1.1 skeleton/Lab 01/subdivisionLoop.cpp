#ifndef subdivisionLoop_h
#define M_PI 3.141592654

#include <cstdio>
#include "subdivisionLoop.h"
#endif /* subdivisionLoop_h */

namespace subdivisionLoop
{
	/// <summary>
	/// A function to check if two numbers are almost equal using the machine epsilon
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="x">First number</param>
	/// <param name="y">Second number</param>
	/// <param name="ulp">Unit in last place</param>
	/// <returns></returns>
	template <class T>
	std::enable_if_t<!std::numeric_limits<T>::is_integer, bool> approxEqual(T x, T y, int ulp)
	{	
		// the machine epsilon is scaled to the magnitude of the values used
		// and multiple with the desired precision in the ulps (units in the last place)
		return std::abs(x - y) <= std::numeric_limits<T>::epsilon() * std::abs(x + y) * ulp
			// unless the result is subnormal
			|| std::abs(x - y) < std::numeric_limits<T>::min();
	}

	/// <summary>
	/// A function to add a vertex to a list of vertices then return the designated index. If vertex already exists, we return the designated index.
	/// </summary>
	/// <param name="vertexList">Vertex List</param>
	/// <param name="vertexCount">Number of vertex</param>
	/// <param name="vertex">Vertex to be inserted</param>
	/// <returns>Index of the newly inserted vertex</returns>
	std::pair<bool, int> appendVertexToVertexList(double vertexList[MAXV][3], int vertexCount, Eigen::Vector3d vertex)
	{
		for (int i = 1; i <= vertexCount; i++)
		{	
			// if the new vertex is almost identical to one vertex in the vertexList, then there is no need to append it into the vertex list
			if (approxEqual(vertexList[i][0], vertex[0], 2) && approxEqual(vertexList[i][1], vertex[1], 2) && approxEqual(vertexList[i][2], vertex[2], 2))
			{
				return std::make_pair(false, i);
			}
		}

		// If reached here, we can proceed to add the new vertex into the vertex list. Since this ensures that the new vertex to be added is an essential vertex.
		vertexList[vertexCount + 1][0] = vertex[0];
		vertexList[vertexCount + 1][1] = vertex[1];
		vertexList[vertexCount + 1][2] = vertex[2];
		return std::make_pair(true, vertexCount + 1);
	}

	/// <summary>
	/// A function that adds a triangle to the triangle list
	/// </summary>
	/// <param name="triangleList">Triangle List</param>
	/// <param name="triangleCount">Number of triangle</param>
	/// <param name="vertexIndices">Triangle indices to be inserted</param>
	void appendTriangleToTriangleList(int triangleList[MAXV][3], int triangleCount, Eigen::Vector3i vertexIndices)
	{
		triangleList[triangleCount + 1][0] = vertexIndices[0];
		triangleList[triangleCount + 1][1] = vertexIndices[1];
		triangleList[triangleCount + 1][2] = vertexIndices[2];
	}

	/// <summary>
	/// Reference: http://www.cs.cmu.edu/afs/cs/academic/class/15462-s14/www/lec_slides/Subdivision.pdf pg7-10
	/// This function is for odd vertices - Calculating of a new vertex for the interior
	/// A function that returns a new vertex in subdivision loop for the interior
	/// For each edge, computes v = 3/8 * (a+b) + 1/8 * (c+d)
	/// </summary>
	/// <param name="vertexList">Vertex list</param>
	/// <param name="vertex1IdxOfEdge">Vertex index of an edge</param>
	/// <param name="vertex2IdxOfEdge">Partner vertex index of an edge</param>
	/// <param name="adjacentVertex1">Vertex Index that is next to the edge </param>
	/// <param name="adjacentVertex2">Vertex Index that is next to the edge</param>
	/// <returns>New Vertex</returns>
	Eigen::Vector3d generateOddLoopVertexInterior(double vertexList[MAXV][3], int vertex1IdxOfEdge, int vertex2IdxOfEdge, int adjacentVertex1, int adjacentVertex2)
	{
		Eigen::Vector3d edgeVertex1(vertexList[vertex1IdxOfEdge][0], vertexList[vertex1IdxOfEdge][1], vertexList[vertex1IdxOfEdge][2]);
		Eigen::Vector3d edgeVertex2(vertexList[vertex2IdxOfEdge][0], vertexList[vertex2IdxOfEdge][1], vertexList[vertex2IdxOfEdge][2]);
		Eigen::Vector3d adjVertex1(vertexList[adjacentVertex1][0], vertexList[adjacentVertex1][1], vertexList[adjacentVertex1][2]);
		Eigen::Vector3d adjVertex2(vertexList[adjacentVertex2][0], vertexList[adjacentVertex2][1], vertexList[adjacentVertex2][2]);
		return 3.0 / 8.0 * (edgeVertex1 + edgeVertex2) + 1.0 / 8.0 * (adjVertex1 + adjVertex2); // v = 3.0 / 8.0 * (a + b) + 1.0 / 8.0 * (c + d)
	}

	/// <summary>
	/// Reference: http://www.cs.cmu.edu/afs/cs/academic/class/15462-s14/www/lec_slides/Subdivision.pdf pg7-10
	/// This function is for odd vertices - Calculating of new vertex for the boundary
	/// A function that returns a new vertex in subdivision loop for the boundary
	/// For each edge, computes v= (a + b) / 2
	/// </summary>
	/// <param name="vertexList">Vertex list</param>
	/// <param name="vertex1Index">Vertex index of an edge</param>
	/// <param name="vertex2Index">Partnering index of an edge</param>
	/// <returns>New vertex</returns>
	Eigen::Vector3d generateOddLoopVertexBoundary(double vertexList[MAXV][3], int vertex1Index, int vertex2Index)
	{
		Eigen::Vector3d edgeVertex1(vertexList[vertex1Index][0], vertexList[vertex1Index][1], vertexList[vertex1Index][2]);
		Eigen::Vector3d edgeVertex2(vertexList[vertex2Index][0], vertexList[vertex2Index][1], vertexList[vertex2Index][2]);
		return (edgeVertex1 + edgeVertex2) / 2.0; // v = 1.0 / 2.0 * (a + b)
	}

	/// <summary>
	/// Reference: http://www.cs.cmu.edu/afs/cs/academic/class/15462-s14/www/lec_slides/Subdivision.pdf pg7-11
	/// This function is for even vertices - Calculating of a new vertex for the interior
	/// A function that returns a new vertex in subdivision loop for the interior
	/// </summary>
	/// <param name="vertexList">Vertex list</param>
	/// <param name="originalVertex">Original Vertex</param>
	/// <param name="neighboringVertexIndices">set of all neighboring vertices</param>
	/// <param name="version">B = {2 options, refer to pg15 of the referenced material}</param>
	/// <returns>New vertex</returns>
	Eigen::Vector3d generateEvenLoopVertexInterior(double vertexList[MAXV][3], int originalVertex, std::set<int> neighboringVertexIndices, int version)
	{
		Eigen::Vector3d ogVertex(vertexList[originalVertex][0], vertexList[originalVertex][1], vertexList[originalVertex][2]);
		double n = neighboringVertexIndices.size(); // get the number of neighbors that the originalVertex has

		double B = 0;
		if (n == 3.0) // if n = 3, B = 3/16
		{
			B = 3. / 16;
		}
		else // if n > 3
		{
			if (version == 1)
			{
				B = 3.0 / 8 / n; // Warran
			}
			else
			{
				B = 1. / n * (5. / 8 - pow(3. / 8 + 1. / 4 * cos(2. * M_PI / n), 2)); // original choice of Loop[16]
			}
		}

		Eigen::Vector3d sum(0.0, 0.0, 0.0);
		for (auto& vertexIndex : neighboringVertexIndices)
		{
			Eigen::Vector3d vertex(vertexList[vertexIndex][0], vertexList[vertexIndex][2], vertexList[vertexIndex][2]);
			sum += vertex;
		}
		return ogVertex * (1 - n * B) + sum * B; //v = v * ( 1 - k * BETA) + (sum of all k neighbor vertices) * BETA
	}

	/// <summary>
	/// Reference: http://www.cs.cmu.edu/afs/cs/academic/class/15462-s14/www/lec_slides/Subdivision.pdf pg7-11
	/// This function is for even vertices - Calculating of a new vertex for the boundary
	/// A function that returns a new vertex in subdivision loop for the boundary
	/// </summary>
	/// <param name="vertexList">Vertex List</param>
	/// <param name="originalVertex">Original Vertex</param>
	/// <param name="vertex1Index">First vertex index of an edge</param>
	/// <param name="vertex2Index">Second vertex index of an edge</param>
	/// <returns>New vertex</returns>
	Eigen::Vector3d generateEvenLoopVertexBoundary(double vertexList[MAXV][3], int originalVertex, int vertex1Index, int vertex2Index)
	{
		Eigen::Vector3d vertexOrigin(vertexList[originalVertex][0], vertexList[originalVertex][1], vertexList[originalVertex][2]);
		Eigen::Vector3d vertex1(vertexList[vertex1Index][0], vertexList[vertex1Index][1], vertexList[vertex1Index][2]);
		Eigen::Vector3d vertex2(vertexList[vertex2Index][0], vertexList[vertex2Index][1], vertexList[vertex2Index][2]);

		return 3. / 4 * vertexOrigin + 1. / 8 * (vertex1 + vertex2); // v = 1.0 / 8.0 * (a + b) + 3.0 / 4.0 * (v)
	}
}