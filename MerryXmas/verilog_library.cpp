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
	bool moduleUsed{ false }, closeCondMet{ false };
	int i{ 0 };
	while (isspace(line[i]))
		i++;
	while (line[i] != ' ' && i < line.length() ) {
		firstWord += line[i];
		i++;
	}
	removeWhiteSpace(firstWord);
	for (int i{ 0 }; i < sizeof(modules); i++) {
		if (firstWord == modules[i].name) {
			if (checkForRepeat(modules, firstWord, index) >= 0)
				modules[index].reducedCalledModules[checkForRepeat(modules, firstWord, index)].count++;
			else {
				ReducedModule newModule{ firstWord, 1 };
				modules[index].reducedCalledModules.push_back(newModule);
			}
			moduleUsed = true;
			break;
		}
	}
	if (!moduleUsed)
		determineGate(modules, line, firstWord, index);
}

int checkForRepeat(Modules* modules, std::string name, int index) {
	for (int i{ 0 }; i < modules[index].reducedCalledModules.size(); i++) {
		if (modules[index].reducedCalledModules[i].moduleName == name)
			return i;
	}
	return -1;
}

void determineGate(Modules* modules, std::string line, std::string firstWord, int index) {
	int count = std::count_if(line.begin(), line.end(), [](char c) {return c == ','; });
	
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
		dataFlowGates(modules, line, index);
}

PreviousGate dataFlowGates(Modules* modules, std::string line, int index) {
	int i{ 0 };
	PreviousGate status = DONE;
	while (line[i] != '=')
		i++;
	i++;
	int parCount = std::count_if(line.begin(), line.end(), [](char c) {return c == '('; });
	if (parCount > 0) {
		//std::cout << "notready\n";
	}
	else {
		if (std::count_if(line.begin(), line.end(), [](char c) {return c == '~'; }) > 0)
			modules[index].inverterCount += std::count_if(line.begin(), line.end(), [](char c) {return c == '~'; });
		if (std::count_if(line.begin(), line.end(), [](char c) {return c == '&'; }) > 0) {
			modules[index].andGates.push_back(std::count_if(line.begin(), line.end(), [](char c) {return c == '&'; }));
		}
		if (std::count_if(line.begin(), line.end(), [](char c) {return c == '|'; }) > 0) {
			modules[index].orGates.push_back(std::count_if(line.begin(), line.end(), [](char c) {return c == '|'; }));
		}
		if (std::count_if(line.begin(), line.end(), [](char c) {return c == '^'; }) > 0) {
			modules[index].xorGates.push_back(std::count_if(line.begin(), line.end(), [](char c) {return c == '^'; }));
		}
	}
	return status; 
}


//Final stages
void completeSummary(Modules* modules, int moduleTotal) {
	addUp(modules, moduleTotal);

	std::cout << "Total modules found: " << moduleTotal << "\n\nModule List:\n";
	for (int i{ 0 }; i < moduleTotal; i++) {
		printAspect(modules, i);
	}
	
	//add error function for below
	//HERE
	char response;
	std::string errorModule;
	int requriedTransistors;
	std::cout << "-------------------------------------------------------------------------------------\nIs there anything you need to add/change (y/n): ";
	std::cin >> response;
	if (response == 'y') {
		std::cout << "Which module needs modification: ";
		std::cin >> response;
		std::cout << "How many transistors are expected: ";
		std::cin >> requriedTransistors;
	}
	//TO HERE
	std::cout << "\nThank you for using my program, good luck in your class.\n\t-ARM\n";
}

void addUp(Modules* modules, int moduleTotal) {
	for (int i{ 0 }; i < moduleTotal; i++) {
		modules[i].moduleTotal += (modules[i].inverterCount * 2);
		for (int a : modules[i].andGates) {
			modules[i].moduleTotal += (a + 1) * 2;
		}
		for (int a : modules[i].orGates) {
			modules[i].moduleTotal += (a + 1) * 2;
		}
		for (int a : modules[i].nandGates) {
			modules[i].moduleTotal += a  * 2;
		}
		for (int a : modules[i].norGates) {
			modules[i].moduleTotal += a * 2;
		}
	}
}

//rename to print module specs or sumn
void printAspect(Modules* modules, int index) {
	std::cout << "-------------------------------------------------------------------------------------\n" << "[ " << index + 1 << " ]\tModule name: \"" << modules[index].name
		//<< " (Total Transistors: " << modules[index].moduleTotal 
		<< "\"\n\tInverters: " << modules[index].inverterCount << "\n\tAND Gate List: ";
	for (int i = 0; i < modules[index].andGates.size(); i++) 
		std::cout << modules[index].andGates[i] << ' ';
	std::cout << "\n\tNAND gate list: ";
	for (int i = 0; i < modules[index].nandGates.size(); i++) 
		std::cout << modules[index].nandGates[i] << ' ';
	std::cout << "\n\tOR gate list: ";
	for (int i = 0; i < modules[index].orGates.size(); i++) 
		std::cout << modules[index].orGates[i] << ' ';
	std::cout << "\n\tNOR gate list: ";
	for (int i = 0; i < modules[index].norGates.size(); i++) 
		std::cout << modules[index].norGates[i] << ' ';
	std::cout << "\n\tXOR gate list: ";
	for (int i = 0; i < modules[index].xorGates.size(); i++) 
		std::cout << modules[index].xorGates[i] << ' ';
	std::cout << "\n\tXNOR gate list: ";
	for (int i = 0; i < modules[index].xnorGates.size(); i++) 
		std::cout << modules[index].xnorGates[i] << ' ';
	std::cout << "\n\tInternally called modules:\n";
	for (int i = 0; i < modules[index].reducedCalledModules.size(); i++) { //edit to make putput a better list
		std::cout << "\n\t\t{" << i + 1 << "} " << modules[index].reducedCalledModules[i].moduleName << ", " << modules[index].reducedCalledModules[i].count << " times";
	}
	std::cout << "\n\n\tTotal transistors counted in "<< modules[index].name <<": " << modules[index].moduleTotal << "\n\n";
}