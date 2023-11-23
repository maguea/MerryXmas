#pragma once
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////////

struct Modules {
	string name;
	int inverterCount;
	vector <int> andGates;
	vector <int> orGates;
	vector <int> nandGates;
	vector <int> norGates;
	vector <int> xorGates;
	vector <int> xnorGates;
	vector <string> calledModules;
};

void printAspect(Modules* modules, int index);

void moudleList(string line, Modules* modules, int& firstI); 

void moduleInternals(string line, Modules* modules, int& index);

void trim(string& line); 

void replaceSubs(string& line, Modules* modules, int index, int oparCnt);

void status(string gate, Modules* modules, int index);

string findFirstWord(string line, int length);

string moduleName(string line);

bool isComment(string line);

bool nCondition(string line);

int notCounter(int i, string line);

int findModule(string match, Modules* modules);