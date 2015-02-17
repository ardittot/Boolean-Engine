#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib> 
#include <stdlib.h> 
using namespace std;

void read_file (char (&Cube)[2048][12], int *numVar, int *numCube, char* name) {
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

void write_file (char Cube[2048][12], int numVar, int numCube, char* name) {
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

void print_s (char Cube[2048][12], int numVar, int numCube) {
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

void copycube (char Cube[2048][12], int numVar, int numCube, char (&CubeOut)[2048][12], int *numVarOut, int *numCubeOut) {
	for (int i = 0; i < numCube; i++) {
		for (int j = 0; j < numVar; j++) {
			CubeOut[i][j] = Cube[i][j];
		}
	}
	*numVarOut = numVar;
	*numCubeOut = numCube;
}

bool isUnate (char Cube[2048][12], int numVar, int numCube)
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

bool isDontCare (char Cube[2048][12], int numVar, int numCube)
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

bool isEmpty (char Cube[2048][12], int numVar, int numCube)
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

bool isTerminate (char Cube[2048][12], int numVar, int numCube)
{
	bool U;
	U = isDontCare(Cube,numVar,numCube) || isEmpty(Cube,numVar,numCube);
	U = U || (numCube==1);
	return U;
}

char selSplit (char Cube[2048][12], int numVar, int numCube)
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

void Cofactor (char Cube[2048] [12], int numVar, int numCube, char idx, int pol, char (&Cube_out)[2048][12], int *numVar_out, int *numCube_out) {
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
	*numCube_out = k;
	*numVar_out = numVar;
}

void And (char (&Cube)[2048] [12], int numCube, char idx, int pol) {
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

void Or (char CubeP[2048] [12], int numCubeP, int numVarP, char CubeN[2048] [12], int numCubeN, int numVarN, char (&CubeOut)[2048][12], int *numCubeOut, int *numVarOut) {
	*numCubeOut = numCubeP + numCubeN;
	int numVar = max(numVarP,numVarN);
	*numVarOut = numVar;
	//memset(CubeOut, 0, sizeof(CubeOut));
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

void Complement (char Cube[2048][12], int numVar, int numCube, char (&CubeOut)[2048][12], int *numVarOut, int *numCubeOut)
{
	char Cube_t [2048] [12];
	memset(Cube_t, 0, sizeof(Cube_t));
	int k=0;
	if (isEmpty(Cube,numVar,numCube)) {
		*numCubeOut = 1;
		std::fill(CubeOut[0], CubeOut[0]+numVar, -1);
	} else if (isDontCare(Cube,numVar,numCube)) {
		*numCubeOut = 0;
		memset(CubeOut, 0, sizeof(CubeOut));
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
		for (int i = 0; i < 2048; i++) {
			for (int j = 0; j < 12; j++) {
				CubeOut[i][j] = Cube_t[i][j];
			}
		}
		*numCubeOut = k;	
	}
	*numVarOut = numVar;
}

void neg(char Cube[2048][12], int numVar, int numCube, char (&CubeOut)[2048][12], int *numVarOut, int *numCubeOut) {
	int k;
	char cx;
	char CubeP [2048] [12];
	char CubeN [2048] [12];
	//char Cube_t [2048] [12];
	//int numCube_t, numVar_t;
	memset(CubeP, 0, sizeof(CubeP));
	memset(CubeN, 0, sizeof(CubeN));
	int numVarP=0, numVarN=0;
	int numCubeP=0, numCubeN=0;
	if (!isTerminate(Cube,numVar,numCube)) {
		cx = selSplit(Cube,numVar,numCube);
		
		Cofactor (Cube, numVar, numCube, cx, 1, CubeP, &numVarP, &numCubeP);
		Cofactor (Cube, numVar, numCube, cx, 0, CubeN, &numVarN, &numCubeN);

		neg(CubeN, numVarN, numCubeN, CubeN,&numVarN,&numCubeN);
		neg(CubeP, numVarP, numCubeP, CubeP,&numVarP,&numCubeP);
		
		And(CubeP, numCubeP, cx, 1);
		And(CubeN, numCubeN, cx, 0);
		
		Or(CubeP, numCubeP, numVarP, CubeN, numCubeN, numVarN, CubeOut, &*numCubeOut, &*numVarOut);
	} else {
		Complement(Cube, numVar, numCube, CubeOut, &*numVarOut, &*numCubeOut);
	}
}

void And2 (char Cube1[2048] [12], int numCube1, int numVar1, char Cube2[2048] [12], int numCube2, int numVar2, char (&CubeOut)[2048][12], int *numCubeOut, int *numVarOut) {
	char Cube_t [2048] [12];
	memset(Cube_t, 0, sizeof(Cube_t));
	int numCube_t, numVar_t;
	neg(Cube1, numVar1, numCube1, Cube_t, &numVar_t, &numCube_t);
	neg(Cube2, numVar2, numCube2, CubeOut, &*numVarOut, &*numCubeOut);
	Or(Cube_t, numCube_t, numVar_t, CubeOut, *numCubeOut, *numVarOut, Cube_t, &numCube_t, &numVar_t);
/* 	for (int i = 0; i < *numCubeOut; i++) {
		for (int j = 0; j < *numVarOut; j++){
			Cube_t[i+numCube_t][j] = CubeOut[i][j];
		}
	}
	numCube_t = numCube_t + *numCubeOut; */
	neg(Cube_t, numVar_t, numCube_t, CubeOut, &*numVarOut, &*numCubeOut);
}

int main(int argc, char* argv[]) 
{
	int numVar[17], numCube[17];
	char Funct [17] [2048] [12];
	memset(Funct, 0, sizeof(Funct));
	memset(numVar, 0, sizeof(numVar));
	memset(numCube, 0, sizeof(numCube));
	
	cout << "VLSI CAD: Logic to Layout \n";
	cout << "Programming Assignment 1 - Bonus Credit" << endl;
	
	std::ifstream InFile (argv[1]);
	std::string str;
	
	if (InFile.is_open()) {
		while ( getline (InFile,str) && !str.empty() ) {
			char Op;
			int idx1, idx2, idx3;
			//char Cube_t [2048] [12];
			//memset(Cube_t, 0, sizeof(Cube_t));
			std::string tmp; 
			std::stringstream tmp_oss;
			istringstream buff(str);
			char *cstr = new char[2];
			buff >> Op; 
			switch(Op){
				case 'r'  :
					buff >> idx1;
					tmp_oss << idx1; tmp = tmp_oss.str();
					strcpy(cstr, tmp.c_str());
					strcat (cstr,".pcn ");
					read_file (Funct[idx1], &numVar[idx1], &numCube[idx1], cstr);
					cout << "DEBUG " << Op << " " << idx1 << " " << endl;
					//print_s(Funct[idx1],numVar[idx1],numCube[idx1]);
					break; 
				case '!'  :
					buff >> idx1;
					buff >> idx2;
					neg(Funct[idx2], numVar[idx2], numCube[idx2], Funct[idx1], &numVar[idx1], &numCube[idx1]);
					cout << "DEBUG " << Op << " " << idx1 << endl; //exit(0);
					//print_s(Funct[idx1],numVar[idx1],numCube[idx1]);
					break; 
				case 'c'  :
					buff >> idx1;
					buff >> idx2;
					copycube(Funct[idx2], numVar[idx2], numCube[idx2], Funct[idx1], &numVar[idx1], &numCube[idx1]);
					cout << "DEBUG " << Op << " " << idx1 << " " << idx2 << endl; //exit(0);
					//print_s(Funct[idx1],numVar[idx1],numCube[idx1]);
					break; 
				case '+'  :
					buff >> idx1;
					buff >> idx2;
					buff >> idx3;
					Or(Funct[idx2], numCube[idx2], numVar[idx2], Funct[idx3], numCube[idx3], numVar[idx3], Funct[idx1], &numCube[idx1], &numVar[idx1]);
					cout << "DEBUG " << Op << " " << idx1<< " "  << idx2<< " "  << idx3 << endl; //exit(0);
					//print_s(Funct[idx1],numVar[idx1],numCube[idx1]);
					break; 
				case '&'  :
					buff >> idx1;
					buff >> idx2;
					buff >> idx3;
					
					And2(Funct[idx2], numCube[idx2], numVar[idx2], Funct[idx3], numCube[idx3], numVar[idx3], Funct[idx1], &numCube[idx1], &numVar[idx1]);
/* 					int numCube_t, numVar_t;
					neg(Funct[idx2], numVar[idx2], numCube[idx2], Cube_t, &numVar_t, &numCube_t);
					//print_s(Cube_t,numVar_t,numCube_t);
					neg(Funct[idx3], numVar[idx3], numCube[idx3], Funct[idx1], &numVar[idx1], &numCube[idx1]);
					//print_s(Funct[idx1],numVar[idx1],numCube[idx1]);
					for (int i = 0; i < numCube_t; i++) {
						int A = i + numCube[idx1];
						for (int j = 0; j < numVar_t; j++){
							//Cube_t[i+numCube_t][j] = Funct[idx1][i][j];
							Funct[idx1][A][j] = Cube_t[i][j];
						}
					}
					numCube[idx1] = numCube_t + numCube[idx1];
					URC_part(Funct[idx1], numVar[idx1], numCube[idx1], Funct[idx1], &numVar[idx1], &numCube[idx1]); */
					
					cout << "DEBUG " << Op << " " << idx1 << " " << idx2 << " " << idx3 << endl; //exit(0);
					break; 
				case 'p'  :
					buff >> idx1;
					tmp_oss << idx1; tmp = tmp_oss.str();
					strcpy(cstr, tmp.c_str());
					strcat (cstr,".pcn ");
					write_file (Funct[idx1], numVar[idx1], numCube[idx1], cstr);
					cout << "DEBUG " << Op << " " << idx1 << endl;
					//print_s(Funct[idx1],numVar[idx1],numCube[idx1]);
					break; 
				case 'q'  :
					cout << "DEBUG END " << endl;
				   break; 
				   
				//default :
					
			}
		}
		//Close file
		InFile.close();
	}
	return 0;
}
