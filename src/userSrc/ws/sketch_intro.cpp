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
zPoint a, b, o;
zVector center;
zTransform tmat;
zPointArray myPointArray;



void setup() // EVENT 
{

	// local scope --> meaning the name and contents are only visible within setup ;
	a = zPoint(-5, 5, 0);
	b = zPoint(5, 5, 0);
	o = zPoint(0, 0, 0);

	// arrays ;
	myPointArray.push_back(a); // adding a point into myPointArray ;
	myPointArray.push_back(b);
	myPointArray.push_back(o);

	// construct the **transformation matrix** to represent the rotated and translated **frame**

	center = zVector(15, 5, 0);

	zVector xAxis, yAxis, zAxis;

	//
	xAxis = zVector(1, -5, 0); // xAxis of the frame
	zAxis = zVector(0, 0, 1); // zAxis of the frame
	yAxis = xAxis ^ zAxis; // use cross product to calculate the yAxis of the frame
	
	// normalise the axes;
	xAxis.normalize();
	yAxis.normalize();
	zAxis.normalize();

	// insert the axes into the columns of hte transformation matrix ;
	tmat.col(0) << xAxis.x, xAxis.y, xAxis.z, 1;
	tmat.col(1) << yAxis.x, yAxis.y, yAxis.z, 1;
	tmat.col(2) << zAxis.x, zAxis.y, zAxis.z, 1;
	tmat.col(3) << center.x, center.y, center.z, 1;

	//a = a * tmat; // multiply orignal point with the transformation matrix to get the new point
	//b = b * tmat;
	//o = o * tmat;
	// use the transformation matrix, to transform the original points to their new location i.e to the new frame.

	
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



	for (int i = 0; i < 3; i++)
	{
		Dsp.drawPoint(myPointArray[i], zColor(1, 0, 0, 1), 8);
	}
	
}


void keyPress(unsigned char k, int xm, int ym)
{
	
	for (int i = 0; i < myPointArray.size(); i++)
	{
		myPointArray[i] = myPointArray[i] * tmat;
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
