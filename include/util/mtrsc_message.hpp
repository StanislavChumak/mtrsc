#ifndef MTRSC_MESSAGE_HPP
#define MTRSC_MESSAGE_HPP

#include <sstream>
#include <iomanip>

#define TYPE_MESSAGE \
X(ERROR) \
X(WARNING) \
X(INFO)

#define LOG_WRITE(file, offset, param, name) \
file.write(reinterpret_cast<char*>(&param), sizeof(param)); \
mtrs::util::parameter_message(offset, name, param, sizeof(param), file.tellp())

namespace mtrs::util
{

enum class TypeMessage
{
#define X(TMSG) TMSG,
    TYPE_MESSAGE
    UNKNOWN
#undef X
};

enum class FlagMessage
{
    SKIP_ERROR,
    SKIP_WARNING,
    PRINT_INFO,
    DETAIL_INFO
};

namespace detail
{
    struct MessegeConfig
    {
        bool skip_error = false;
        bool skip_warning = false;
        bool print_info = false;
        bool detail_info = false;
    };
    inline MessegeConfig _config;
    void show_message(TypeMessage tmsg, std::string&& message);
}

void flag_message(FlagMessage flag);

template<typename... Args>
void mtrsc_message(TypeMessage tmsg, Args&&... args)
{
    std::stringstream ss;
    ((ss << std::forward<Args>(args)), ...) << std::endl;

    detail::show_message(tmsg, ss.str());
}

template<typename... Args>
void mtrsc_error(Args&&... args)
{
    mtrsc_message(TypeMessage::ERROR, args...);
}

template<typename... Args>
void mtrsc_warning(Args&&... args)
{
    mtrsc_message(TypeMessage::WARNING, args...);
}

template<typename... Args>
void mtrsc_info(Args&&... args)
{
    mtrsc_message(TypeMessage::INFO, args...);
}

template<typename T>
bool verification_message(std::string name, const T &check, const T &due)
{
    bool check_result = check == due;
    if(detail::_config.skip_error || check_result) return check_result;

    mtrsc_message(TypeMessage::ERROR, "The value of \"", name, "\"actually corresponds to \"",
        check, "\", which does not match the proper \"", due);
    
    return false;
}

template<typename T>
void parameter_message(size_t offset, std::string name, T var, size_t size, size_t current_pos)
{
    if(detail::_config.detail_info)
    {
        mtrsc_message(TypeMessage::INFO, '[', std::right, std::setw(4), (current_pos - size), '-',
            std::left, std::setw(4), current_pos, "] ", std::string(offset, ' '), name, '[', var, ']');
    }
    else
    {
        mtrsc_message(TypeMessage::INFO, std::string(offset, ' '), name, '[', var, ']');
    }
}

template<typename T>
void variable_message(size_t offset, std::string name, T var)
{
    if(detail::_config.detail_info)
    {
        mtrsc_message(TypeMessage::INFO, "[    -    ] ", std::string(offset, ' '), name, '[', var, ']');
    }
}

}

#endif
