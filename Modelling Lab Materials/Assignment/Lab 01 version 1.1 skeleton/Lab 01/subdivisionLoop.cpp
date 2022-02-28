#ifndef subdivisionLoop_h
#define subdivisionLoop_h
#define M_PI 3.141592654

#include <stdio.h>
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
	/// <param name="limit"></param>
	/// <returns></returns>
	template <class T>
	typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type approxEqual(T x, T y, int ulp)
	{	
		// the machine epsilon is scaled to the magnitude of the values used
		// and multipled with the desired precision in the ulps (units in the last place)
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
			if (approxEqual(vertexList[i][0], vertex[0], 2) && approxEqual(vertexList[i][1], vertex[1], 2) && approxEqual(vertexList[i][2], vertex[2], 2))
			{
				return std::make_pair(false, i);
			}
		}
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
	/// A function that returns a new vertex in subdivision loop when there is an edge-face. For each edge, computes v = (a+b) / 2
	/// </summary>
	/// <param name="vertexList">Vertex list</param>
	/// <param name="vertex1IdxOfEdge">Vertex index of an edge</param>
	/// <param name="vertex2IdxOfEdge">Partner vertex index of an edge</param>
	/// <param name="adjacentVertex1">Vertex Index that is next to the edge </param>
	/// <param name="adjacentVertex2">Vertex Index that is next to the edge</param>
	/// <returns>New Vertex</returns>
	Eigen::Vector3d generateOddLoopVertex(double vertexList[MAXV][3], int vertex1IdxOfEdge, int vertex2IdxOfEdge, int adjacentVertex1, int adjacentVertex2)
	{
		Eigen::Vector3d edgeVertex1(vertexList[vertex1IdxOfEdge][0], vertexList[vertex1IdxOfEdge][1], vertexList[vertex1IdxOfEdge][2]);
		Eigen::Vector3d edgeVertex2(vertexList[vertex2IdxOfEdge][0], vertexList[vertex2IdxOfEdge][1], vertexList[vertex2IdxOfEdge][2]);
		Eigen::Vector3d adjVertex1(vertexList[adjacentVertex1][0], vertexList[adjacentVertex1][1], vertexList[adjacentVertex1][2]);
		Eigen::Vector3d adjVertex2(vertexList[adjacentVertex2][0], vertexList[adjacentVertex2][1], vertexList[adjacentVertex2][2]);
		return 3.0 / 8.0 * (edgeVertex1 + edgeVertex2) + 1.0 / 8.0 * (adjVertex1 + adjVertex2);
	}

	/// <summary>
	/// A function that generate a new vertex in subdivision loop when there is an edge-face. For each edge, computes v=(a + b) / 2
	/// </summary>
	/// <param name="vertexList">Vertex list</param>
	/// <param name="vertex1Index">Vertex index of an edge</param>
	/// <param name="vertex2Index">Partnering index of an edge</param>
	/// <returns>New vertex</returns>
	Eigen::Vector3d generateOddLoopVertexEdge(double vertexList[MAXV][3], int vertex1Index, int vertex2Index)
	{
		Eigen::Vector3d edgeVertex1(vertexList[vertex1Index][0], vertexList[vertex1Index][1], vertexList[vertex1Index][2]);
		Eigen::Vector3d edgeVertex2(vertexList[vertex2Index][0], vertexList[vertex2Index][1], vertexList[vertex2Index][2]);
		return (edgeVertex1 + edgeVertex2) / 2.0;
	}

	/// <summary>
	/// A function that generates a new even vertex in subdivision loop when we do not have a edge-face
	/// </summary>
	/// <param name="vertexList">Vertex list</param>
	/// <param name="originalVertex">Original Vertex</param>
	/// <param name="neighboringVertexIndices">set of all neighbouring vertices</param>
	/// <param name="version">Version == 1 ? 3.0 / 8 / n : 1.0 / n * (5.0 / 8 - pow(3.0 / 8 + 1.0 / 4 * cos(2.0 * M_PI / n), 2))</param>
	/// <returns>New vertex</returns>
	Eigen::Vector3d generateEvenLoopVertex(double vertexList[MAXV][3], int originalVertex, std::set<int> neighboringVertexIndices, int version)
	{
		Eigen::Vector3d ogVertex(vertexList[originalVertex][0], vertexList[originalVertex][1], vertexList[originalVertex][2]);
		double n = neighboringVertexIndices.size();

		double cal = 0;
		if (n == 3.0)
		{
			cal = 3.0 / 16;
		}
		else 
		{
			if (version == 1)
			{
				cal = 3.0 / 8 / n;
			}
			else
			{
				cal = 1.0 / n * (5.0 / 8 - pow(3.0 / 8 + 1.0 / 4 * cos(2.0 * M_PI / n), 2));
			}
		}
		Eigen::Vector3d sum(0.0, 0.0, 0.0);
		for (auto& vertexIndex : neighboringVertexIndices)
		{
			Eigen::Vector3d vertex(vertexList[vertexIndex][0], vertexList[vertexIndex][2], vertexList[vertexIndex][2]);
			sum += vertex;
		}
		return ogVertex * (1 - n * cal) + sum * cal;
	}

	/// <summary>
	/// A function that generates a new even vertex in the subdivision loop when there is an edge-face.
	/// </summary>
	/// <param name="vertexList">Vertex List</param>
	/// <param name="originalVertex">Original Vertex</param>
	/// <param name="vertex1Index">First vertex index of an edge</param>
	/// <param name="vertex2Index">Second vertex index of an edge</param>
	/// <returns>New vertex</returns>
	Eigen::Vector3d generateEvenLoopVertexEdge(double vertexList[MAXV][3], int originalVertex, int vertex1Index, int vertex2Index)
	{
		Eigen::Vector3d vertexOrigin(vertexList[originalVertex][0], vertexList[originalVertex][1], vertexList[originalVertex][2]);
		Eigen::Vector3d vertex1(vertexList[vertex1Index][0], vertexList[vertex1Index][1], vertexList[vertex1Index][2]);
		Eigen::Vector3d vertex2(vertexList[vertex2Index][0], vertexList[vertex2Index][1], vertexList[vertex2Index][2]);

		return 3.0 / 4 * vertexOrigin + 1.0 / 8 * (vertex1 + vertex2);
	}
}