// "MerryXmas" main.cpp
// Created by ARM 11/21/2023
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

    //Check if program was copied correctly
    if (!originalVerilog) {
        std::cout << "Program Download Error.";
        return 0;
    }

    //Create dupe without huge comments
    while (std::getline(originalVerilog, line)){
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

    //rewrite vector into array for easier transfer
    Modules* modules = new Modules[moduleTotal];

    for (int i{ 0 }; i < moduleTotal; i++)
        modules[i].name = nameLst[i];

    //Use smaller file without comments to avoid dupe, run scanner
    int index{ 0 };
    std::fstream copyFile;
    copyFile.open("verilog_no_comments.txt");
    while (std::getline(copyFile, line)) {
        //removeWhiteSpace(line);
        if (findSubstring(line, "endmodule"))
            index++; //send this index along to a count function OR make a object for each
        determineAbstraction(modules, line, index);
    }
    
    
    //Add entire thing to print function feature
    addUp(modules, moduleTotal);

    std::cout << "Totoal modules found: " << moduleTotal << "\n\nModule List:\n";
    for (int i{ 0 }; i < moduleTotal; i++) {
        printAspect(modules, i);
    }
    

    //Console imput for missed modules
    char response;
    std::string errorModule;
    int requriedTransistors;
    std::cout << "Is there anything you need to add/change (y/n): ";
    std::cin >> response;
    if (response == 'y') {
        std::cout << "Which module needs modification: ";
        std::cin >> response;
        std::cout << "How many transistors are expected: ";
        std::cin >> requriedTransistors;
    }
    return 0;
}
