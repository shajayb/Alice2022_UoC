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
zPoint b;
zVectorArray pts;

IO fileImporter;


////////////////_______________ MVC

void setup() // EVENT 
{

	// USES of the . notation
	// object.action()
	// object.property

	aPt.x = 5; // setting the property ( in this case the x coordinate of aPt) ;

	
	fileImporter.read("data/voxGrid.txt", pts);// executing an action
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

	


	for ( int i = 0; i <= 200; i+=50 )
	{

		zPoint pt(i, i, 25);
		Dsp.drawPoint(pt, red, 5);
	}


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
