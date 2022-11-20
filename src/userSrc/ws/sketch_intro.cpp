#define _MAIN_
#ifdef _MAIN_

#include "main.h"

//// zSpace Core Headers
#include <headers/zApp/include/zObjects.h>
#include <headers/zApp/include/zFnSets.h>
#include <headers/zApp/include/zViewer.h>

using namespace zSpace;

#include <VoxelFactory.h>
#include "spatialBin.h"
#include "RenderMesh.h"
#include "InputOutput.h"

////////////////////////////////////////////////////////// MVC APP


// model view controller

//// ------------------------------------------------------------------------------------
zUtilsDisplay Dsp; // global definitions 
zPoint aPt;
zColor blue;
zColor red;
zVectorArray pts;

IO fileImporter;


////////////////_______________ MVC

// vectors and transformations.
// transfromation = translation, rotation, scale

// global scope of a variable --> meaning the names and the contents of the variable are visible everywhere in the code.

// **** mathematical concepts **** 
// vectors & transformation (matrix)
// vectors : add,substact, cross product, dot product
// 
// transform : 
// new point = old point * transform matrix
// columns of hte transform matrix are the x,y,z axes and the origin of the frame ;

// *** programming concepts ****
// variable ?
// MVC --> setup, update, draw, keypress etc;
// global scope and local scope of variable
// class or object orient programming

//class-name instance name;


class voxelFactory
{
public:

	// properties / attributes / fields
	zFnMesh meshFn; // a class that allows you to query a mesh
	zObjMesh omesh; // a class to store the mesh in memomry
	zTransform tmat;

	// actions

	//1. load mesh

	void loadMesh( string fileToLoad )
	{
		meshFn = zFnMesh(omesh); // initiliase meshFn, similar to zPoint ;
		//meshFn.from("data/test.obj", zSpace::zOBJ); // use meshFn to import the mesh;
		meshFn.from(fileToLoad, zSpace::zOBJ); // use meshFn to import the mesh;
	}

	// set transform matrix
	// 
	void setTransform( zVector center , zVector xAxis ) // argument or input for the action ;
	{
		// construct or create the transform matrix ;
		tmat.setIdentity(); // set default x,y,z axis;

		//zVector xAxis(1, 1, 0);
		zVector zAxis(0, 0, 1);
		zVector yAxis = xAxis ^ zAxis;

		xAxis.normalize();
		zAxis.normalize();
		yAxis.normalize();

		tmat.col(0) << xAxis.x, xAxis.y, xAxis.z, 1;
		tmat.col(1) << yAxis.x, yAxis.y, yAxis.z, 1;
		tmat.col(2) << zAxis.x, zAxis.y, zAxis.z, 1;

		tmat.col(3) << center.x, center.y, center.z, 1; //we are the setting the origin of the frame to 5,5,5 ;


	}
	// 2. trasnfromMesh

	void transformMesh()
	{
		// get the current locations of the vertices of the mesh
		zPointArray meshVertexPositions;
		meshFn.getVertexPositions(meshVertexPositions); // use meshFn, and use the action getVertexPositions to retireve the location of all the vertices(zPoint);

		// apply the transform matrix to the current locations of hte mesh vertices, to calculate the new positions of the mesh vertices;
		for (int i = 0; i < meshVertexPositions.size(); i++)
		{
			zPoint oldPoint = meshVertexPositions[i];
			zPoint newPoint = oldPoint * tmat;

			//
			meshVertexPositions[i] = newPoint;
		}

		// apply the new positions to the mesh vertices;
		meshFn.setVertexPositions(meshVertexPositions);
	}

	//draw itself

	void draw()
	{
		omesh.draw();
	}
};



//class-name instance name; you need an isntance of a class to be able to trigger the actions defined inside the class;
voxelFactory testVoxel;
voxelFactory testVoxel2;

void setup() // EVENT 
{

	//instance.action();

	testVoxel.loadMesh("data/test_plato.obj");
	testVoxel2.loadMesh("data/test.obj");
}



// 100 times a second // FRAME RATE
void update(int value)
{

	
}
	
// happens 100 times a second
void draw()
{
	backGround(0.8);
	drawGrid(50);

	testVoxel.draw();
	testVoxel2.draw();
}


void keyPress(unsigned char k, int xm, int ym)
{
	
	if (k == 'm')
	{
		//do something .. i.e move mesh
		// what are teh ingredients to move a mesh ?
		// transform matrix --> origin of the frame , x,y,z axes of the frame;
		// new point = old point * transform matrix
		
		testVoxel.setTransform( zVector(5,5,0), zVector(1,5,0));
		testVoxel.transformMesh();

		testVoxel2.setTransform( zVector(25, 25, 0) , zVector( 1,1,0) );
		testVoxel2.transformMesh();
		
	}
}

void mousePress(int b, int state, int x, int y)
{


}

void mouseMotion(int x, int y)
{



}
#endif // _MAIN_

//
//
