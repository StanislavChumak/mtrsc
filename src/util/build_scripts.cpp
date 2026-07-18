#include "util/build_scripts.hpp"

#include "util/mtrsc_message.hpp"

#include <fstream>

namespace mtrs::util
{

void write_generated_cmake(const std::vector<ScriptEntry>& scripts)
{
    std::ofstream out("scripts_build/generated_scripts.cmake");

    out << "# AUTO-GENERATED - no manual editing required\n";
    for (const auto& s : scripts)
    {
        out << "add_script_module(" << s.target_name
            << " \"" << s.source_path.generic_string() << "\""
            << " \"" << s.dest_dir.generic_string() << "\")\n";
    }
}

bool build_all_scripts()
{
    fs::path build_output = "scripts_build/_cmake_build";

    std::ostringstream configure_cmd;
    configure_cmd << "cmake -S \"scripts_build\""
                  << " -B \"" << build_output.string() << "\""
#if !defined(_WIN32)
                  << " -DCMAKE_BUILD_TYPE=Release"
#endif
                  ;

    if (std::system(configure_cmd.str().c_str()) != 0)
    {
        util::mtrsc_error("CMake configure failed for scripts");
        return false;
    }

    std::ostringstream build_cmd;
    build_cmd << "cmake --build \"" << build_output.string() << "\""
              << " --config Release"
              << " --parallel";

    if (std::system(build_cmd.str().c_str()) != 0)
    {
        util::mtrsc_error("CMake build failed for scripts");
        return false;
    }
    return true;
}

}