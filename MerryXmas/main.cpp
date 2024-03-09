// "MerryXmas" main.cpp
// Created by ARM 10/21/2023
// Last Edited 03/08/2024
// 
// Matthew 4:4, "Man shall not live by bread alone, but by every word that proceeds from the mouth of God."

#include"verilog_library.h"

int main() {
    std::string line;
    std::vector<std::string> nameLst;
    int moduleTotal{ 0 };
    std::ifstream originalVerilog{ "copied_verilog.txt" };
    std::ofstream verilogNC{ "verilog_no_comments.txt" };
    
    if (!originalVerilog) { //Fail case for if program was downloaded incorrectly
        std::cout << "Program Download Error.";
        return -1;
    }

    while (std::getline(originalVerilog, line)){ //Create dupe without huge comments
        if (!isComment(line)) {
            verilogNC << removeInLineComments(line) << "\n"; //fix inline comments to work with /**/
            if (findSubstring(line, "module ") && !findSubstring(line, "endmodule")) {
                moduleTotal++;
                nameLst.push_back(getName(line));
            }
        }
    }
    verilogNC.close();
    originalVerilog.close(); 

    Modules* modules = new Modules[moduleTotal]; //rewrite vector into array for easier transfer

    for (int i{ 0 }; i < moduleTotal; i++)
        modules[i].name = nameLst[i];

    int index{ 0 }; //Use smaller file without comments to avoid dupe, run scanner
    std::fstream copyFile;
    copyFile.open("verilog_no_comments.txt");
    while (std::getline(copyFile, line)) {
        if (findSubstring(line, "endmodule"))
            index++; //send this index along to a count function OR make a object for each
        determineAbstraction(modules, line, index);
    }
    
    completeSummary(modules, moduleTotal); //Prints and asks for potential edits

    return 0;
}
