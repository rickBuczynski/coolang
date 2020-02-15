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

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>

extern "C" char* gc_malloc_string(int size);

extern "C" void* IO_out_string(void* io, char* str) {
  printf("%s", str);
  return io;
}

extern "C" void* IO_out_int(void* io, int i) {
  printf("%d", i);
  return io;
}

extern "C" char* IO_in_string(void* io) {
  static constexpr int init_buf_size = 250;

  int i = 0;
  int buf_size = init_buf_size;
  char* buf = static_cast<char*>(malloc(buf_size));
  if (buf == nullptr) return nullptr;
  for (char c = getchar(); c != 0 && c != '\n' && c != EOF; c = getchar()) {
    buf[i] = c;
    i++;
    if (i == buf_size) {
      buf_size *= 2;
      char* new_buf = static_cast<char*>(realloc(buf, buf_size));
      if (new_buf == nullptr) {
        free(buf);
        return nullptr;
      } else {
        buf = new_buf;
      }
    }
  }
  buf[i] = 0;
  i++;
  char* gc_str = gc_malloc_string(i);
  memcpy(gc_str, buf, i);
  free(buf);
  return gc_str;
}
