#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>

extern "C" void gc_add_string_root(char** root);
extern "C" void gc_remove_string_root(char** root);
extern "C" char* gc_malloc_string(int size);

extern "C" char* String_concat(char* lhs, char* rhs) {
  // Need GC roots to preserve params since this func allocates
  gc_add_string_root(&lhs);
  gc_add_string_root(&rhs);

  int concated_size = strlen(lhs) + strlen(rhs) + 1;
  char* concated_str = gc_malloc_string(concated_size);

  strcpy(concated_str, lhs);
  strcat(concated_str, rhs);

  gc_remove_string_root(&rhs);
  gc_remove_string_root(&lhs);

  return concated_str;
}

extern "C" char* String_substr(char* str, int start_index, int substr_len) {
  // Need a GC root to preserve self param since this func allocates
  gc_add_string_root(&str);

  char* substr = gc_malloc_string(substr_len + 1);
  strncpy(substr, str + start_index, substr_len);
  substr[substr_len] = 0;

  gc_remove_string_root(&str);

  return substr;
}

extern "C" int String_length(char* str) { return strlen(str); }
