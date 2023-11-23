#include"help.h"
#include<algorithm>
#include<fstream>
#include<string>
#include<cstring>
#include <regex>
using namespace std;

//Prints each part of struct
void printAspect(Modules* modules, int index) {
	cout << "[ " << index + 1 << " ]\tModule name: \"" << modules[index].name 
		<< "\"\n\tInverters: " << modules[index].inverterCount << "\n\tAND Gate List: ";
	for (int i = 0; i < modules[index].andGates.size(); i++) {
		cout << modules[index].andGates[i] << ' ';
	}
	cout << "\n\tNAND Gate List: ";
	for (int i = 0; i < modules[index].nandGates.size(); i++) {
		cout << modules[index].nandGates[i] << ' ';
	}
	cout << "\n\tOR Gate List: ";
	for (int i = 0; i < modules[index].orGates.size(); i++) {
		cout << modules[index].orGates[i] << ' ';
	}
	cout << "\n\tNOR Gate List: ";
	for (int i = 0; i < modules[index].norGates.size(); i++) {
		cout << modules[index].norGates[i] << ' ';
	}
	cout << "\n\tXOR Gate List: ";
	for (int i = 0; i < modules[index].xorGates.size(); i++) {
		cout << modules[index].xorGates[i] << ' ';
	}
	cout << "\n\tXNOR Gate List: ";
	for (int i = 0; i < modules[index].xnorGates.size(); i++) {
		cout << modules[index].xnorGates[i] << ' ';
	}
	cout << "\n\tInternally Called Modules: ";
	for (int i = 0; i < modules[index].calledModules.size(); i++) {
		cout << modules[index].calledModules[i] << ' ';
	}
	cout << "\n\n";
}

//find and document all modules
void moudleList(string line, Modules* modules, int& firstI) {
	if (findFirstWord(line, 6) == "module") {
		modules[firstI].name = moduleName(line);
		modules[firstI].inverterCount = 0;
		firstI++;
	}
}

//main
void moduleInternals(string line, Modules* modules, int& index) {
	string name{};
	int location{ 0 };
	if (findFirstWord(line, 6) == "module") {
		name = moduleName(line);
		location = findModule(name, modules);
	}
	else if(findFirstWord(line, 9) == "endmodule") {
		//insert modules[index].count function
		index++;
	}
	else if(findFirstWord(line, 6) == "assign") { 
		int oparCnt = count(line.begin(), line.end(), '(');
		while (oparCnt > 0) {
			replaceSubs(line, modules, index, oparCnt);
			oparCnt--;
		}
		modules[index].inverterCount = notCounter(0, line) + modules[index].inverterCount;
	}
	else {
		string firstWord = findFirstWord(line, findSpace(line) - 1);
		callsModule(line, firstWord, modules, index);
	}
}

void callsModule(string line, string firstWord, Modules* modules, int index) {
	int foundIndex = findModule(firstWord, modules); 
	if (!findModuleCall(line, "wire")) {
		if (foundIndex != -1) {
			modules[index].calledModules.push_back(modules[foundIndex].name);
		}
	}
	else {
		//wire condition here
	}
}

//remove white space
void trim(string& line) {
	line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());
}

void replaceSubs(string& line, Modules* modules, int index, int oparCnt) {
	int newOparCnt = oparCnt;
	string temp, templine{ line };
	trim(templine);
	if ((oparCnt > 0)) {
		for (int i = 0; i < templine.length(); i++) {
			if (templine[i] == '(') {
				newOparCnt--;
				i--;
				if (newOparCnt == 0) {
					while (templine[i] != ')') {
						temp += templine[i];
						i++;
					}
					temp += templine[i];
					status(temp, modules, index);
					break;
				}
			}
		}
	}
}

void status(string gate, Modules* modules, int index) {
	if (count(gate.begin(), gate.end(), '&') > 0) {
		if (nCondition(gate)){
			modules[index].nandGates.push_back(count(gate.begin(), gate.end(), '&'));
			modules[index].inverterCount--;
		}
		else {
			gate.erase(0, 1);
			modules[index].andGates.push_back(count(gate.begin(), gate.end(), '&'));
		}
	}
	else if (count(gate.begin(), gate.end(), '|') > 0) {
		if (nCondition(gate)){
			modules[index].norGates.push_back(count(gate.begin(), gate.end(), '|'));
			modules[index].inverterCount--; 
		}
		else {
			gate.erase(0, 1);
			modules[index].orGates.push_back(count(gate.begin(), gate.end(), '|'));
		}
	}
	else if (count(gate.begin(), gate.end(), '^') > 0) {
		if (nCondition(gate)){
			modules[index].inverterCount--;
			modules[index].xnorGates.push_back(count(gate.begin(), gate.end(), '^'));
		}
		else{
			gate.erase(0, 1);
			modules[index].xorGates.push_back(count(gate.begin(), gate.end(), '^'));
		}
	}
}

//finds first word of a line up to specified length
string findFirstWord(string line, int length) {
	int i{ 0 };
	string firstWord;
	trim(line);
	for (char c : line) {
		if (i < length)
			firstWord += c;
		i++;
	}
	return firstWord;
}

//finds module name
string moduleName(string line) {
	int i{ 0 };
	string name;
	for (char c : line) {
		if (i > 6) {
			if (c == '(') {
				break;
			}
			name += c;
		}
		i++;
	}
	return name;
}


//checks for '//' at beginning of a line
bool isComment(string line) {
	if (findFirstWord(line, 2) == "//")
		return true;
	else
		return false;
}

bool nCondition(string line) {
	if (line[0] == '~')
		return true;
	return false;
}

bool findModuleCall(string prevMod, const string& posMod) {
	return prevMod.find(posMod) != string::npos;
}

//find amount of ~
int notCounter(int i, string line){
	int count = 0; 
	if (i == line.size()) 
		return 0;
	if (line[i] == '~')
		count++;
	count += notCounter(i + 1, line);
	return count;
}

//return index of specified module
int findModule(string match, Modules* modules) {
	for (int i{ 0 }; i < sizeof(modules); i++) {
		if (modules[i].name == match)
			return i;
	}
	return -1;
}

int findSpace(string line) {
	for (int i{ 0 }; i < line.length(); i++) {
		if (line[i] == ' ') {
			return i;
			break;
		}
	}
	return -1;
}