#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <stdlib.h>

struct ReducedModule {
	std::string moduleName;
	int count{ 0 };
	bool isBottomLayer{ true };
};

struct Modules {
	std::string name;
	int inverterCount{ 0 };
	std::vector <int> andGates, orGates, nandGates, norGates, xorGates, xnorGates;
	std::vector <ReducedModule> reducedCalledModules;
	int moduleTotal{ 0 };
};

enum PreviousGate {
	AND,
	OR, 
	XOR,
	DONE
};

//Tools for line
bool isComment(std::string line);
bool findSubstring(std::string line, const std::string& subStr);
std::string removeInLineComments(std::string line);
std::string getName(std::string line);
void removeWhiteSpace(std::string& line); 
int checkForRepeat(Modules* modules, std::string name, int index);

//Module counter
void determineAbstraction(Modules* modules, std::string line, int index);
void determineGate(Modules* modules, std::string line, std::string firstWord, int index);
PreviousGate dataFlowGates(Modules* modules, std::string line, int index, PreviousGate lastStatus);

//Print module specs
void completeSummary(Modules* modules, int moduleTotal, std::vector<std::string> nameLst);
void addUp(Modules* modules, int moduleTotal);
void additionalModuleTransistors(Modules* modules, int moduleTotal, std::vector<std::string> nameLst);
void printAspect(Modules* modules, int index);
void errorChecker(Modules* modules);