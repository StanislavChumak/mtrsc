#include "util/mtrsc_message.hpp"

#include <iostream>

namespace mtrs::util
{

void flag_message(FlagMessage flag)
{
    switch (flag)
    {
    case FlagMessage::SKIP_ERROR:
        detail::_config.skip_error = true;
        break;
    case FlagMessage::SKIP_WARNING:
        detail::_config.skip_warning = true;
        break;
    case FlagMessage::PRINT_LOG:
        detail::_config.print_log = true;
        break;
    case FlagMessage::DETAIL_LOG:
        detail::_config.detail_log = true;
        break;
    }
}

void detail::show_message(TypeMessage tmsg, std::string&& message)
{
    switch (tmsg)
    {
    case TypeMessage::ERROR:
        if(detail::_config.detail_log) std::cerr << "[ !! - !! ] ";
        std::cerr << message;
        break;
    case TypeMessage::WARNING:
        if(detail::_config.detail_log) std::cerr << "[ ?? - ?? ] ";
        std::cout << message;
        break;
    case TypeMessage::LOG:
        std::cout << message;
        break;
    default:
        if(detail::_config.detail_log) std::cerr << "[ ?! - ?! ] ";
        std::cerr << message;
        break;
    }
}

}