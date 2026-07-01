#ifndef MTRSC_MESSAGE_HPP
#define MTRSC_MESSAGE_HPP

#include <sstream>
#include <iomanip>

#define TYPE_MESSAGE \
X(ERROR) \
X(WARNING) \
X(LOG)

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
    PRINT_LOG,
    DETAIL_LOG
};

namespace detail
{
    struct MessegeConfig
    {
        bool skip_error = false;
        bool skip_warning = false;
        bool print_log = false;
        bool detail_log = false;
    };
    inline MessegeConfig _config;
    void show_message(TypeMessage tmsg, std::string&& message);
}

void flag_message(FlagMessage flag);

#define MTRS_ERROR(...) mtrs::util::mtrsc_message(mtrs::util::TypeMessage::ERROR, __VA_ARGS__)

template<typename... Args>
void mtrsc_message(TypeMessage tmsg, Args&&... args)
{
    std::stringstream ss;
    switch (tmsg)
    {
    case TypeMessage::ERROR:
        if(detail::_config.skip_error) return;
        ss << "ERROR: ";
        break;
    case TypeMessage::WARNING:
        if(detail::_config.skip_warning) return;
        ss << "WARNING: ";
        break;
    case TypeMessage::LOG:
        if(!detail::_config.print_log) return;
        ss << "LOG: ";
        break;
    default:
        ss << "UNKNOWN: ";
        break;
    }
    ((ss << std::forward<Args>(args)), ...) << std::endl;

    detail::show_message(tmsg, ss.str());
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
    if(detail::_config.detail_log)
    {
        mtrsc_message(TypeMessage::LOG, '[', std::right, std::setw(4), (current_pos - size), '-',
            std::left, std::setw(4), current_pos, "] ", std::string(offset, ' '), name, '[', var, ']');
    }
    else
    {
        mtrsc_message(TypeMessage::LOG, std::string(offset, ' '), name, '[', var, ']');
    }
}

template<typename T>
void variable_message(size_t offset, std::string name, T var)
{
    if(detail::_config.detail_log)
    {
        mtrsc_message(TypeMessage::LOG, "[    -    ] ", std::string(offset, ' '), name, '[', var, ']');
    }
}

}

#endif
