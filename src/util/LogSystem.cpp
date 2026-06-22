#include "util/LogSystem.hpp"

#include <iostream>
#include <iomanip>


void LogSystem::set_config(LogSystemConfig &&config) noexcept
{
    _config = std::move(config);
}

void LogSystem::check_offset(std::string name, size_t offset, size_t current_pos)
{
    if(!_config.print_error || offset == current_pos) return;

    if(_config.print_long_log)
    {
        std::cerr << "[ !! - !! ] ";   
    }

    std::cerr << "The \"" << name << "\" indent is actually " << current_pos;
    std::cerr << ", which does not correspond to the stated " << offset << std::endl;
}

void LogSystem::print_err(std::string error)
{
    std::cerr << error << std::endl;
}

void LogSystem::print_header(const char magic[4], float version)
{
    if(_config.print_log)
    {

    }
    else if(_config.print_long_log)
    {
        std::cout << "[   0-8   ] header[" << magic << " " << std::setprecision(2) << version << ']' << std::endl;
    }
}

void LogSystem::print_parameter(std::string name, std::string var, size_t size, size_t current_pos)
{
    if(_config.print_log)
    {

    }
    else if(_config.print_long_log)
    {
        std::cout << '[' << std::right << std::setw(4) << (current_pos - size) << '-';
        std::cout << std::left << std::setw(4) << current_pos << "] ";
        std::cout << name << '[' << var << ']' << std::endl;
    }
}

void LogSystem::print_variable(std::string name, std::string var)
{
    if(_config.print_log)
    {

    }
    else if(_config.print_long_log)
    {
        std::cout << "[    -    ] ";
        std::cout << name << '[' << var << ']' << std::endl;
    }
}