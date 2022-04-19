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



zUtilsDisplay disp;
bool draw_hat = true;


class box
{
public:

	float w, d, h;
	zPoint center;
	zUtilsDisplay disp;

	box() // constructor 
	{
		w = d = h = 1;
		center = zPoint(1, 1, 1);
	}

	void drawMyself()
	{
		zPoint maxBB(center.x + d * 0.5, center.y + w * 0.5, center.z + h * 0.5) ;
		zPoint minBB(center.x - d * 0.5, center.y - w * 0.5, center.z - h * 0.5) ;

		disp.drawCube(minBB, maxBB);

		disp.drawPoint(center, zColor(1, 0, 1,1), 15);
	}
};

box testBox;;
box myArrayofBoxes[10][10][10];


////////////////_______________ MVC
// model view controller paradigm of software
// 
// action : when should it happen ? when i press a key ? when i press a mouse button ?
//

// objects or classes needed to import a OBJ mesh, display it and work with it;

zObjMesh o_mesh; // a class of object to store mesh type of data (V,E,F)
zFnMesh fnMesh; // another class of object to manipulate (read / writing) the information stored in the class above;


void setup() // run once when the software opens
{

	// define centers of each voxel ;
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			for (int k = 0; k < 10; k++)
			{
				myArrayofBoxes[i][j][k].center = zPoint(i, j, k);

			}
		}
	}

	testBox.w = 1;;
	

	//o_mesh.vertex[1].x = 2;
	//o_mesh.vertex[1].x = 5;
	//o_mesh.vertex[1].x = 27

	// attached zObjMesh to ZFnMesh;
	fnMesh = zFnMesh(o_mesh);

	//use zFnMesh to manipoulate zObjMesh;
	fnMesh.from("data/cross3d.obj", zSpace::zOBJ);
	
	// instance.action() ;
	// instance name = fnmesh
	// action  = from()


}


// happens 100 times a second
void draw()
{
	// set background color to grey
	backGround(0.8);
	// draw a 50 x 50 square grid on the ground plane
	drawGrid(25);
	drawGrid(50);

	//draw mesh;

	o_mesh.draw();

	//
	zPointArray vPos; // vPos = empty array of zPoints
	fnMesh.getVertexPositions(vPos); // fill the empty array of zPOints with the zPoint of the vertices of mesh we imported;

	for (int i = 0; i < vPos.size(); i++)
	{
		zPoint a = vPos[i];
		zPoint b = a;
		b.z += 3;

		disp.drawLine(a, b, zColor(1, 0, 0, 1), 3);
	}

	

}

// 100 times a second
void update(int value)
{

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