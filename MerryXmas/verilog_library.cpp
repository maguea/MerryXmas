#include"verilog_library.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <cctype>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <functional>


//Initial read functions
bool isComment(std::string line) {
	//check if a line is a comment, not /**/ cases.
	if (line[0] == '#')
		return true;
	else if (line[0] == '/' && line[1] == '/')
		return true;
	return false;
}

bool findSubstring(std::string line, const std::string& subStr) {
	return line.find(subStr) != std::string::npos;
}

std::string removeInLineComments(std::string line) {
	//somehow add case for /**/
	std::string temp, editedLine{""};
	bool inLine = false;
	for (int i{ 0 }; i < line.length(); i++) {
		editedLine += line[i];
		if (line[i] == ';')
			break;
	}
	return editedLine;
}

void removeWhiteSpace(std::string& line) {
	line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());
}

std::string getName(std::string line) {
	std::string name;
	for (int i{ 0 }; i < line.length(); i++) {
		if (i > 6) {
			if (line[i] == '(' || line[i] == ' ')
				break;
			name += line[i];
		}
	}
	return name;
}//Find module name

//Main gate calculator
void determineAbstraction(Modules* modules, std::string line, int index) {
	std::string firstWord;
	bool moduleUsed = false;
	for (char c : line) {
		if (c == ' ')
			break;
		firstWord += c;
	}
	removeWhiteSpace(firstWord);
	for (int i{ 0 }; i < sizeof(modules); i++) {
		if (firstWord == modules[i].name) {
			moduleUsed = true;
			modules[index].calledModules.push_back(modules[i].name);
			break;
		}
	}
	if (!moduleUsed)
		determineGate(modules, line, firstWord, index);
}

void determineGate(Modules* modules, std::string line, std::string firstWord, int index) {
	auto count = std::count_if(line.begin(), line.end(), [](char c) {return c == ','; });
	if (firstWord == "not")
		modules[index].inverterCount += count;
	else if (firstWord == "and")
		modules[index].andGates.push_back(count);
	else if (firstWord == "nand")
		modules[index].nandGates.push_back(count);
	else if (firstWord == "or")
		modules[index].orGates.push_back(count);
	else if (firstWord == "nor")
		modules[index].norGates.push_back(count);
	else if (firstWord == "xor")
		modules[index].xorGates.push_back(count);
	else if (firstWord == "xnor")
		modules[index].xnorGates.push_back(count);
	else if (firstWord == "assign")
		return;
}

void addUp(Modules* modules, int moduleTotal) {
	for (int i{ 0 }; i < moduleTotal; i++) {
		modules[i].moduleTotal += (modules[i].inverterCount * 2);
		for (int a : modules[i].andGates) {
			std::cout << (a + 1) * 2 << std::endl;
		}
	}
}

//rename to print module specs or sumn
void printAspect(Modules* modules, int index) {
	std::cout << "[ " << index + 1 << " ]\tModule name: \"" << modules[index].name 
		//<< " (Total Transistors: " << modules[index].moduleTotal 
		<< "\"\n\tInverters: " << modules[index].inverterCount << "\n\tAND Gate List: ";
	for (int i = 0; i < modules[index].andGates.size(); i++) 
		std::cout << modules[index].andGates[i] << ' ';


	std::cout << "\n\tNAND Gate List: ";
	for (int i = 0; i < modules[index].nandGates.size(); i++) 
		std::cout << modules[index].nandGates[i] << ' ';
	std::cout << "\n\tOR Gate List: ";
	for (int i = 0; i < modules[index].orGates.size(); i++) 
		std::cout << modules[index].orGates[i] << ' ';
	std::cout << "\n\tNOR Gate List: ";
	for (int i = 0; i < modules[index].norGates.size(); i++) 
		std::cout << modules[index].norGates[i] << ' ';
	std::cout << "\n\tXOR Gate List: ";
	for (int i = 0; i < modules[index].xorGates.size(); i++) 
		std::cout << modules[index].xorGates[i] << ' ';
	std::cout << "\n\tXNOR Gate List: ";
	for (int i = 0; i < modules[index].xnorGates.size(); i++) 
		std::cout << modules[index].xnorGates[i] << ' ';
	std::cout << "\n\tInternally Called Modules: ";
	for (int i = 0; i < modules[index].calledModules.size(); i++) 
		std::cout << modules[index].calledModules[i] << ' ';
	std::cout << "\n\n";
}