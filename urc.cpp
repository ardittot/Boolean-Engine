#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib> 
#include <stdlib.h> 
using namespace std;

void read_file (char (&Cube)[400][20], int *numVar, int *numCube, char* name) {
	int N, tmp, nCube;
	std::string str;
	std::ifstream InFile (name);
	
	if (InFile.is_open())
	{
		// Read first line: #Var
		getline (InFile,str);
		*numVar = atoi(str.c_str());
		cout << "NumVar = " << *numVar << endl;
		// Read first line: #Cube
		getline (InFile,str);
		*numCube = atoi(str.c_str());
		cout << "NumCube = " << *numCube << endl;
		cout << endl;
		//Initialize Cube
		nCube = 0;
		//Read each line/cube of text file
		while ( getline (InFile,str) && !str.empty() )
		{
			//Read #non-dont_care vars
			istringstream buff(str);
			buff >> N;
			//Initialize used Cube
			std::fill(Cube[nCube], Cube[nCube]+*numVar, -1);
			//Read each active var
			for (int i=0; i<N; i++)
			{
				buff >> tmp;
				if (tmp < 0) {
					Cube [nCube] [abs(tmp)-1] = -2;
				} else {
					Cube [nCube] [abs(tmp)-1] = -3;
				}
			}
			nCube++;
		}
		//Close file
		InFile.close();
	}
}

void write_file (char Cube[400][20], int numVar, int numCube, char* name) {
	std::ofstream OutFile (name);
	std::string str;
	OutFile << numVar << endl;
	OutFile << numCube << endl;
	for (int i = 0; i < numCube; i++) {
		str = " ";
		int NC = 0;
		for (int j = 0; j < numVar; j++) {
			char ct = Cube[i][j]; int idx;
			std::string tmp1; 
			std::stringstream tmp1_oss;
			if (ct == -3) {
				idx = j+1; tmp1_oss << idx; tmp1 = tmp1_oss.str();
				str = str + tmp1;
				str = str + " ";
				NC++;
			} else if (ct == -2) {
				idx = -(j+1); tmp1_oss << idx; tmp1 = tmp1_oss.str();
				str = str + tmp1;
				str = str + " ";
				NC++;
			}
		}
		std::string tmp1; 
		std::stringstream tmp1_oss;
		tmp1_oss << NC; tmp1 = tmp1_oss.str();
		str = tmp1 + str;
		OutFile << str << endl;
	}
	OutFile.close();
}

void print_s (char Cube[400][20], int numVar, int numCube) {
	for (int i = 0; i < numCube; i++) {
		for (int j = 0; j < numVar; j++) {
			char ct = Cube[i][j];
			if (ct==-1) {
				cout << "- ";
			} else if (ct==-2) {
				cout << "0 ";
			} else if (ct==-3) {
				cout << "1 ";
			}
		}
		cout << endl;
	}
	cout << endl;
}

bool isUnate (char Cube[400][20], int numVar, int numCube)
{
	bool U = true;
	for (int j = 0; j < numVar; j++) {
		char tmp = Cube[0][j];
		for (int i = 1; i < numCube; i++) {
			tmp = tmp & Cube[i][j];
			if (tmp == -4) {
				U = false;
				break;
			}
		}
	}
	return U;
}

bool isDontCare (char Cube[400][20], int numVar, int numCube)
{
	bool U;
	for (int i = 0; i < numCube; i++) {
		U = true;
		for (int j = 0; j < numVar; j++) {
			U = U && (Cube[i][j] == -1);
		}
		if (U) {
			break;
		}
	}
	return U;
}

bool isEmpty (char Cube[400][20], int numVar, int numCube)
{
	bool U = true;
	for (int i = 0; i < numCube; i++) {
		char tmp = Cube[i][0];
		for (int j = 0; j < numVar; j++) {
			char tmp = Cube[i][j];
			if (tmp != -4) {
				U = false;
				break;
			}
		}
		if (!U) {
			break;
		}
	}
	return U;
}

bool isTerminate (char Cube[400][20], int numVar, int numCube)
{
	bool U;
	U = isDontCare(Cube,numVar,numCube) || isEmpty(Cube,numVar,numCube);
	U = U || (numCube==1);
	return U;
}

char selSplit (char Cube[400][20], int numVar, int numCube)
{
	char tmp = 0;
	int nD, nT, nC;
	int nD_t = 0;
	int nM_t = numVar;
	for (int j = 0; j < numVar; j++) {
		nD = numVar;
		nC = 0;
		nT = 0;
		for (int i = 0; i < numCube; i++) {
			if (Cube[i][j] == -1) {
				nD--;
			} else if (Cube[i][j] == -2) {
				nC++;
			} else if (Cube[i][j] == -3) {
				nT++;
			}
		}
		
		if (nT!=0 && nC!=0) {
			if (nD > nD_t) {
				tmp = j;
				nD_t = nD;
				nM_t = abs(nT-nC);
			} else if (nD == nD_t) {
				if (abs(nT-nC) < nM_t) {
					tmp = j;
					nD_t = nD;
					nM_t = abs(nT-nC);
				}
			}
		} else {
			if (nD > nD_t) {
				tmp = j;
				nD_t = nD;
				nM_t = abs(nT-nC);
			}
		}
	}
	return tmp;
}

void Cofactor (char Cube[400] [20], int numVar, int numCube, char idx, int pol, char (&Cube_out)[400][20], int *numCube_t) {
	int k = 0;
	char cmp;
	if (pol==1) {
		cmp = -3;
	} else if (pol==0) {
		cmp = -2;
	}
	for (int i = 0; i < numCube; i++) {
		char tmp = Cube[i][idx];
		if (tmp == -1 || tmp == cmp) {
			for (int j = 0; j < numVar; j++){
				Cube_out[k][j] = Cube[i][j];
			}
			Cube_out[k][int(idx)] = -1;
			k++;
		}
	}
	*numCube_t = k;
}

void And (char (&Cube)[400] [20], int numCube, char idx, int pol) {
	char tmp;
	if (pol==1) {
		tmp = -3;
	} else if (pol==0) {
		tmp = -2;
	}
	for (int i = 0; i < numCube; i++) {
		Cube[i][int(idx)] = tmp;
	}
}

void Or (char CubeP[400] [20], int numCubeP, char CubeN[400] [20], int numCubeN, int numVar, char (&CubeOut)[400][20], int *numCubeOut) {
	*numCubeOut = numCubeP + numCubeN;
	memset(CubeOut, 0, sizeof(CubeOut));
	for (int i = 0; i < numCubeP; i++) {
		for (int j = 0; j < numVar; j++){
			CubeOut[i][j] = CubeP[i][j];
		}
	}
	for (int i = 0; i < numCubeN; i++) {
		for (int j = 0; j < numVar; j++){
			CubeOut[i+numCubeP][j] = CubeN[i][j];
		}
	}
}

void Complement (char (&Cube)[400][20], int numVar, int *numCube)
{
	char Cube_t [400] [20];
	memset(Cube_t, 0, sizeof(Cube_t));
	int k=0;
	if (isEmpty(Cube,numVar,*numCube)) {
		*numCube = 1;
		std::fill(Cube[0], Cube[0]+numVar, -1);
	} else if (isDontCare(Cube,numVar,*numCube)) {
		*numCube = 0;
		memset(Cube, 0, sizeof(Cube));
	} else {
		for (int j = 0; j < numVar; j++) {
			if (Cube[0][j] != -1) {
				for (int z = 0; z < numVar; z++) {
					Cube_t[k][z] = -1;
				}
				Cube_t[k][j] = ~(Cube[0][j] ^ -4);
				k++;
			}
		}
		for (int i = 0; i < 400; i++) {
			for (int j = 0; j < 20; j++) {
				Cube[i][j] = Cube_t[i][j];
			}
		}
		*numCube = k;	
	}
}

void neg(char (&Cube)[400][20], int numVar, int *numCube) {
	int k;
	char cx;
	char CubeP [400] [20];
	char CubeN [400] [20];
	memset(CubeP, 0, sizeof(CubeP));
	memset(CubeN, 0, sizeof(CubeN));
	int numCubeP=0, numCubeN=0;
	if (!isTerminate(Cube,numVar,*numCube)) {
		cx = selSplit(Cube,numVar,*numCube);
		
		Cofactor (Cube, numVar, *numCube, cx, 1, CubeP, &numCubeP);
		Cofactor (Cube, numVar, *numCube, cx, 0, CubeN, &numCubeN);

		neg(CubeN, numVar, &numCubeN);
		neg(CubeP, numVar, &numCubeP);
		
		And(CubeP, numCubeP, cx, 1);
		And(CubeN, numCubeN, cx, 0);
		
		int numCube_t = *numCube;
		Or(CubeP, numCubeP, CubeN, numCubeN, numVar, Cube, &numCube_t);
		*numCube = numCube_t;

	} else {
		int numCube_t = *numCube;
		Complement(Cube, numVar, &numCube_t);
		*numCube = numCube_t;
	}
}

int main(int argc, char* argv[]) 
{
	int numVar, numCube;
	char Cube [400] [20];
	memset(Cube, 0, sizeof(Cube));
	
	cout << "VLSI CAD: Logic to Layout \n";
	cout << "Programming Assignment 1" << endl;

	read_file (Cube, &numVar, &numCube, argv[1]);
	
	//Write cube contents
	cout << "INPUT FUNCTION " << endl;	
	print_s(Cube,numVar,numCube);
	//End write
		
	neg(Cube, numVar, &numCube);
	
	//Write cube contents
	cout << "OUTPUT FUNCTION " << endl;	
	cout << "NumVar = " << numVar << endl;
	cout << "NumCube = " << numCube << endl;
	print_s(Cube,numVar,numCube);
	//End write
	
	write_file (Cube, numVar, numCube, argv[2]);
	
	return 0;
}
