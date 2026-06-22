#ifndef LOG_SYSTEM_HPP
#define LOG_SYSTEM_HPP

#include <sstream>

#define FILE_WRITE(file, param) file.write(reinterpret_cast<char*>(&param), sizeof(param))

#define LOG_WRITE(file, param, name) \
file.write(reinterpret_cast<char*>(&param), sizeof(param)); \
LogSystem::print_parameter(name, std::to_string(param), sizeof(param), file.tellp())

struct LogSystemConfig
{
    bool print_error = true;
    bool print_log = false;
    bool print_long_log = false;
};

class LogSystem
{
    inline static LogSystemConfig _config;

public:
    static void set_config(LogSystemConfig &&config) noexcept;

    template<typename ...Args>
    static std::string args_to_str(Args&&... args)
    {
        
        std::stringstream ss;
        ((ss << std::forward<Args>(args)), ...);
        return ss.str();
    }

    static void check_offset(std::string name, size_t offset, size_t current_pos);
    static void print_err(std::string error);

    static void print_header(const char magic[4], float version);
    static void print_parameter(std::string name, std::string var, size_t size, size_t current_pos);
    static void print_variable(std::string name, std::string var);
};







#endif
