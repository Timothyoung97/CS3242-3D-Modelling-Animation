#pragma once

#include <set>
#include <map>
#include <vector>
#include <Eigen/Dense>

// maximum number of vertices and triangles
#define MAXV 1000000
#define MAXT 1000000

//typedef int OrTri;
//typedef int tIdx;
//
//inline OrTri makeOrTri(tIdx t, int version) { return (t << 3) | version; };
//inline tIdx idx(OrTri ot) { return ot >> 3; };
//inline int ver(OrTri ot) { return ot & 0b111; };
//inline OrTri enext(OrTri ot) { return 0; };
//inline OrTri sym(OrTri ot) { return 0; };


class myObjType {
private:
	int vertexCount = 0;
	int triangleCount = 0;

	double vertexList[MAXV][3];		// vertices list: row i -> (x, y, z) coordinates of the i-th triangle
	double vertexNormalList[MAXT][3];	// vertex normal list
	int triangleList[MAXT][3];			// triangle list: row i -> (v1, v2, v3) vertices of the i-th triangle
	double triangleNormalList[MAXT][3]; // triangle normal list
	int fnextList[MAXT][3];				// fnext list: 

	double lmax[3];          // the maximum coordinates of x,y,z
	double lmin[3];          // the minimum coordinates of x,y,z

	//int statMinAngle[18];	// each bucket is  degrees has a 10 degree range from 0 to 180 degree
	//int statMaxAngle[18]; 

	int uniqueCompCount;
	bool toggleSubdivision = false;
	bool toggleDrawnSubdivisionEdge = true;

	std::vector<std::vector<double>> colors;
	std::map<int, int> getComponentID;								// return the componentID for each triangle. 
	std::map<int, std::set<int>> getAdjFacesFromFace;				// get the adjacent faces given a face
	std::map<int, std::set<int>> getAdjFacesFromVertex;				// get the adjacent faces given a vertex
	std::map<int, std::set<int>> getAdjVerticesFromVertex;			// get the adjacent vertices given a vertex
	std::map<std::set<int>, std::set<int>> getAdjFacesFromEdge;		// get the adjacent faces given an edge (vertex1, vertex2)
	std::map<std::set<int>, std::set<int>> getAdjVerticesFromEdge;	// get the adjacent vertices given an edge (vertex1, vertex2)

	void setupAdjList();
	void processNumOfComponents();
	bool orientTriangles();
	void drawEdges();
	void readObjFile(std::string filename);		// To read in a .obj file
	void readOffFile(std::string filename);		// To read in a .off file
	int enext(const int orTri);
	int sym(const int orTri);
	int org(const int orTri);
	int dest(const int orTri);


public:
	myObjType() { 
		vertexCount = 0; 
		triangleCount = 0; 
	};
	void readFile(std::string filename);  // assumming file contains a manifold
	void writeFile(std::string filename);
	void draw(const bool toggleSmooth, const bool toggleEdges, const bool toggleColorComponets);  

	// Subdivision
	void loopSubdivide(int b_version);
	void barycentrixSubdivide();
};


