#ifndef BUILD_SCRIPTS_HPP
#define BUILD_SCRIPTS_HPP

#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

namespace mtrs::util
{
    
struct ScriptEntry
{
    std::string target_name;
    fs::path source_path;
    fs::path dest_dir;
};

void write_generated_cmake(const std::vector<ScriptEntry>& scripts);
bool build_all_scripts();

}

#endif
