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
zVector corner;
zTransform tmat;
zTransform tmat_forBox;
zPointArray myPointArray;

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

class triangle
{
public:

	// properties
	zPoint v1, v2, v3;
	zTransform tmat;
	zUtilsDisplay display;

	//actions or methods;
	void drawYourself()
	{
		display.drawPoint(v1, zColor(0, 0, 0, 1), 5);
		display.drawPoint(v2, zColor(0, 0, 0, 1), 5);
		display.drawPoint(v3, zColor(0, 0, 0, 1), 5);

		display.drawLine(v1, v2, zColor(0,0,1,1));
		display.drawLine(v2, v3);
		display.drawLine(v3, v1);
	}


	void transformYourSelf()
	{
		// new point = old point * tranform matrix ;
		v1 = v1 * tmat;
		v2 = v2 * tmat;
		v3 = v3 * tmat;
	}


};

triangle redT;
triangle blueT;

vector<triangle> mytriangleArray;

void setup() // EVENT 
{



	for (int i = 0; i < 10; i++)
	{
		triangle aTriangle;
		aTriangle.v1 = zPoint(-5, 5, 0);
		aTriangle.v2 = zPoint(5, 5, 0);
		aTriangle.v3 = zPoint(0, 0, 0);

		aTriangle.tmat.col(0) << 1, 0, 0, 1; // xAxis of frame
		aTriangle.tmat.col(1) << 0, 1, 0, 1; // yAxis of the frame
		aTriangle.tmat.col(2) << 0, 0, 1, 1; // zAxis of frame

		aTriangle.tmat.col(3) << float(i)*15.0, 0, 0, 1; // origin of frame

		
		mytriangleArray.push_back(aTriangle);
	}



	
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

	// trigger the action of drawing --> call the method of the class
	/*redT.drawYourself();
	blueT.drawYourself();*/

	for (int i = 0; i < mytriangleArray.size(); i++)
	{
		mytriangleArray[i].drawYourself();
	}
}


void keyPress(unsigned char k, int xm, int ym)
{
	
	if (k == 'm')
	{
		for (int i = 0; i < mytriangleArray.size(); i++)
		{
			mytriangleArray[i].transformYourSelf();
		}

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
