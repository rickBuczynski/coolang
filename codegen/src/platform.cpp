// Copyright 2019 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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
                             const std::filesystem::path& gc_obj_path,
                             const std::filesystem::path& exe_path) {
  return PlatformClass::GetLinkerCommand(obj_path, gc_obj_path, exe_path);
}

std::string GetObjectFileExtension() {
  return PlatformClass::GetObjectFileExtension();
}
std::string GetExeFileExtension() {
  return PlatformClass::GetExeFileExtension();
}

}  // namespace platform
}  // namespace coolang