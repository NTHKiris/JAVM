#include "StackFrame.h"
#include <iostream>
#include <fstream>
#include "errors.h"
#include "constants.h"
using namespace std;

StackFrame::StackFrame() : opStackMaxSize(OPERAND_STACK_MAX_SIZE), localVarArrSize(LOCAL_VARIABLE_ARRAY_SIZE) {}

void StackFrame::processInstruction( const string & instruction,const string &  argument){
   
}

void StackFrame::getElement(const string &inputLine, string &instruction, string &argument){
   int pos = inputLine.find(' ');
    if (pos != string::npos) {
        instruction = inputLine.substr(0, pos);
        argument = inputLine.substr(pos + 1);
    } else {
        instruction = inputLine;
    }
}
void StackFrame::run(string filename) {
    string filepath = "testcase/" + filename;
    ifstream file(filepath);
    if(!file.is_open()){
        cout<<"Cannot open file "<< filename << endl;
         return;
    }
    string inputLine;
      int line = 0;
    while(getline(file,inputLine)){
        line++; 
        string instruction="",
               argument="";
        cout<<"Processing line "<<line<<": "<<inputLine<<endl;
        getElement(inputLine, instruction, argument);
        // cout<<instruction<<" "+argument<<endl;
        processInstruction(instruction,argument);
      

    }
    file.close();

}