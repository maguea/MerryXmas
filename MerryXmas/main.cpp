// "MerryXmas" Created by ARM 11/21/2023
// 
// Matthew 4:4, "Man shall not live by bread alone, but by every word that proceeds from the mouth of God."

#include <iostream>
#include<fstream>
#include<string>
#include"help.h"
using namespace std;

int main() {
    string line; 
    fstream verilog;
    int firstI{ 0 }, index{ 0 };
    ifstream originalVerilog{ "copied_verilog.txt" };
    ofstream verilogNC{ "verilog_no_comments.txt" };

    int moduleTotal{ 0 }; 
    cout << "How many modules did you instantiate: "; 
    cin >> moduleTotal; 
    Modules* modules = new Modules[moduleTotal]; 

    if (originalVerilog && verilogNC) {
        while (getline(originalVerilog, line)) {
            if (!isComment(line)) {
                verilogNC << line << "\n";
                moudleList(line, modules, firstI);
            }
        }
        verilogNC.close();
        originalVerilog.close(); 
    }
    else 
        cout << "error";

    verilog.open("verilog_no_comments.txt");
    if (verilog) {
        while (getline(verilog, line)) {
            moduleInternals(line, modules, index);
        }
    }
    else
        cout << "error";

    cout << "\nList of found modules with their factors\n";
    for (int i = 0; i < moduleTotal; i++) {
        printAspect(modules, i);
    }

    int mainModule{ 0 };
    cout << "Which module do you want to calculate (enter number 1 ~ " << moduleTotal << "): ";
    cin >> mainModule;

    return 0;
}