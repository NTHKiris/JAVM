#ifndef __STACK_FRAME_H__
#define __STACK_FRAME_H__

#include <string>


/*
StackFrame declaration
*/

class StackFrame {

private:
    int opStackMaxSize; // max size of operand stack
    int localVarArrSize; // size of local variable array
    
    struct Element
    {
        float value;
        int type;
    };
    int line;
    int topStack;
    Element *opStack;
    int opStackSize;
    float *localVarArr;
    int localVarSize; 
    bool* localVarInitialized;

   

    

public:
    /*
    Constructor of StackFrame
    */
    StackFrame();
    ~StackFrame();
    void pushToOpStack(float value, int type);
    Element popFromOpStack();
    Element topOpStack();
    void storeToLocalVar(int index, float value, int type);
    Element loadFromLocalVar(int index);
    void processInstruction(const std::string & instruction,const std::string &  argument);
    void getElement(const std::string &inputLine, std::string &instruction, std::string &argument);
    int preInstr(std::string instruction);
    bool isValidNumber(const std::string& s);
    bool isInt(const std::string& s);
    /*
    Run the method written in the testcase
    @param filename name of the file
    */
    void run(std::string filename);
    void printOperandStack();
    void printLocalVariables();
    
};


#endif // !__STACK_FRAME_H__