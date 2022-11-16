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

zPoint a, b, o;
zVector center;

void setup() // EVENT 
{
	a = zPoint(-5, 5, 0);
	b = zPoint(5, 5, 0);
	o = zPoint(0, 0, 0);

	center = zVector(15, 5, 0);

	
}



// 100 times a second // FRAME RATE
void update(int value)
{

	// LHS = RHS
	center.x = center.x + 0.001;

	a = a + center;
	b = b + center;
	o = o + center;
}
	
// happens 100 times a second
void draw()
{
	backGround(0.8);
	drawGrid(50);


	Dsp.drawPoint(a, zColor(1, 0, 0, 1), 8);
	Dsp.drawPoint(b, zColor(0, 0, 1, 1), 8);

	Dsp.drawLine(a, b);
	Dsp.drawLine(a, o);
	Dsp.drawLine(b, o);
	
}


void keyPress(unsigned char k, int xm, int ym)
{
	
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
