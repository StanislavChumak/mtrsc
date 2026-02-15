#include "util/string_optimizer.h"

std::string stiring_optimizer(std::string str)
{
    std::string result;
    result.reserve(str.length());
    
    std::string operators = "+-*/=<>!&|?:(){};.,";
    
    bool spacePending = false;
    bool lastCharWasOperator = false;
    bool isDefine = false;
    
    for(char c : str)
    {
        if(std::isspace(c))
        {
            if(c == '\n' && isDefine)
            {
                result += '\n';
                isDefine = false;
                lastCharWasOperator = true;
                continue;
            }
            spacePending = true;
            continue;
        }

        if(c == '#') isDefine = true;
        
        bool isOperator = operators.find(c) != std::string::npos;
        
        if(isOperator)
        {
            result += c;
            lastCharWasOperator = true;
            spacePending = false;
        }
        else
        {
            if(spacePending && !lastCharWasOperator && !result.empty()) 
                result += ' ';
            
            result += c;
            lastCharWasOperator = false;
            spacePending = false;
        }
    }
    return result;
}