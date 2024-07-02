#ifndef __STACK_FRAME_H__
#define __STACK_FRAME_H__

#include <string>

/*
StackFrame declaration
*/
class StackFrame {
    int opStackMaxSize; // max size of operand stack
    int localVarArrSize; // size of local variable array
public:
    /*
    Constructor of StackFrame
    */
    StackFrame();
    void processInstruction(const std::string & instruction,const std::string &  argument);
    void getElement(const std::string &inputLine, std::string &instruction, std::string &argument);
    /*
    Run the method written in the testcase
    @param filename name of the file
    */
    void run(std::string filename);
};

#endif // !__STACK_FRAME_H__