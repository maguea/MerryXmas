#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include<string>

///////////////////////////////////////////////////////////////////////////////////////////////////////

struct Modules {
	std::string name;
	int inverterCount{ 0 };
	std::vector <int> andGates;
	std::vector <int> orGates;
	std::vector <int> nandGates;
	std::vector <int> norGates;
	std::vector <int> xorGates;
	std::vector <int> xnorGates;
	std::vector <std::string> calledModules;
	int moduleTotal{ 0 };
};//DONT EDIT, PERECTLY FINE

//Tools for line
bool isComment(std::string line);
bool findSubstring(std::string line, const std::string& subStr);
std::string removeInLineComments(std::string line);
std::string getName(std::string line);
void removeWhiteSpace(std::string& line); 

//Module counter
void determineAbstraction(Modules* modules, std::string line, int index);
void determineGate(Modules* modules, std::string line, std::string firstWord, int index);

//Print module specs
void addUp(Modules* modules, int moduleTotal);
void printAspect(Modules* modules, int index);