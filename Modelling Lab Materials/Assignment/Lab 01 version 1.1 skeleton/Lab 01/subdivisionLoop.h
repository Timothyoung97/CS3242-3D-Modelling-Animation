#pragma once
#include <set>
#include <Eigen/Dense>

#define MAXV 1000000
#define MAXT 1000000
#define EPSILON s

namespace subdivisionLoop
{
	template <class T>
	typename std::enable_if<!std::numeric_limits<T>::isInteger, bool>::type approxEqual(T x, T y, int ulp);
	std::pair<bool, int> appendVertexToVertexList(double vertexList[MAXV][3], int vertexCount, Eigen::Vector3d vertex);
	void addTriangleToTriangleList(int triangleList[MAXV][3], int triangleCount, Eigen::Vector3i vertexIndices);
	Eigen::Vector3d getOddLoopVertex(double vertexList[MAXV][3], int edgeOfVertex1, int adjacentVertex1, int adjacentVertex2);
	Eigen::Vector3d getOddLoopVertexEdge(double vertexList[MAXV][3], int vertex1Index, int vertex2Index);
	Eigen::Vector3d getEvenLoopVertex(double vertexList[MAXV][3], int originalVertex, std::set<int> neighboringVertexIndices, int betaVersion);
	Eigen::Vector3d getEvenLoopVertexEdge(double vertexList[MAXV][3], int originalVertex, int vertex1Index. int vertex2Index);
}