//#define _MAIN_
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

#define FOR3X  for (int i = 0, x = -0.5 * float(voxDIM); i < voxDIM; i++, x += 1.0)\
			   {\
				 for (int j = 0, y = -0.5 * float(voxDIM); j < voxDIM; j++, y += 1)\
				 {\
					for (int k = 0, z = -0.5 * float(voxDIM); k < 1; k++, z += 1)\


#define ENDFOR3X		}\
					}



zVector zScreenToCamera(int x, int y, double zPlane = 0)
{
	double camera_pos[3];
	GLdouble matModelView[16], matProjection[16];
	int viewport[4];
	// get matrices and viewport:
	glGetDoublev(GL_MODELVIEW_MATRIX, matModelView);
	glGetDoublev(GL_PROJECTION_MATRIX, matProjection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	int scrCenX = (viewport[2] - viewport[0]) / 2;
	int scrCenY = (viewport[3] - viewport[1]) / 2;
	gluUnProject
	(
		scrCenX + x, scrCenY + y, zPlane, //screen coords
		matModelView, matProjection, viewport, //mvp matrices
		&camera_pos[0], &camera_pos[1], &camera_pos[2] // return pos
	);

	return zVector(camera_pos[0], camera_pos[1], camera_pos[2]);
}



double background = 0.0;
bool compute = false;

zUtilsCore core;
//voxel vxls[voxDIM][voxDIM][voxDIM];
#define VOX_MAX 50000
voxel VXLS[VOX_MAX];
int NUM_VXL = 0;
voxel testVxl;

zPointArray PT;
zIntArray PC, POC;
zFnMesh MFN;
zObjMesh OM;

zTransform tmat;
zVector u, v, w, c;

float scl = 1;
bool drawOM = false;

RenderMesh RM;
double lightScale = 16;
bool flipNormals = true;
bool drawFaces = false;
bool drawWire = false;
zVector camPt;




////////////////_______________ MVC

void setup()
{

	S.addSlider(&lightScale, "lightScale");
	S.sliders[S.numSliders - 1].maxVal = 2;

	B.addButton(&flipNormals, "flipNormals");
	B.addButton(&drawFaces, "drawFaces");
	B.addButton(&drawWire, "drawWire");

	//cout << " change made in class" << endl;

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



	//draw renderMesh
	if (drawFaces)
		for (int i = 0; i < voxDIM; i++)
		{

			RM.updateColorArray(lightScale, flipNormals, camPt);
			RM.draw(drawFaces, drawWire, false);

		}
	else
		// draw VXLS
		for (int i = 0; i < NUM_VXL; i++)VXLS[i].display();

	glPopMatrix();

	// draw textVXL
	testVxl.display();


}

float z = 0.0;

void keyPress(unsigned char k, int xm, int ym)
{

	// voxel grids - creation and manipulation




	if (k == 'l')updateCam = !updateCam;

	// ----------------------------------------------------------------------------------------------------

	if (k == 'i')
	{

		// read voxGrid - get voxel centers and voxel dims
		IO importer;
		zVectorArray pts;
		zVector dxdydz;
		dxdydz = importer.read("data/points.csv", pts);


		// check if number of voxels <  VXLS array size
		if (pts.size() >= VOX_MAX)cout << " -- insufficient memory " << endl;
		else
			NUM_VXL = pts.size();

		// load test file, get boundingBox of mesh;
		testVxl.createMeshFromFile("data/cross3d.obj");

		zVector mn, mx;
		testVxl.meshFn.getBounds(mn, mx);
		zVector diagMeshBox = mx - mn;

		// calculate scaling factor by coparing diagonals
		zVector diag(0, 0, 0);
		diag += dxdydz;
		float L1 = diag.length();
		float L2 = diagMeshBox.length();
		float scale = L1 / L2;

		//
		// iterate through the voxels : load mesh, transform mesh to voxel size and location

		for (int i = 0; i < NUM_VXL; i++)
		{
			tmat.setIdentity();
			// create axis of voxel
			u = zVector(1, 0, 0);
			v = u ^ zVector(0, 0, 1);
			w = u ^ v;
			v = w ^ u;
			u.normalize(); v.normalize(); w.normalize();

			//scale axes of voxel as per scal factor calculated above
			u *= scale; v *= scale; w *= scale;
			tmat.col(0) << u.x, u.y, u.z, 1;
			tmat.col(1) << v.x, v.y, v.z, 1;
			tmat.col(2) << w.x, w.y, w.z, 1;

			// set voxel center from the array constructed from reading the text file
			tmat.col(3) << pts[i].x, pts[i].y, pts[i].z, 1;

			// set the transformation matrix of the voxel 
			VXLS[i].updateMatrix(tmat);
			VXLS[i].id = i;

		}


	}

	if (k == 'I')
	{
		RM.reset();
		for (int i = 0; i < NUM_VXL; i++)
		{
			VXLS[i].update();
			RM.addMesh(VXLS[i].o_mesh);
		}

	}
	//--------------------------------- testVoxel , testing class methods

	if (k == 'n')
	{

		// load mesh into testVxl
		testVxl.createMeshFromFile("data/cross3d.obj");

		////----------------- construct the transformation matrix

		// set axes of tmat to world axes;
		tmat.setIdentity();

		// construct new axes using cross prodcts;
		// set u axis to diagonal facing top-right quadrant
		u = zVector(1, sin(10 * DEG_TO_RAD), 0);
		// v vector as the cross product of the u axis and the up vector
		v = u ^ zVector(0, 0, 1);
		// w vector as the cross product of the u axis and the v vector
		w = v ^ u;
		//v = u ^ w;

		//construct scaling factor
		u.normalize(); v.normalize(); w.normalize();
		u *= 50; v *= 50; w *= 50;

		// --- set the matrix columns with scaled vectors
		tmat.col(0) << u.x, u.y, u.z, 1;
		tmat.col(1) << v.x, v.y, v.z, 1;
		tmat.col(2) << w.x, w.y, w.z, 1;
		tmat.col(3) << 25, 25, 25, 1;

		// use the transformatrix as constructed, to transform the mesh;
		testVxl.transformMesh(tmat);


		//	RM.addMesh(testVxl.o_mesh);
	}



	if (k == 'm')
	{

		//-------- construct the translation transform matrix
		tmat.setIdentity();
		tmat.col(3) << 0, 0, z, 1;
		z += 0.01;

		//
		testVxl.updateMatrix(tmat);
		testVxl.update();

	}

	if (k == 'w')
	{
		RM.writeOBJ("data/outRM_.obj");
	}



}

void mousePress(int b, int state, int x, int y)
{


}

void mouseMotion(int x, int y)
{

	bool dragging = (glutGetModifiers() == GLUT_ACTIVE_ALT) ? true : false;
	int cur_msx = winW * 0.5;
	int cur_msy = winH * 0.5;
	camPt = zScreenToCamera(cur_msx, cur_msy, 0.2);

}
#endif // _MAIN_

//
//