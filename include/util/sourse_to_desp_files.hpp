#ifndef SOURSE_TO_DESP_FILES_HPP
#define SOURSE_TO_DESP_FILES_HPP

#include <filesystem>

namespace mtrs::util
{

void copy_and_transform_directory(const std::filesystem::path &source_dir, const std::filesystem::path &desp_dir);

}


#endif
