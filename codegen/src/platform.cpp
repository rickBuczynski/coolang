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

std::string GetLinkerCommand(const std::filesystem::path& obj_path,
                             const std::filesystem::path& exe_path) {
  return PlatformClass::GetLinkerCommand(obj_path, exe_path);
}

std::string GetObjectFileExtension() {
  return PlatformClass::GetObjectFileExtension();
}
std::string GetExeFileExtension() {
  return PlatformClass::GetExeFileExtension();
}

}  // namespace platform
}  // namespace coolang