/*
 * Copyright 2019 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef COOLANG_CODEGEN_LINUX_PLATFORM_H
#define COOLANG_CODEGEN_LINUX_PLATFORM_H

#include <filesystem>
#include <iostream>

#include "coolang/codegen/bitness.h"

namespace coolang {

class LinuxPlatform {
 public:
  static std::string GetObjectFileExtension() { return ".o"; }
  static std::string GetExeFileExtension() { return ""; }

  static std::string GetLinkerCommand(
      const std::vector<std::filesystem::path>& obj_paths,
      const std::filesystem::path& exe_path, Bitness bitness) {
    switch (bitness) {
      case Bitness::x32: {
        std::cerr << "32bit is not support on Linux, only 64bit is supported.";
        abort();
      }
      case Bitness::x64:
        break;
    }

    std::string obj_input_linker_arg;

    for (const auto& obj_path : obj_paths) {
      obj_input_linker_arg += obj_path.string();
      obj_input_linker_arg += " ";
    }

    std::string output_exe_linker_arg = "-o ";
    output_exe_linker_arg += exe_path.string();
    output_exe_linker_arg += " ";

    std::string linker_cmd = "clang++ ";

    linker_cmd += output_exe_linker_arg;
    linker_cmd += obj_input_linker_arg;

    return linker_cmd;
  }
};

}  // namespace coolang

#endif  // COOLANG_CODEGEN_LINUX_PLATFORM_H
