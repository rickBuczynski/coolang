#ifndef COOLANG_CODEGEN_PLATFORM_H
#define COOLANG_CODEGEN_PLATFORM_H

#include <filesystem>
#include <optional>

namespace coolang {
namespace platform {

std::string GetLinkerCommand(const std::filesystem::path& input_file_path,
                             const std::optional<std::string>& exe_filename);
std::string GetObjectFileExtension();
std::string GetExeFileExtension();

}  // namespace platform
}  // namespace coolang

#endif  // COOLANG_CODEGEN_PLATFORM_H
