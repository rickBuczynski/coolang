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

#ifndef COOLANG_CODEGEN_PLATFORM_H
#define COOLANG_CODEGEN_PLATFORM_H

#include <filesystem>

namespace coolang {
namespace platform {

std::string GetLinkerCommand(const std::filesystem::path& obj_path,
                             const std::filesystem::path& std_lib_obj_path,
                             const std::filesystem::path& exe_path);
std::string GetObjectFileExtension();
std::string GetExeFileExtension();

}  // namespace platform
}  // namespace coolang

#endif  // COOLANG_CODEGEN_PLATFORM_H
