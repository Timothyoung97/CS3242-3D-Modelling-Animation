#pragma once

#include <Eigen/Dense>
#include <set>
#include <map>

#define MAXV 1000000
#define MAXT 1000000
#define EPSILON s

namespace operationLib
{
	int getIndexNotDiscovered(int triangleCount, const std::set<int> tFound);
	std::pair<int, int> getVerticesFromtIdxAndtVersion(int triangleList[MAXT][3], const int tIdx, const int tVersion);
	bool isObjectContainsBoundary(int fnextList[MAXT][3], int triangleList[MAXT][3], int triangleCount);
	bool isDifferentOrientation(int triangleList[MAXT][3], const int tIdx1, const int tVersion1, const int tIdx2, const int tVersion2);
	double computeAngle(const Eigen::Vector3d vector1, const Eigen::Vector3d vector2);
	void generateStatistics(double vertexList[MAXV][3], int vertexCount, int triangleList[MAXT][3], int triangleCount);
	void generateVertexNormals(double vertexNormalList[MAXV][3], double triangleNormalList[MAXT][3], std::map<int, std::set<int>> getVertexFromAdjFace, int vertexCount);
	void generateFaceNormals(double triangleNormalList[MAXT][3], double vertexList[MAXV][3], int triangleList[MAXT][3], int triangleCount);
};