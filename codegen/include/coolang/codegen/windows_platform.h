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

#ifndef COOLANG_CODEGEN_WINDOWS_PLATFORM_H
#define COOLANG_CODEGEN_WINDOWS_PLATFORM_H

#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <set>

#include "coolang/codegen/bitness.h"
#include "windows_com/windows_com.hpp"

namespace coolang {

class WindowsPlatform {
 public:
  static std::string BitStr(Bitness bitness) {
    switch (bitness) {
      case Bitness::x32:
        return "x86";
      case Bitness::x64:
        return "x64";
    }
  }

  static std::string GetLinkerCommand(
      const std::filesystem::path& obj_path,
      const std::filesystem::path& std_lib_obj_path,
      const std::filesystem::path& exe_path, Bitness bitness) {
    std::string obj_input_linker_arg = obj_path.string();
    obj_input_linker_arg += " ";

    std::string output_exe_linker_arg = "-OUT:";
    output_exe_linker_arg += exe_path.string();
    output_exe_linker_arg += " ";

    const std::string bit_str = BitStr(bitness);

    const auto msvc_linker_path = GetMsvcLinkerPath(bit_str);
    const auto msvc_lib_path = GetMsvcLibPath(bit_str);

    if (!msvc_linker_path.has_value() || !msvc_lib_path.has_value()) {
      std::cerr << "Linking failed because a Visual Studio 2017 installation "
                   "wasn't found."
                << std::endl;
      std::exit(1);
    }

    const auto win10_kit_lib_path = GetLatestWindows10KitLibPath();

    if (!win10_kit_lib_path.has_value()) {
      std::cerr << "Linking failed because a Windows 10 SDK wasn't found."
                << std::endl;
      std::exit(1);
    }

    const auto um_path = (win10_kit_lib_path.value() / "um" / bit_str).string();
    const auto ucrt_path =
        (win10_kit_lib_path.value() / "ucrt" / bit_str).string();

    std::string linker_cmd = "cmd /C \"";
    linker_cmd += "\"" + msvc_linker_path.value().string() + "\" ";
    linker_cmd += obj_path.string() + " ";
    linker_cmd += std_lib_obj_path.string() + " ";
    linker_cmd += output_exe_linker_arg;
    linker_cmd += "libcmt.lib ";
    // TODO windows requires legacy_stdio_definitions.lib in new versions of
    // MSVC to link against printf but not in old versions
    // https://github.com/rust-lang/rust/issues/42830
    linker_cmd += "legacy_stdio_definitions.lib ";
    linker_cmd += "-LIBPATH:\"" + msvc_lib_path.value().string() + "\" ";
    linker_cmd += "-LIBPATH:\"" + um_path + "\" ";
    linker_cmd += "-LIBPATH:\"" + ucrt_path + "\" ";
    linker_cmd += "-SUBSYSTEM:CONSOLE ";
    linker_cmd += "-NOLOGO";
    linker_cmd += "\"";

    return linker_cmd;
  }

  static std::string GetObjectFileExtension() { return ".obj"; }
  static std::string GetExeFileExtension() { return ".exe"; }

 private:
  // Gets the path which contains all Windows 10 SDKs usually:
  // C:\Program Files (x86)\Windows Kits\10\Lib
  // which will contain subdirectories for each SDK version
  static std::optional<std::filesystem::path> GetWindows10KitsPath() {
    HKEY roots_key;
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                     L"SOFTWARE\\Microsoft\\Windows Kits\\Installed Roots", 0,
                     KEY_QUERY_VALUE | KEY_WOW64_32KEY | KEY_ENUMERATE_SUB_KEYS,
                     &roots_key) != ERROR_SUCCESS) {
      return std::nullopt;
    }

    DWORD tmp_buf_len = MAX_PATH;
    const auto* kits_root_10 =
        static_cast<const WCHAR*>(calloc(tmp_buf_len, sizeof(WCHAR)));
    if (RegQueryValueEx(roots_key, L"KitsRoot10", NULL, NULL,
                        LPBYTE(kits_root_10), &tmp_buf_len) != ERROR_SUCCESS) {
      return std::nullopt;
    }

    std::wstring kits_root_10_ws(kits_root_10);
    free((void*)kits_root_10);

    return std::filesystem::path(std::move(kits_root_10_ws)) / "Lib";
  }

  static std::optional<std::filesystem::path> GetLatestWindows10KitLibPath() {
    const auto kits_path = GetWindows10KitsPath();
    if (!kits_path.has_value()) {
      return std::nullopt;
    }

    std::set<std::filesystem::path> kit_versions;

    for (const auto& kit_version :
         std::filesystem::directory_iterator(kits_path.value())) {
      if (std::filesystem::exists(kit_version.path() / "ucrt") &&
          std::filesystem::exists(kit_version.path() / "um")) {
        kit_versions.insert(kit_version);
      }
    }
    if (kit_versions.empty()) {
      return std::nullopt;
    }
    return *kit_versions.crbegin();
  }

  // see
  // https://github.com/ziglang/zig/blob/395f3d946ad62eb881796044819194f649742a65/src/windows_sdk.cpp
  static std::optional<std::filesystem::path> GetMsvcInstallPath() {
    // COM Smart Pointers requires explicit scope
    {
      const HRESULT rc = CoInitializeEx(NULL, COINIT_MULTITHREADED);
      if (rc != S_OK && rc != S_FALSE) {
        return std::nullopt;
      }

      ISetupConfigurationPtr setup_config;
      if (setup_config.CreateInstance(__uuidof(SetupConfiguration)) != S_OK) {
        return std::nullopt;
      }

      IEnumSetupInstancesPtr all_instances;
      if (setup_config->EnumInstances(&all_instances) != S_OK) {
        return std::nullopt;
      }

      ISetupInstance* cur_instance;
      ULONG found_inst;
      while (all_instances->Next(1, &cur_instance, &found_inst) == S_OK) {
        BSTR install_path;
        if (cur_instance->GetInstallationPath(&install_path) != S_OK) {
          return std::nullopt;
        }
        return std::filesystem::path(std::wstring(install_path));
      }
    }
    return std::nullopt;
  }

  static std::optional<std::filesystem::path> GetMsvcToolsPath() {
    const auto msvc_install_path = GetMsvcInstallPath();
    if (!msvc_install_path.has_value()) {
      return std::nullopt;
    }

    std::ifstream vc_tools_version_file(
        msvc_install_path.value() /
        R"(VC\Auxiliary\Build\Microsoft.VCToolsVersion.default.txt)");

    if (!vc_tools_version_file.is_open()) {
      return std::nullopt;
    }

    std::string tools_version;
    vc_tools_version_file >> tools_version;

    return msvc_install_path.value() / R"(VC\Tools\MSVC)" / tools_version;
  }

  static std::optional<std::filesystem::path> GetMsvcLinkerPath(
      const std::string& bit_str) {
    const auto msvc_tool_path = GetMsvcToolsPath();
    if (!msvc_tool_path.has_value()) {
      return std::nullopt;
    }
    return msvc_tool_path.value() / "bin" / ("Host" + bit_str) / bit_str /
           "link.exe";
  }

  static std::optional<std::filesystem::path> GetMsvcLibPath(
      const std::string& bit_str) {
    const auto msvc_tool_path = GetMsvcToolsPath();
    if (!msvc_tool_path.has_value()) {
      return std::nullopt;
    }
    return msvc_tool_path.value() / "lib" / bit_str;
  }
};

}  // namespace coolang

#endif  // COOLANG_CODEGEN_WINDOWS_PLATFORM_H
