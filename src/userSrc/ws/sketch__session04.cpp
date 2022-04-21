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


zUtilsCore core; // a object class with set of math utilities, such as remapping ;
zUtilsDisplay disp;
bool draw_hat = true;




////////////////_______________ MVC
// model view controller paradigm of software
// 
// action : when should it happen ? when i press a key ? when i press a mouse button ?
//

// objects or classes needed to import a OBJ mesh, display it and work with it;

zObjMesh o_mesh; // a object class to store mesh type of data (V,E,F)
zFnMesh fnMesh; // another object class to manipulate (read / writing) the information stored in the class above;


zPointArray vPos;

//zFnGraph fnGraph;
//zObjGraph o_graph;

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


	fnMesh = zFnMesh(o_mesh);
	fnMesh.from("data/grid_maya.obj", zSpace::zOBJ, true);



	// instance.action()
	//API - application programming interface;
	// 

	
	o_mesh.setDisplayElements(true, true, true);
	//instance.action();
	// instance.property = value;

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
	wireFrameOn();

		o_mesh.draw();
	
	wireFrameOff();


	zIntArray eVertIds;
	zPointArray vPosns; // create empty input array of type zPoint;
	fnMesh.getVertexPositions(vPosns); // send the empty array as in put to method();
	// at this point in the executiong of code, the array vPosns will be filled with the vertex positions (x,y,z) of the mesh stored in o_mesh ;
	
	zVector up(0, 0, 1);
	zVector sunDir(1, 1, 4);
	zScalarArray edgeAngles;

	
	for (zItMeshEdge E(o_mesh); !E.end(); E++)
	{
		//zItMeshEdge E(o_mesh);// initialise to edge 0;


		if (!E.onBoundary())continue;

		// step 1 - get center
		zPoint cen = E.getCenter();// instance.action() ;
		disp.drawPoint( cen, zColor(1, 0, 0, 1), 8);

		// step 2 - get edge vertices
		eVertIds.clear();
		E.getVertices(eVertIds);

		// print to console or console out or cout 
		//cout << eVertIds[0] << " -- " << eVertIds[1] << endl;;
		int idOfV0 = eVertIds[0];
		zPoint V0 = vPosns[ idOfV0 ];

	

		// calculate the vector perpendicular to the edge direction
		zVector O = V0 - cen;
		zVector edgeNorm = O ^ up;

		// calculate the angle between the edgeNormal and the sunDir;
		edgeNorm.normalize();
		sunDir.normalize();

		float CosineOfAngle = edgeNorm * sunDir; // dot product between normlised vectors = cosine of angle between them;
		edgeAngles.push_back(CosineOfAngle);


	}


	// calculate teh min and max range of the angles
	float min;
	float max;
	min = 1000000;
	max = min * -1;

	for (float angle : edgeAngles)
	{
		if (angle < min)min = angle;
		if (angle > max)max = angle;
	}

	// remap the domain from min,max as calculated above, to 0,1
	for (float &angle : edgeAngles)
	{
		angle = core.ofMap(angle,zDomainFloat(min,max), zDomainFloat(0,1));
	}


	// use remapped angles to draw colored edges;
	int cnt = 0;
	for (zItMeshEdge E(o_mesh); !E.end(); E++)
	{

		if (!E.onBoundary())continue;

		// step 1 - get center
		zPoint cen = E.getCenter();// instance.action() ;
		
		// step 2 - get edge vertices
		eVertIds.clear();
		E.getVertices(eVertIds);

		// print to console or console out or cout 
		//cout << eVertIds[0] << " -- " << eVertIds[1] << endl;;
		int idOfV0 = eVertIds[0];
		zPoint V0 = vPosns[idOfV0];

		// calculate the vector perpendicular to the edge direction
		zVector O = V0 - cen;

		// use remapped edge angles to draw the edge in a color that represents the angle per edge
		zColor edgeColor(edgeAngles[cnt], 0, 0, 1);
		disp.drawLine(cen, cen + O, edgeColor, 8);

		cnt++;
	}



}

// 100 times a second
void update(int value)
{

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