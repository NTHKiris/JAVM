#include "StackFrame.h"
#include <iostream>
#include <fstream>
#include "errors.h"
#include "constants.h"
#include <iomanip>
using namespace std;
void StackFrame::printOperandStack()
{
    cout << "Operand Stack: <";
    for (int i = 0; i < opStackSize; ++i)
    {
        if (i > 0)
            cout << ", ";
        if (opStack[i].type == 1)
            cout << opStack[i].value;
        else
            cout << (opStack[i].value);
        cout << "," << opStack[i].type;
    }
    cout << ">" << endl;
}

int StackFrame::preInstr(string instruction)
{
    char pre = instruction[0];
    if (pre == 'i')
        return 0;
    if (pre == 'f')
        return 1;
    return -1;
}
void StackFrame::printLocalVariables()
{
    cout << "Local Variables: [";
    for (int i = 0; i < localVarSize; i += 2)
    {
        if (i > 0)
            cout << ",";
        int type = (localVarArr[i]);
        cout << type << ",";
        if (type == 1) // float
            cout << localVarArr[i + 1];
        else // int
            cout << (localVarArr[i + 1]);
    }
    cout << "]" << endl;
}

StackFrame::StackFrame() : opStackMaxSize(OPERAND_STACK_MAX_SIZE), localVarArrSize(LOCAL_VARIABLE_ARRAY_SIZE)
{
    line = 0;
    opStackSize = 0;
    localVarSize = localVarArrSize;
    opStack = new Element[opStackMaxSize];
    localVarArr = new float[localVarArrSize];
    for (int i = 0; i < localVarArrSize; ++i)
    {
        localVarArr[i] = 0.0f;
    }
    localVarInitialized = new bool[localVarArrSize / 2];
    for (int i = 0; i < localVarArrSize / 2; ++i)
    {
        localVarInitialized[i] = false;
    }
}
StackFrame::~StackFrame()
{
    delete[] opStack;
    delete[] localVarArr;
    delete[] localVarInitialized;
}
void StackFrame::pushToOpStack(float value, int type)
{
    if (opStackSize >= opStackMaxSize / 2)
    {
        throw StackFull(line);
    }
    opStack[opStackSize].value = static_cast<float>(value);
    opStack[opStackSize].type = type;
    opStackSize++;
}
StackFrame::Element StackFrame::popFromOpStack()
{
    if (opStackSize <= 0)
    {
        throw StackEmpty(line);
    }
    opStackSize--;

    return opStack[opStackSize];
}
StackFrame ::Element StackFrame::topOpStack()
{
    if (opStackSize <= 0)
    {
        throw StackEmpty(line);
    }
    return opStack[opStackSize - 1];
}
void StackFrame::storeToLocalVar(int index, float value, int type)
{
    if (index < 0 || index > localVarSize)
    {
        throw ArrayOutOfRange(line);
    }
    localVarArr[index] = type;
    localVarArr[index + 1] = value;
    localVarInitialized[index / 2] = true;
}
StackFrame::Element StackFrame::loadFromLocalVar(int index)
{
    if (this->localVarSize == 0)
        throw StackEmpty(line);
    if (!localVarInitialized[index / 2])
    {
        throw UndefinedVariable(line);
    }
    if (index < 0)
    {
        throw ArrayOutOfRange(line);
    }

    //  cout<<localVarArr[index * 2 + 1]<<" "<< static_cast<int>(localVarArr[index * 2])<<" ";
    return {localVarArr[index + 1], static_cast<int>(localVarArr[index])};
}
bool StackFrame::isValidNumber(const string &s)
{
    if (s.empty())
        return false;

    int start = 0;
    if (s[0] == '-' || s[0] == '+')
        start = 1;

    bool hasDecimal = false;
    for (size_t i = start; i < s.length(); i++)
    {
        if (s[i] == '.')
        {
            if (hasDecimal)
                return false;
            hasDecimal = true;
        }
        else if (!isdigit(s[i]))
        {
            return false;
        }
    }

    return true;
}
bool StackFrame::isInt(const std::string &s)
{
    if (s.empty())
        return false;
    for (size_t i = 0; i < s.length(); i++)
    {
        if (s[i] == '.')
        {
            return false;
        }
    }
    return true;
}

void StackFrame::processInstruction(const string &instruction, const string &argument)
{
    // print
    if (instruction == "top" || instruction == "val")
    {
        if (instruction == "top")
        {
            Element e = topOpStack();
            if (e.type == 0)
            {
                int x = e.value;
                cout << x << endl;
            }
            else
            {
                float x = static_cast<float>(e.value);
                cout << x << endl;
            }
        }
        if (instruction == "val")
        {
            int index = stoi(argument);
            if (index < 0 || index % 2 != 0)
                throw TypeMisMatch(line);
            if (!localVarInitialized[index / 2])
            {
                throw UndefinedVariable(line);
            }
            if (localVarArr[index] == 0)
            {
                int x = localVarArr[index + 1];
                cout << x << endl;
            }
            else
            {
                float x = (localVarArr[index + 1]);
                cout << x << endl;
            }
        }
    }
    string instruct = instruction.substr(1, instruction.length());
    int type = preInstr(instruction);

    // const load store
    if (instruct == "const" || instruct == "store" || instruct == "load")
    {

        if (instruct == "const")
        {
            if (!isValidNumber(argument))
            {
                throw TypeMisMatch(line);
            }

            if (type == 0)
            {
                if (!isInt(argument))
                    throw TypeMisMatch(line);
                pushToOpStack(stoi(argument), type);
            }
            else if (type == 1)
            {
                // if (isInt(argument))
                //     throw TypeMisMatch(line);
                pushToOpStack(stof(argument), type);
            }
        }

        else if (instruct == "store")
        {
            int index = stoi(argument);
            if (index % 2 == 0)
            {
                int index = stoi(argument);
                Element e = popFromOpStack();
                if (type != e.type)
                    throw TypeMisMatch(line);
                storeToLocalVar(index, e.value, e.type);
            }
            else
            {
                throw TypeMisMatch(line);
            }
        }
        else if (instruct == "load")
        {

            int index = stoi(argument);
            if (index % 2 == 0)
            {
                if (type != localVarArr[index])
                    throw TypeMisMatch(line);
                pushToOpStack(localVarArr[index + 1], localVarArr[index]);
            }
            else
            {
                throw UndefinedVariable(line);
            }
        }
    }
    // change type
    if (instruct == "2f" || instruct == "2i")
    {
        if (instruct == "2f")
        {
            Element e = popFromOpStack();
            if (type != e.type)
                throw TypeMisMatch(line);
            pushToOpStack(e.value, 1);
        }
        if (instruct == "2i")
        {
            Element e = popFromOpStack();
            if (type != e.type)
                throw TypeMisMatch(line);
            const float INT_MAX_F = 2147483647.0f;
            const float INT_MIN_F = -2147483648.0f;

            if (e.value >= INT_MAX_F || e.value <= INT_MIN_F)
            {
                throw UndefinedVariable(line);
            }
            int x = static_cast<int>(e.value);
            pushToOpStack(x, 0);
        }
    }

    // math
    if (instruct == "add" || instruct == "sub" || instruct == "mul" || instruct == "div")
    {
        Element e1 = popFromOpStack();
        // if (type != e1.type)
        //     throw TypeMisMatch(line);
        Element e2 = popFromOpStack();
        // if (type != e2.type)
        //     throw TypeMisMatch(line);
        if (type == 0 && e1.type == 1)
            throw TypeMisMatch(line);
        if (type == 0 && e2.type == 1)
            throw TypeMisMatch(line);
        if (e1.type != e2.type)
        {
            e1.type = e2.type = 1;
        }
        float x1 = e1.value;
        float x2 = e2.value;
        if (instruct == "add")
        {
            pushToOpStack(x1 + x2, type);
        }
        if (instruct == "sub")
        {
            pushToOpStack(x2 - x1, type);
        }
        if (instruct == "mul")
        {
            pushToOpStack(x1 * x2, type);
        }
        if (instruct == "div")
        {
            if (x1 == 0)
            {
                throw DivideByZero(line);
            }
            if (type == 0)
            {
                pushToOpStack(static_cast<int>(x2 / x1), type);
            }
            else
            {
                pushToOpStack(x2 / x1, type);
            }
        }
    }
    if (instruct == "rem" || instruct == "and" || instruct == "or" || instruct == "eq" || instruct == "neq" || instruct == "lt" || instruct == "gt")
    {

        Element e1 = popFromOpStack();
        Element e2 = popFromOpStack();
        float x1 = e1.value;
        float x2 = e2.value;

        if (type == 0 && e1.type == 1)
            throw TypeMisMatch(line);
        if (type == 0 && e2.type == 1)
            throw TypeMisMatch(line);

        if (instruct == "rem")
        {
            if (e1.value == 0)
                throw DivideByZero(line);
            int x = ((int)x2 % (int)x1);
            pushToOpStack(x, type);
        }

        if (instruct == "and")
        {
            int x = (int)x1 & (int)x2;
            pushToOpStack(x, 0);
        }

        if (instruct == "or")
        {
            int x = (int)x1 | (int)x2;
            pushToOpStack(x, 0);
        }

        if (instruct == "eq")
        {
            int x = x1 == x2 ? 1 : 0;
            pushToOpStack(x, 0);
        }

        if (instruct == "neq")
        {
            int x = x1 != x2 ? 1 : 0;
            pushToOpStack(x, 0);
        }

        if (instruct == "lt")
        {
            int x = x2 < x1 ? 1 : 0;
            pushToOpStack(x, 0);
        }
        if (instruct == "gt")
        {
            int x = x2 > x1 ? 1 : 0;
            pushToOpStack(x, 0);
        }
    }
    if (instruct == "bnot" || instruct == "neg")
    {
        Element e = popFromOpStack();
        float x = e.value;
        if (type == 0 && e.type == 1)
            throw TypeMisMatch(line);

        if (instruct == "bnot")
        {
            x == 0 ? pushToOpStack(1, type) : pushToOpStack(0, type);
        }
        if (instruct == "neg")
        {
            pushToOpStack(e.value * -1, type);
        }
    }
}

void StackFrame::getElement(const string &inputLine, string &instruction, string &argument)
{
    size_t pos = inputLine.find(' ');
    if (pos != string::npos)
    {
        instruction = inputLine.substr(0, pos);
        argument = inputLine.substr(pos + 1);
    }
    else
    {
        instruction = inputLine;
    }
}
void StackFrame::run(string filename)
{

    fstream readFile(filename, std::ios::in);

    // string filepath =  filename;
    // // string filepath =  filename;
    // ifstream file(filepath);
    if (!readFile.is_open())
    {
        cout << "Cannot open file " << filename << endl;
        return;
    }
    string inputLine;
    while (getline(readFile, inputLine))
    {
        line++;
        string instruction = "",
               argument = "";
        // cout << "Processing line " << line << ": " << inputLine << endl;
        getElement(inputLine, instruction, argument);
        // cout << instruction << " " + argument << endl;
        processInstruction(instruction, argument);
        // printLocalVariables();
        // printOperandStack();
    }
    readFile.close();
}