#ifndef COOLANG_CODEGEN_PLATFORM_H
#define COOLANG_CODEGEN_PLATFORM_H

#include <filesystem>

namespace coolang {
namespace platform {

inline std::string GetObjectFileExtension() {
#ifdef _WIN32
  return ".obj";
#endif

#ifdef __unix__
  return ".o";
#endif
}

inline std::string GetExeFileExtension() {
#ifdef _WIN32
  return ".exe";
#endif

#ifdef __unix__
  return "";
#endif
}

inline std::string GetLinkerCommandWindows(
    const std::filesystem::path& object_file_path,
    const std::filesystem::path& exe_file_path) {
  std::string obj_input_linker_arg = object_file_path.string();
  obj_input_linker_arg += " ";

  std::string output_exe_linker_arg = "-OUT:";
  output_exe_linker_arg += exe_file_path.string();
  output_exe_linker_arg += " ";

  // clang-format off
  std::string linker_cmd = "cmd /C \"";
  linker_cmd += "\"C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Tools/MSVC/14.14.26428/bin/Hostx86/x86/link.exe\" ";
  linker_cmd += obj_input_linker_arg;
  linker_cmd += output_exe_linker_arg;
  linker_cmd += "libcmt.lib ";
  // TODO windows requires legacy_stdio_definitions.lib in new versions of MSVC to link against printf but not in old versions
  // https://github.com/rust-lang/rust/issues/42830
  linker_cmd += "legacy_stdio_definitions.lib ";
  linker_cmd += "-LIBPATH:\"C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Tools/MSVC/14.14.26428/lib/x86\" ";
  linker_cmd += "-LIBPATH:\"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.16299.0/um/x86\" ";
  linker_cmd += "-LIBPATH:\"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.16299.0/ucrt/x86\" ";
  linker_cmd += "-SUBSYSTEM:CONSOLE ";
  linker_cmd += "-MACHINE:X86";
  linker_cmd += "\"";

  return linker_cmd;
}

inline std::string GetLinkerCommandLinux(
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

inline std::string GetLinkerCommand(
    const std::filesystem::path& input_file_path,
    const std::optional<std::string>& exe_filename) {
  std::filesystem::path object_file_path = input_file_path;
  object_file_path.replace_extension(platform::GetObjectFileExtension());

  std::filesystem::path exe_file_path = input_file_path;
  if (exe_filename.has_value()) {
    exe_file_path.replace_filename(exe_filename.value());
  }
  exe_file_path.replace_extension(platform::GetExeFileExtension());

#ifdef _WIN32
  return GetLinkerCommandWindows(object_file_path, exe_file_path);
#endif

#ifdef __unix__
  return GetLinkerCommandLinux(object_file_path, exe_file_path);
#endif
}

}  // namespace platform
}  // namespace coolang

#endif  // COOLANG_CODEGEN_PLATFORM_H
