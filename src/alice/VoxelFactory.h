
#define _VOXEL_FACTORY_
#ifdef _VOXEL_FACTORY_

#include "main.h"

//// zSpace Core Headers
#include <headers/zApp/include/zObjects.h>
#include <headers/zApp/include/zFnSets.h>
#include <headers/zApp/include/zViewer.h>

//#include "RenderMesh.h";



#define voxDIM 8

class voxel
{
public:


	zPointArray pts, origPts;
	zIntArray pConnects;
	zIntArray pCounts;
	zColorArray vertexColors;

	zObjMesh o_mesh; // create an instance, called mesh, of class zObjMesh
	zFnMesh meshFn; // create an instance, called meshFn, of class zFnMesh
	zTransform mat;
	float* mat_asArray;
	////_____
	

	zUtilsCore core;
	zUtilsDisplay utils;

	zVector minBB, maxBB;
	int id;

	//

	float voxelParam;
	float voxelId;


	////_______________

	voxel()
	{
		minBB = zVector(-0.5, -0.5, -0.5);
		maxBB = minBB * -1;

		mat.setIdentity();
	
	}

	////_______________

	void initiliase()
	{

	}

	void update( float diagLength = 2)
	{
		zVector cen( mat(0, 3), mat(1, 3), mat(2, 3) );
		
		//cout << cen.z << " --- cenz" << endl;

		//if (cen.z < .1)
		//{
		//	createMeshFromFile("data/typeA.obj");
		//	//cout << " typeA" << endl;
		//}

		//if (cen.z > .5)
		//{
		//	createMeshFromFile("data/typeB.obj");
		//	
		//	//cout << " typeB" << endl;
		//}


		//if (cen.z > .1 && cen.z < .5)
		//{
		//	createMeshFromFile("data/typeC.obj");
		//	//cout << " typeC" << endl;
		//	
		//}

		// create crss3d mesh
		createMeshFromFile("data/cross3d.obj");
		transformMesh(mat);
		
		//--------------------- trasnfromPArtofMesh

		//contruct array with vertex ids of the vertices that need to be trasnfromed
		int src[] = { 0,1,2,3,4,5,6,7 };// cross 3d
		int n = sizeof(src) / sizeof(src[0]);
		zIntArray ids(src, src + n);

		// construct a gradient parameter
		zVector u, v, w, c;
		c = cen;
		float maxL = diagLength;
		float mult = ofMap(id, 0, 3097, 0.1, 4); ;// 4 - ofMap(cen.length(), 0, maxL, 0.1, 4);

		//cout << mult << "--mult" << endl;

		//construct the transformation matrix
		zTransform TM;
		TM.setIdentity();
		u = zVector(1, 0, 0);
		v = zVector(0, 1, 0);
		w = zVector(0, 0, 1);
		u.normalize(); v.normalize(); w.normalize();
		u *= mult; v *= mult; w *= mult;

		//assign the values to the matrix
		TM.col(0) << u.x, u.y, u.z, 1;
		TM.col(1) << v.x, v.y, v.z, 1;
		TM.col(2) << w.x, w.y, w.z, 1;
		TM.col(3) << 0, 0, 0, 1;

		// use transfrom matrix and ids, to transform part of the mesh ;
		transformPartOfMesh(ids, TM);
	}

	void updateMatrix( zTransform tmat)
	{
		mat = tmat;
	}


	////_______________

	void clearArrays()
	{
		pts.clear();
		pConnects.clear();
		pCounts.clear();


	}

	void createMeshFromFile(string obj)
	{
		clearArrays();
		
		// load a mesh using zFnMesh and zObjMesh
		meshFn = zFnMesh(o_mesh);
		meshFn.from(obj, zSpace::zOBJ);

		// getting the original positions of the vertices, and storing them in a ZVectorArray called origPts;
		meshFn.getVertexPositions(origPts);
	}

	void copyMesh(zPointArray& PT, zIntArray& PC, zIntArray& POC)
	{
		meshFn.getVertexPositions(pts);
		meshFn.getPolygonData(pConnects, pCounts);

		for (int i = 0; i < pConnects.size(); i++)
		{
			POC.push_back(pConnects[i] + PT.size());
		}

		for (int i = 0; i < pts.size(); i++)
		{
			PT.push_back(pts[i]);
		}

		for (int i = 0; i < pCounts.size(); i++)
		{
			PC.push_back(pCounts[i]);
		}
	}

	void transformPartOfMesh(zIntArray& ids, zTransform& tmat)
	{

		// get the current positions of the vertices, store them in pts array;
		meshFn.getVertexPositions(pts);

		// transform each point
		for (int i = 0; i < ids.size(); i++)
		{
			pts[ids[i]] = origPts[ids[i]] * tmat;
		}
		//
		for (int i = 0; i < ids.size(); i++)
		{
			pts[ids[i]] = pts[ids[i]] * mat;
		}


		// set the trasnformed points as the new vertex positions;
		meshFn.setVertexPositions(pts);

	}

	void transformMesh(zTransform tmat)
	{

		mat = tmat;
		// get the current positions of the vertices, store them in pts array;
		meshFn.getVertexPositions(pts);

		// transform each point
		for (int i = 0; i < pts.size(); i++)
		{
			pts[i] = origPts[i] * tmat;
		}

		// set the trasnformed points as the new vertex positions;
		meshFn.setVertexPositions(pts);
	}

	void ConvertMatrixToArray()
	{
		mat_asArray = mat.data();
		
	}

	void applyVertexColors()
	{

		vertexColors.resize(pts.size());

		// transform each point
		for (int i = 0; i < meshFn.numVertices(); i++)
		{
		//	vertexColors[i] = zColor(getColour(pts[i].z, -voxDIM * 0.5, voxDIM * 0.5).r, getColour(pts[i].z, -voxDIM * 0.5, voxDIM * 0.5).g, getColour(pts[i].z, -voxDIM * 0.5, voxDIM * 0.5).b, 1);
		}

		meshFn.setVertexColors(vertexColors, true);

	}


	//____________________
	zTransform matTmp;
	void display()
	{
		

		o_mesh.draw();

		wireFrameOn();
				utils.drawCube(minBB*mat, maxBB*mat );
		wireFrameOff();
		
		zVector cen( mat(0, 3), mat(1, 3), mat(2, 3) );
		utils.drawPoint(cen, zColor(1,0,0,1),1);
	}
};


#endif 
//_VOXEL_FACTORY_