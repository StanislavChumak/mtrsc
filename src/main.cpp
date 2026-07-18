#include "util/sourse_to_desp_files.hpp"
#include "util/from_json.hpp"
#include "util/mtrsc_message.hpp"

#include <filesystem>

int main(int argc, char **argv)
{
    if(argc < 2)
    {
        mtrs::util::mtrsc_error("No arguments");
        return 0;
    }

    if(argc > 3)
    {
        std::string arg;

        for(int i = 1; i < (argc - 1); i++)
        {
            arg = argv[i];
            
            if(arg == "-log")
            {
                mtrs::util::flag_message(mtrs::util::FlagMessage::PRINT_INFO);
            }
            else if(arg == "-det-log")
            {
                mtrs::util::flag_message(mtrs::util::FlagMessage::PRINT_INFO);
                mtrs::util::flag_message(mtrs::util::FlagMessage::DETAIL_INFO);
            }
        }
    }

    std::string sourse_path = argv[argc - 2];
    std::string desp_path = argv[argc - 1];

    mtrs::util::copy_and_transform_directory(sourse_path, desp_path);

    return 0;
}

