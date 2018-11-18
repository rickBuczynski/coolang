#ifndef COOLANG_CODEGEN_LINUX_PLATFORM_H
#define COOLANG_CODEGEN_LINUX_PLATFORM_H

#include <filesystem>

namespace coolang {

class LinuxPlatform {
 public:
  static std::string GetObjectFileExtension() { return ".o"; }
  static std::string GetExeFileExtension() { return ""; }

  static std::string GetLinkerCommand(
      const std::filesystem::path& object_file_path,
      const std::filesystem::path& exe_file_path) {
    std::string obj_input_linker_arg = object_file_path.string();
    obj_input_linker_arg += " ";

    std::string output_exe_linker_arg = "-o ";
    output_exe_linker_arg += exe_file_path.string();
    output_exe_linker_arg += " ";

    std::string linker_cmd = "gcc ";
    linker_cmd += output_exe_linker_arg;
    linker_cmd += obj_input_linker_arg;

    return linker_cmd;
  }
};

}  // namespace coolang

#endif  // COOLANG_CODEGEN_LINUX_PLATFORM_H
