#ifndef _RENDERMESH_
#define _RENDERMESH_

#include "main.h"
//#include "ALICE_ROBOT_DLL.h"
//#include "metaMesh.h"
//using namespace ROBOTICS;


#define _LM_VERTICES 100000
#define _LM_FACECOUNTS_ 100000
#define _LM_FACECONNECTS_ 100000

#define MAX_CUBES 100000

class RenderMesh
{
public:

	GLfloat lm_positions[MAX_CUBES * 72];
	GLfloat lm_normals[MAX_CUBES * 72];
	GLfloat lm_colors[MAX_CUBES * 72];

	GLfloat lm_positions_tris[MAX_CUBES * 72];
	GLfloat lm_normals_tris[MAX_CUBES * 72];
	GLfloat lm_colors_tris[MAX_CUBES * 72];

	GLfloat lm_positions_lines[_LM_VERTICES * 2];
	int n_v_lines;
	//GLuint line_indices[10000];
	
	//int polyCounts[_LM_FACECOUNTS_];
	int n_v, n_f;
	int n_v_tris, n_f_tris;
	int offset = 0;
	int numCubes;
	//metaMesh ground;

	RenderMesh()
	{
		n_v = n_f = 0;
		n_v_tris = n_f_tris = 0;
		n_v_lines = 0;
	}

	//////////////////////////////////////////////// utilities
	void reset()
	{
		n_v = n_f = 0;
		n_v_tris = n_f_tris = 0;
		n_v_lines = 0;
	}
	
	void lightsOn(GLfloat light_pos[4])
	{
		// set position and enable light0
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

		//
		light_pos[0] *= -1;
		glEnable(GL_LIGHT1);
		glLightfv(GL_LIGHT1, GL_POSITION, light_pos);

		light_pos[1] *= -1;
		glEnable(GL_LIGHT2);
		glLightfv(GL_LIGHT2, GL_POSITION, light_pos);


		//glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 2.0);
		//glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 1.0);
		//glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.5);

		// enable material color
		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
	}

	void addtriVertex(  zVector &normal, zVector&P)
	{
		lm_normals_tris[n_v_tris + 0] = normal.x * -1;
		lm_normals_tris[n_v_tris + 1] = normal.y * -1;
		lm_normals_tris[n_v_tris + 2] = normal.z * -1;
		// positions
		lm_positions_tris[n_v_tris + 0] = P.x;
		lm_positions_tris[n_v_tris + 1] = P.y;
		lm_positions_tris[n_v_tris + 2] = P.z;

		n_v_tris += 3;
		if (n_v_tris > MAX_CUBES * 72)n_v_tris = 0;
	}

	void addQuadVertex(zVector&normal, zVector&P)
	{
		lm_normals[n_v + 0] = normal.x * -1;
		lm_normals[n_v + 1] = normal.y * -1;
		lm_normals[n_v + 2] = normal.z * -1;
		// positions
		lm_positions[n_v + 0] = P.x;
		lm_positions[n_v + 1] = P.y;
		lm_positions[n_v + 2] = P.z;

		n_v += 3;
		if (n_v > MAX_CUBES * 72)n_v = 0;
	}

	void addLineVertex(zVector&P)
	{
		lm_positions_lines[n_v_lines + 0] = P.x * 1;
		lm_positions_lines[n_v_lines + 1] = P.y * 1;
		lm_positions_lines[n_v_lines + 2] = P.z * 1;
		// positions
		
		n_v_lines += 3;
		if (n_v_lines > _LM_VERTICES * 2)n_v_lines = 0;
	}

	void getVectorFromArray(int nv, zVector&P, bool triArray = false)
	{

		P = triArray ? zVector(lm_positions_tris[nv + 0], lm_positions_tris[nv + 1], lm_positions_tris[nv + 2])
			         : zVector(lm_positions[nv + 0], lm_positions[nv + 1], lm_positions[nv + 2]);
	}

	void getNormalFromArray(int nv, zVector&normal, bool triArray = false)
	{
		normal = triArray ? zVector(lm_normals_tris[nv + 0], lm_normals_tris[nv + 1], lm_normals_tris[nv + 2])
			: zVector(lm_normals[nv + 0], lm_normals[nv + 1], lm_normals[nv + 2]);
	}

	void addCube( zVector &minBB, zVector &maxBB )
	{
		zVector diag = maxBB - minBB;
		zVector diagMax = zVector(1, 1, 1) - zVector(-1, -1, -1);
		float scale =  (diag.length()) / ( diagMax.length() );
		zVector cen = (maxBB + minBB) * 0.5;


		for (int i = 0; i < 72; i+= 3)
		{
			zVector P =  zVector(vertices[i], vertices[i + 1], vertices[i + 2]) * scale + cen;
			zVector normal = zVector(normals[i], normals[i + 1], normals[i + 2]);
			int nv = n_v;
			addQuadVertex(normal, P);
		}

		n_f += 6;
		numCubes++;
	}

	void addMesh( zObjMesh &o_mesh /*quad & tri only mesh presumed*/)
	{
		zFnMesh fnMesh(o_mesh);
		
		zVectorArray vPosns, vNormals; 
		fnMesh.getVertexNormals(vNormals);
		fnMesh.getVertexPositions(vPosns);
		
		zIntArray pCounts, pConnects;
		fnMesh.getPolygonData(pConnects, pCounts);

		int start_faceVertex = 0;
		for (int i = 0; i < pCounts.size(); i++)
		{
			
			//int f_nv = fnMesh.get; // M.faces[i].n_e;
			//int *face_vertices = M.faces[i].faceVertices();

			int f_nv = pCounts[i];
		

			if( f_nv == 4 )
			{
				for (int n = 0; n < 4; n++)
				{

					zVector P = vPosns[ pConnects[start_faceVertex+n] ];
					zVector normal = vNormals[pConnects[start_faceVertex + n]];
						//M.vertices[face_vertices[n]].vertexNormal(M.positions).normalise();
					addQuadVertex( normal, P);
				}

				n_f++;
			}

			if (f_nv == 3)
			{
				for (int n = 0; n < 3; n++)
				{

					zVector P = vPosns[pConnects[start_faceVertex + n]];
					zVector normal = vNormals[pConnects[start_faceVertex + n]];
					addtriVertex( normal, P);

				}

				n_f_tris++;
			}

			start_faceVertex += f_nv;
		}

		/*for (int i = 0; i < M.n_e; i++)
		{
			if (M.edges[i].lFace == NULL || M.edges[i].rFace == NULL )
			{
				addLineVertex( M.positions[ M.edges[i].vStr->id ] );
				addLineVertex( M.positions[ M.edges[i].vEnd->id ]);
			}
			else 
			{
				float ar;
				vec n1 = M.edges[i].lFace->normal(M.positions, &ar);
				vec n2 = M.edges[i].rFace->normal(M.positions, &ar);
				if (n1.angle(n2) > 60)
				{
					addLineVertex(M.positions[M.edges[i].vStr->id]);
					addLineVertex(M.positions[M.edges[i].vEnd->id]);
				}
			}
		}*/

	}

	//////////////////////////////////////////////// compute

	void updateColorArray(double scale = 10.0, bool flipNormal = false, zVector light = zVector(0, 0, 10))
	{
		//n_v = 0;
		numCubes = n_f / 6;
		//for (int n = 0; n < numCubes; n++)
			for (int i = 0; i < n_v; i += 3)
			{
				int nv = i;// n_v;
				zVector P,normal;
				getVectorFromArray( nv,P);
				getNormalFromArray(nv, normal);
			
				zColor clr = computeColorOfVertex(light, P, normal, nv, scale, flipNormal);
				lm_colors[nv + 0] = clr.r;
				lm_colors[nv + 1] = clr.g;
				lm_colors[nv + 2] = clr.b;// clr.z;

			}

			for (int i = 0; i < n_v_tris; i += 3)
			{
				int nv = i;// n_v;
				zVector P,normal;
				getVectorFromArray(nv, P,true);
				getNormalFromArray(nv, normal,true);

				zColor clr = computeColorOfVertex(light, P, normal, nv, scale, flipNormal);

				lm_colors_tris[nv + 0] = clr.r;
				lm_colors_tris[nv + 1] = clr.g;
				lm_colors_tris[nv + 2] = clr.b;// clr.z;
			}
	}

	zColor computeColorOfVertex(zVector&light, zVector&P, zVector&normal, int &nv, double scale = 5.0250, bool flipNormal = false)
	{
		double ambO = (light - P).angle(normal * (flipNormal ? -1 : 1));
		double dSq = ((light - P)*(light - P));
		double maxD = ((light)*(light));
		dSq = ofMap(dSq, 0, maxD, 0, 1);
		//ambO = ofMap(ambO, 0, 180, -1, 1);

		double ao = 1.0 - (ambO * ofMap(dSq, 0, maxD, 0, 1)  * scale);
		//if (ambO < 90) ao = (ambO * ofMap(dSq, 0, maxD, 0, 1)  * scale);

		zColor clr(ao, ao, ao,1.0);
	
		return clr;
	}

	int getNearestVertexOnGroundPlane(zVector P, int divs , zVector &minBB, zVector &maxBB )
	{
		double delX, delY;
		delX = maxBB.x - minBB.x;
		delY = maxBB.y - minBB.y;
		delX /= divs;
		delY /= divs;
		
		P -= minBB;
		int u = floor(P.x / delX);
		int v = floor(P.y / delY);
		int vertexID = u * divs + v;
		return vertexID;
	}

	
	//////////////////////////////////////////////// output

	void writeOBJ(string outFileName)
	{
		printf(" ----------- writing large-mesh \n ");

		float scaleBack = 1.0;

		ofstream myfile;
		myfile.open(outFileName.c_str(), ios::out);


		if (myfile.fail())
		{
			myfile << " error in opening file  " << outFileName.c_str() << endl;
			return;
		}


		// vertices
		for (int i = 0; i < n_v; i+=3)
		{

			char s[200];
			sprintf(s, "v %1.4f %1.4f %1.4f ", lm_positions[i], lm_positions[i+1] , lm_positions[i+2] );

			myfile << s << endl;

		}

		// faces
		int numCubes = n_f / 6;
		for (int i = 0; i < numCubes  ; i++)
		{


			string str;
			str = "f ";
			for (int j = 0; j < 24; j++)
			{
				char s[200];
				itoa(indices[j] + i * 24 + 1, s, 10);
				str += s;
				str += "//";
				str += s;
				str += " ";
				if ((j+1) % 4 == 0 /*&& j > 0*/ )
				{
					
					myfile << str.c_str() << endl;
					str = "f ";
				}
			}

			

		}


		myfile.close();

		return;
	}

	//////////////////////////////////////////////// display

	void drawFaces( bool color = true )
	{
		{
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);
			if (color)glEnableClientState(GL_COLOR_ARRAY);

			glVertexPointer(3, GL_FLOAT, 0, lm_positions);
			glNormalPointer(GL_FLOAT, 0, lm_normals);
			if (color)glColorPointer(3, GL_FLOAT, 0, lm_colors);
			//glDrawElements(GL_QUADS, n_f * 6, GL_UNSIGNED_BYTE, lm_indices);// index hopping isnt workign currently - 140617
			glDrawArrays(GL_QUADS, 0, n_f * 4);


			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);
			if (color)glDisableClientState(GL_COLOR_ARRAY);
		}

		{
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);
			if(color)glEnableClientState(GL_COLOR_ARRAY);

				glVertexPointer(3, GL_FLOAT, 0, lm_positions_tris);
				glNormalPointer(GL_FLOAT, 0, lm_normals_tris);
				if (color)glColorPointer(3, GL_FLOAT, 0, lm_colors_tris);
				//glDrawElements(GL_QUADS, n_f * 6, GL_UNSIGNED_BYTE, lm_indices);// index hopping isnt workign currently - 140617
				glDrawArrays(GL_TRIANGLES, 0, n_f_tris * 3);


			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);
			if (color)glDisableClientState(GL_COLOR_ARRAY);
		}


	}

	void drawFeatureEdges()
	{
		
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, lm_positions_lines);

			glDrawArrays(GL_LINES, 0, n_v_lines);

		glDisableClientState(GL_VERTEX_ARRAY);
	}

	void draw(bool drawFace = true,  bool drawWire = false , bool drawEdges = true)
	{
		glPushAttrib(GL_CURRENT_BIT);

		if (drawEdges)
		{
			glColor3f(0, 0, 0);
			glLineWidth(3);
				drawFeatureEdges();
			glLineWidth(1);
		}

		if (drawWire)
		{
			glLineWidth(1);
			glColor3f(0, 0, 0);

				wireFrameOn();
					drawFaces(false);
				wireFrameOff();

			glLineWidth(1);
		}

		if (drawFace) drawFaces();

		//glPopMatrix();
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);

	}

};




#endif // !_LARGEMESH_
