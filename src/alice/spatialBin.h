#ifndef SPATIAL_BIN
#define SPATIAL_BIN

#include "ALICE_DLL.h"

#include <headers/zApp/include/zObjects.h>
#include <headers/zApp/include/zFnSets.h>
#include <headers/zApp/include/zViewer.h>
using namespace zSpace;

#define bucketSize 30
struct bucket
{
	int n;
	int ids[bucketSize];
	void addId(int &i)
	{

		if (n >= bucketSize)return;
		ids[n] = i;
		n++;
	}
	void clearBucket()
	{
		n = 0;
	}
	bool containsParticles()
	{
		return (n > 0);
	}
};

void drawSphere(zVector&a, zVector rotate, zVector scale = zVector(1, 1, 1), float r = 1.0, float alpha = 1.0)
{
	GLfloat light_pos[] = { 20, 20, 100, 1.0 };

	glPushMatrix();
	glTranslatef(a.x, a.y, a.z);

	glPushMatrix();

	//
	glRotatef(rotate.x, 1, 0, 0);
	glRotatef(rotate.y, 0, 1, 0);
	glRotatef(rotate.z, 0, 0, 1);
	glScalef(scale.x, scale.y, scale.z);



	glPushAttrib(GL_CURRENT_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	static const GLdouble equation[] = { 0, 0, 1.0, 0.0 };
	glClipPlane(GL_CLIP_PLANE0, equation);
	glEnable(GL_CLIP_PLANE0);

	resetProjection();
	enableLight(light_pos);
	restore3d();

	glColor4f(1.0, 0.0, 0.5, alpha);

	glutSolidSphere(r, 32, 32);

	glPopMatrix();
	glPopMatrix();

	glDisable(GL_CLIP_PLANE0);
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);

}

class spaceGrid
{

#define MAX_SPATIAL_BIN_POINTS 100000
public:

	zVector *positions;

	int np;

	#define RES 40 	
	bucket buckets[RES*RES*RES];
	double dx, dy, dz;
	zVector min, max;

	zUtilsCore core;
	zUtilsDisplay utils;

	spaceGrid()
	{
		np = 0;
		positions = new zVector[MAX_SPATIAL_BIN_POINTS];
	}

	spaceGrid( zVector *pos, int _np)
	{
		positions = pos;
		np = _np;
	}
	////////////////////////////////////////////////////////////////////////// utility methods 
	
	void addPosition(zVector & pt)
	{
		positions[np] = pt;
		np++;
	}
	int getNeighBors(int *&nBorIds, zVector inPt, double searchRadius)
	{
		int sRad_x, sRad_y, sRad_z;
		sRad_x = (searchRadius / dx) + 2;
		sRad_y = (searchRadius / dy) + 2;
		//sRad_z = (searchRadius / dz) + 2;
		sRad_z = (dz == 1e-04) ? 0 : (searchRadius / dz) + 1;

		////////////////////////////////////////////////////////////////////////// calculate voxel nBor search-index based on link-radius

		int u, v, w, index, nborId; // to store 1D and 3D voxel indices per particles, and neighbor id.
		zVector pt, p_n, vel, v_n, diff; // to temp.store positions/velocities of particle and its neighbor.
		double dSq; // to store square of the distance

		nBorIds = new int[bucketSize * sRad_x * 2 + bucketSize * sRad_y * 2 + bucketSize * sRad_z * 2];
		int n_cnt = 0;

		{
			pt = inPt;
			threeDIndexFromPosition(pt, u, v, w);
			if (u == -1 || v == -1 || w == -1)return 0;
			//search in nborhood

			for (int i = u - sRad_x; i <= u + sRad_x; i++)
				for (int j = v - sRad_y; j <= v + sRad_y; j++)
					for (int k = w - sRad_z; k <= w + sRad_z; k++)
					{
						index = threeDIndexToIndex(i, j, k);

						if (index < RES*RES*RES && index >= 0)
							if (buckets[index].containsParticles())
								for (int nb = 0; nb < buckets[index].n; nb++)
								{

									nborId = buckets[index].ids[nb];
									if (nborId >= np)continue;

									p_n = positions[nborId];
									diff = p_n - pt;
									if (diff * diff < searchRadius * searchRadius)
									{
										nBorIds[n_cnt] = nborId;
										n_cnt++;
									}
								}
					}
		}
		return (n_cnt);
	}
	int getPointsFromLowestNBuckets(int *&nPoints, int nBoxes)
	{

		int numPoints = 0;
		int nbx = 0;
		while (nbx < nBoxes && nbx < RES*RES*RES)
		{
			if (buckets[nbx].containsParticles())numPoints += buckets[nbx].n;
			nbx++;
		}


		int cnt = 0;
		nPoints = new int[numPoints];

		for (int i = 0; i < nBoxes; i++)
			for (int j = 0; j < buckets[i].n; j++)
			{
				nPoints[cnt] = buckets[i].ids[j];
				cnt++;
			}
		return cnt;
	}
	void indexToThreeDIndex(int &index, int &u, int &v, int &w)
	{
		u = index % RES;
		v = (index / RES) % RES;
		w = index / (RES * RES);
	}
	int threeDIndexToIndex(int &i, int &j, int &k)
	{
		int index = (i + j*RES + k*RES*RES);
		return (index > 0 && index < RES*RES*RES) ? index : -1;
	}
	void threeDIndexFromPosition(zVector p, int &u, int &v, int &w)
	{
		p.x -= min.x;
		p.y -= min.y;
		p.z -= min.z;
		u = floor(p.x / dx); v = floor(p.y / dy); w = floor(p.z / dz);

		if (u >= RES || u < 0)u = -1;
		if (v >= RES || v < 0)v = -1;
		if (w >= RES || w < 0)w = -1;
	}
	zVector centerOfBucket(int &u, int &v, int &w)
	{
		return (min + (zVector(u*dx, v*dy, w*dz) + zVector(dx, dy, dz)) * 0.5);
	}
	void clearBuckets()
	{
		int index;
		for (int i = 0; i < RES; i++)
			for (int j = 0; j < RES; j++)
				for (int k = 0; k < RES; k++)buckets[i + j*RES + k*RES*RES].clearBucket();
	}
	void computeBucketDimensions()
	{
		if ((max.z - min.z) < 0) max.z = min.z = 0;

		zVector diff = max - min;
		dx = diff.x / (float)RES;
		dy = diff.y / (float)RES;
		dz = diff.z / (float)RES;

		if (dz < 1e-04)dz = 1e-04;

		/*oneOverDx = 1.0 / dx;
		oneOverDy = 1.0 / dy;
		oneOverDz = 1.0 / dz;*/
	}
	void getBoundingBox(zVector &min, zVector &max)
	{
		double x = pow(10, 10);
		min = zVector(x, x, x);
		max = min * -1;

		for (int i = 0; i < np; i++)
		{
			min.x = MIN(positions[i].x, min.x);
			min.y = MIN(positions[i].y, min.y);
			min.z = MIN(positions[i].z, min.z);

			max.x = MAX(positions[i].x, max.x);
			max.y = MAX(positions[i].y, max.y);
			max.z = MAX(positions[i].z, max.z);
		}
	}
	int getNumVoxelsContainingParticles()
	{
		int numVxl = 0;

		for (int i = 0; i < RES; i++)
			for (int j = 0; j < RES; j++)
				for (int k = 0; k < RES; k++)
					if (buckets[threeDIndexToIndex(i, j, k)].containsParticles())numVxl++;

		return numVxl;						
	}

	void getVoxelBB( int i, int j, int k , zVector &minBB,zVector &maxBB)
	{
		minBB = min + zVector(i * dx, j * dy, k * dz);
		maxBB = min + zVector(i * dx, j * dy, k * dz) + zVector(dx, dy, dz);
	}
	//////////////////////////////////////////////////////////////////////////

	void PartitionParticlesToBuckets()
	{
		clearBuckets();

		getBoundingBox(min, max);


		computeBucketDimensions();

		zVector pt; int index;
		int u, v, w;
		for (int i = 0; i < np; i++)
		{
			pt = positions[i];
			threeDIndexFromPosition(pt, u, v, w);
			index = threeDIndexToIndex(u, v, w);

			if (index != -1)
				buckets[index].addId(i);

		}
	}

	//////////////////////////////////////////////////////////////////////////
	void drawBuckets()
	{
		glLineWidth(1);
		int index;
		for (int i = 0; i < RES; i++)
			for (int j = 0; j < RES; j++)
				for (int k = 0; k < RES; k++)
					if (!buckets[threeDIndexToIndex(i, j, k)].containsParticles() )
						utils.drawCube( min + zVector(i * dx, j * dy, k * dz), min + zVector(i * dx, j * dy, k * dz) + zVector(dx, dy, dz), zColor(0.35, 0.35, 0.35, 1), 1);
	}
	void drawParticlesInBuckets()
	{
		glPointSize(5);
		int u, v, w;
		for (int i = 0; i < RES*RES*RES; i++)
		{

			if (buckets[i].n > 0)
			{
				indexToThreeDIndex(i, u, v, w);
				for (int nb = 0; nb < buckets[i].n; nb++)
				{
					//glColor3f(ofMap(u, 0, RES, 0, 1), ofMap(v, 0, RES, 0, 1), ofMap(w, 0, RES, 0, 1));
					utils.drawPoint(positions[buckets[i].ids[nb]], zColor(ofMap(u, 0, RES, 0, 1), ofMap(v, 0, RES, 0, 1), ofMap(w, 0, RES, 0, 1),1));
				}
			}
		}
		glPointSize(1);
	}

};

//class scaledSphere
//{
//public:
//	zVector cen;
//	zVector scale;
//	zVector rotate;
//	float r;
//	Matrix4 transMatrix;
//	Matrix4 transforNormal;
//	Matrix4 transInverse;
//
//
//	scaledSphere(){};
//	scaledSphere(zVector &_cen, zVector &_rotate, zVector &_scale, float _r)
//	{
//
//		r = _r;
//		cen = _cen;
//		scale = _scale;
//		rotate = _rotate;
//
//		transMatrix.identity();
//		transMatrix.scale(_scale.x, _scale.y, _scale.z);
//		transMatrix.rotateZ(rotate.z);
//		transMatrix.rotateY(rotate.y);
//		transMatrix.rotateX(rotate.x);
//		transMatrix.translate(cen);
//
//
//		transforNormal = transMatrix;
//		transforNormal.invert();
//		transforNormal.transpose();
//
//		transInverse = transMatrix;
//		transInverse.invert();
//
//	}
//
//	zVector transformNormal(zVector &n)
//	{
//		return (transforNormal * n);
//	}
//
//	zVector transfromPoint(zVector &pt)
//	{
//		return (transMatrix * pt);
//	}
//
//	zVector invertPt(zVector &pt)
//	{
//
//		return (transInverse * pt);
//	}
//
//	zVector invertNormal(zVector &n)
//	{
//		return (transInverse * n);
//	}
//
//
//	void draw()
//	{
//		glPointSize(5);
//			drawSphere(cen, rotate, scale, r, 0.7);
//
//			// untransformed sphere
//			//drawSphere(zVector(0, 0, 0), zVector(0, 0, 0), zVector(1, 1, 1), r, 0.1);
//
//		glPointSize(1);
//	}
//
//};
#endif // !SPATIAL_BIN
