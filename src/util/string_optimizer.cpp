#include "util/string_optimizer.hpp"

namespace mtrs::util
{

std::string stiring_optimizer(std::string str)
{
    std::string result;
    result.reserve(str.length());
    
    std::string operators = "+-*/=<>!&|?:(){};.,";
    
    bool space_pending = false;
    bool last_char_was_oper = false;
    bool is_define = false;
    
    for(char c : str)
    {
        if(std::isspace(c))
        {
            if(c == '\n' && is_define)
            {
                result += '\n';
                is_define = false;
                last_char_was_oper = true;
                continue;
            }
            space_pending = true;
            continue;
        }

        if(c == '#') is_define = true;
        
        bool is_oper = operators.find(c) != std::string::npos;
        
        if(is_oper)
        {
            result += c;
            last_char_was_oper = true;
            space_pending = false;
        }
        else
        {
            if(space_pending && !last_char_was_oper && !result.empty()) 
                result += ' ';
            
            result += c;
            last_char_was_oper = false;
            space_pending = false;
        }
    }
    return result;
}

}