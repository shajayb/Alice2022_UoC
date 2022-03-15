#ifndef _INPUT_OUTPUT
#define _INPUT_OUTPUT

#include "ALICE_DLL.h"

#include <headers/zApp/include/zObjects.h>
#include <headers/zApp/include/zFnSets.h>
#include <headers/zApp/include/zViewer.h>
using namespace zSpace;


class IO
{
public:

	void write(string header, string outfilename, zVector* P, int np)
	{
		cout << "  IO.write() :  " << outfilename.c_str() << endl;
		// output file
		ofstream myfile;
		myfile.open(outfilename.c_str());

		if (myfile.fail())
		{
			cout << " error in opening file  " << outfilename.c_str() << endl;
			return;

		}

		myfile << "\n ";
		myfile << header << endl;
		// vertex positions
		zVector pt;
		for (int i = 0; i < np; i++)
		{

			myfile << "\n v " << P[i].x << " " << P[i].y << " " << P[i].z;

		}

		myfile << "\n ";
	}

	zVector read(string infilename, zVectorArray& pts)
	{
		
		zVector dxdydzVector;
		//
		ifstream myfile;
		myfile.open(infilename.c_str());

		if (myfile.fail())
		{
			cout << " error in opening file  " << infilename.c_str() << endl;
			return dxdydzVector;

		}

		while (!myfile.eof())
		{
			string str;
			getline(myfile, str);//v -0.4499999881 -0.4499999881 -1.5467031

			vector<string> perlineData = splitString(str, " ");

			if (perlineData.size() > 0)
			{
				// vertex
				if (perlineData[0] == "v")
				{
					if (perlineData.size() == 4)
					{
						zVector pos;
						pos.x = atof(perlineData[1].c_str());
						pos.y = atof(perlineData[2].c_str());
						pos.z = atof(perlineData[3].c_str());

						pts.push_back(pos);
					}
				}

				if (perlineData[0] == "DIM")
				{
					if (perlineData.size() == 4)
					{
						zVector pos;
						pos.x = atof(perlineData[1].c_str());
						pos.y = atof(perlineData[2].c_str());
						pos.z = atof(perlineData[3].c_str());

						dxdydzVector = pos;
					}
				}
			}
		}

		return dxdydzVector;
	}
};


#endif // !_INPUT_OUTPUT