#include "coolang/codegen/platform.h"

#ifdef _WIN32
#include "coolang/codegen/windows_platform.h"
#endif

#ifdef __unix__
#include "coolang/codegen/linux_platform.h"
#endif

namespace coolang {
namespace platform {

#ifdef _WIN32
using PlatformClass = WindowsPlatform;
#endif

#ifdef __unix__
using PlatformClass = LinuxPlatform;
#endif

std::string GetLinkerCommand(const std::filesystem::path& input_file_path,
                             const std::optional<std::string>& exe_filename) {
  std::filesystem::path object_file_path = input_file_path;
  object_file_path.replace_extension(PlatformClass::GetObjectFileExtension());

  std::filesystem::path exe_file_path = input_file_path;
  if (exe_filename.has_value()) {
    exe_file_path.replace_filename(exe_filename.value());
  }
  exe_file_path.replace_extension(PlatformClass::GetExeFileExtension());

  return PlatformClass::GetLinkerCommand(object_file_path, exe_file_path);
}

std::string GetObjectFileExtension() {
  return PlatformClass::GetObjectFileExtension();
}
std::string GetExeFileExtension() {
  return PlatformClass::GetExeFileExtension();
}

}  // namespace platform
}  // namespace coolang