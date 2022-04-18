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


//// ------------------------------------------------------------------------------------




bool draw_hat = true;

//class Hat
//{
//public:
//
//	int w;
//	int hdSz;// 0,1,2,3,
//	float r, g, b; // 0.1,0,2 
//	bool on; // true or false
//	string name; // string of characters;
//
//	///---------------------------
//
//	void drawMyself()
//	{
//		// write code to drawitself;
//		zPoint a(w, 0, 0);
//		zPoint b(w+5, 0, w + 5);
//
//		
//		zColor clr(0, 0, r, 1);
//		disp.drawLine(a, b, clr);
//	}
//};

class box
{
public:

	float w, d, h;
	zPoint center;
	zUtilsDisplay disp;

	void drawMyself()
	{
		zPoint maxBB(center.x + d * 0.5, center.y + w * 0.5, center.z + h * 0.5) ;
		zPoint minBB(center.x - d * 0.5, center.y - w * 0.5, center.z - h * 0.5) ;

		disp.drawCube(minBB, maxBB);

		disp.drawPoint(center, zColor(1, 0, 1,1), 15);
	}
};

box testBox;;

box myArrayofBoxes[10];


////////////////_______________ MVC
// model view controller paradigm of software
// 
// action : when should it happen ? when i press a key ? when i press a mouse button ?
//


void setup() // run once when the software opens
{


	//
	// 
	int myArray[6];

	//for loop
	for (int i = 0; i < 6; i+= 1 )
	{
		myArray[i] = i * 5.2;
		
	}

	for (int i = 0; i < 6; i += 1)
	{
		
		cout << "hello " << i << "-- " << myArray[i] << endl;
	}

	// arrays 

	// classes, objects / instances;
	// C++, python, C#, javaScript etc
	// // Vs
	// C, haskell, fortran

	
	//redhat.r = 0.9; // change and store some property of the instance
	//redhat.w = 5;
	//tallHat.w = 10;// change and store some property of the instance


	//cout << redhat.r << endl; // recall or reuse the stored property
	//cout << tallHat.w << endl; // recall or reuse the stored property

	testBox.w = 5;
	testBox.h = 15;
	testBox.d = 3;

	for (int i = 0; i < 10; i++)
	{
		myArrayofBoxes[i].w = 1;
		myArrayofBoxes[i].h = 1;
		myArrayofBoxes[i].d = 1;

		myArrayofBoxes[i].center = zPoint(i*1.1, 0, 0);

	}
}

// 100 times a second
void update(int value) 
{
	
}

// happens 100 times a second
void draw()
{
	backGround(0.8);
	drawGrid(50);

	glPushMatrix();


	glScaled(10, 10, 10);

	
	if (draw_hat == true)
	{
		//testBox.drawMyself();
		for (int i = 0; i < 10; i++)
		{
			myArrayofBoxes[i].drawMyself(); 
		}
	}

	




}

float z = 0.0;

void keyPress(unsigned char k, int xm, int ym)
{

	// voxel grids - creation and manipulation
	if (k == 'j') // k = 'j'
	{
		
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