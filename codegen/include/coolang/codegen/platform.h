#ifndef COOLANG_CODEGEN_PLATFORM_H
#define COOLANG_CODEGEN_PLATFORM_H

#include <filesystem>

namespace coolang {
namespace platform {

std::string GetLinkerCommand(const std::filesystem::path& obj_path,
                             const std::filesystem::path& exe_path);
std::string GetObjectFileExtension();
std::string GetExeFileExtension();

}  // namespace platform
}  // namespace coolang

#endif  // COOLANG_CODEGEN_PLATFORM_H
