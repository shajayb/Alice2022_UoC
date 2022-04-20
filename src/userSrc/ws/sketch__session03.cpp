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

zObjMesh o_mesh; // a object class to store mesh type of data (V,E,F)
zFnMesh fnMesh; // another object class to manipulate (read / writing) the information stored in the class above;
zPointArray vPos;

void drawFaceEdges( zPointArray &FV )
{
	int start = 0;
	for (auto vp : FV) // for every vertex position in fvPositions, do something;
	{
		disp.drawPoint(vp, zColor(0, 0, 1, 1), 10);

		int currentVertex = start; //0
		int nextVertex = ((currentVertex + 1) % (FV.size()));// if cv = 3, nv = 4 // modulus


		zPoint a = FV[currentVertex];
		zPoint b = FV[nextVertex];

		a.z += 1; b.z += 1; ;

		disp.drawLine(a, b, zColor(1, 0, 0, 1), 3);

		start++;
	}
}

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
	fnMesh.from("data/grid_maya.obj", zSpace::zOBJ);
	
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
	// iterator --> iterates over mesh faces
	// zItMeshVertex --> iterate over mesh vertices;

	zPointArray fvPositions;

	/* ex-1 : 
	 from each face in the mesh, get connected vertices; 
	 for each vertex in connected vertices, draw a line from the current vertex to the next one;*/
	  
	/* ! <-- NOT operator
	 apple = red;
	 do something if apple is not red;
	 if( ! (apple == red ))

	 do something, if apple is red
	 if(  (apple == red ))*/

	//for (zItMeshFace f(o_mesh); !f.end(); f++)
	{
		zItMeshFace f(o_mesh); // a object class to iterate over mesh faces stored in o_mesh ;
		// f is initiated to face 0;
		
		fvPositions.clear();
		f.getVertexPositions(fvPositions); // fill the empty array we created to store fave-vertex positions with the vertex positions of face 0;
		
		//drawFaceEdges(fvPositions); // function or method ;
		
	}



	//ex - 2 ;
	// for each face in mesh, get connected faces;
	// for each connected face ---> repeat action from ex-1;

	zItMeshFaceArray connectedFaces;
		
	//for (zItMeshFace f(o_mesh); !f.end(); f++)
	{
		zItMeshFace f(o_mesh); // a object class to iterate over mesh faces stored in o_mesh ;
		// f is initiated to face 0;

		connectedFaces.clear();
		f.getConnectedFaces(connectedFaces);;// this will fill the mepty array with the mesh faces that are conencted to f ;

		for (auto cf : connectedFaces) // for each cf in connectedFaces --> for each connected face, do something
		{
			fvPositions.clear();
			cf.getVertexPositions(fvPositions); // fill the empty array we created to store fave-vertex positions with the vertex positions of face 0;
				
			drawFaceEdges(fvPositions);

		}

	}


	// if you want to get to a specific face, say face 5;
	// iterators only allow to go in sequence, 
	// for example  cannot directly access element 5 before going thorugh 0,1,2,3,4 

	int cnt = 0;

	for (zItMeshFace f(o_mesh); !f.end(); f++)
	{

		//if ( ( cnt < 5 ) && cnt < 2  ) // redundant ;
		//if ( ( cnt <5 ) || cnt > 15  )
		if ( ( cnt == 5 )  ) // && is the AND operator , ! is the NOT operator; || is the OR operator
		{
			connectedFaces.clear();
			f.getConnectedFaces(connectedFaces);;// this will fill the mepty array with the mesh faces that are conencted to f ;

			for (auto cf : connectedFaces) // for each cf in connectedFaces --> for each connected face, do something
			{
				fvPositions.clear();
				cf.getVertexPositions(fvPositions); // fill the empty array we created to store fave-vertex positions with the vertex positions of face 0;

				drawFaceEdges(fvPositions);

			}
		}

		cnt++;
	}

	//zFnGraph fnGraph;
	//zObjGraph o_graph;

	//fnGraph.addVertex()

}

// 100 times a second
void update(int value)
{

}

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
////		for (auto cf : connectedFaces)
//		{
//			fvPositions.clear();
//			cf.getVertexPositions(fvPositions);
//			for (int i = 0; i < fvPositions.size(); i++)
//			{
//				zPoint a = fvPositions[i];
//				zPoint b = fvPositions[(i + 1) % fvPositions.size()];
//
//				a.z += 1;
//				b.z += 1;
//
//				disp.drawLine(a, b);
//
//			}
//		}