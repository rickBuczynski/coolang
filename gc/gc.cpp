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

bool gc_is_verbose = false;

struct GcObj;

struct GcPtrsInfo {
  int gc_ptr_count;
  GcObj** gc_ptrs;
  int gc_str_count;
  char** gc_strs;
  GcPtrsInfo* next_gc_info;
};

struct GcObj {
  // initialize these during malloc
  // the cool program wont touch these so they will contain garbage
  GcObj* next_obj;
  GcObj* prev_obj;
  bool is_reachable;
  // don't initialize this during malloc
  // the cool program itself handles initializing this
  const char* obj_typename;
  void* vtable;
  int typesize;
  void* constructor;
  void* copy_constructor;
  char* boxed_data;

  GcPtrsInfo gc_ptrs_info;
};

GcObj* ObjFromString(char* str_data_start) {
  if (str_data_start == nullptr) {
    return nullptr;
  }
  char* str_obj_start = str_data_start - sizeof(GcObj);
  GcObj* obj = reinterpret_cast<GcObj*>(str_obj_start);
  return obj;
}

void MarkObj(GcObj* obj) {
  if (obj == nullptr || obj->is_reachable) {
    return;
  }

  obj->is_reachable = true;

  if (obj->boxed_data != nullptr && strcmp(obj->obj_typename, "String") == 0) {
    MarkObj(ObjFromString(obj->boxed_data));
  }

  GcPtrsInfo* gc_ptrs_info = &obj->gc_ptrs_info;

  while (gc_ptrs_info != nullptr) {
    for (int i = 0; i < gc_ptrs_info->gc_ptr_count; i++) {
      MarkObj(gc_ptrs_info->gc_ptrs[i]);
    }
    for (int i = 0; i < gc_ptrs_info->gc_str_count; i++) {
      MarkObj(ObjFromString(gc_ptrs_info->gc_strs[i]));
    }
    gc_ptrs_info = gc_ptrs_info->next_gc_info;
  }
}

int gc_alloc_count = 0;
int gc_free_count = 0;

class GcRoot {
 public:
  explicit GcRoot(GcObj** obj_root) : obj_root_(obj_root) {}
  explicit GcRoot(char** string_root) : string_root_(string_root) {}

  GcObj* Obj() const {
    if (obj_root_ != nullptr) {
      assert(string_root_ == nullptr);
      return *obj_root_;
    }
    assert(string_root_ != nullptr);
    return ObjFromString(*string_root_);
  }

 private:
  GcObj** obj_root_ = nullptr;
  char** string_root_ = nullptr;
};

struct GcRootStack {
  static constexpr int init_capacity = 2;

  GcRootStack() {
    roots = static_cast<GcRoot*>(malloc(init_capacity * sizeof(GcRoot)));
    capacity = init_capacity;
    length = 0;
  }

  ~GcRootStack() { free(roots); }

  void PushRoot(GcRoot root) {
    if (length == capacity) {
      capacity *= 2;
      roots = static_cast<GcRoot*>(realloc(roots, capacity * sizeof(GcRoot)));
    }

    roots[length] = root;
    length++;
  }

  void PopRoot(GcRoot root) {
    assert(roots[length - 1].Obj() == root.Obj());
    length--;
  }

  void MarkReachable() const {
    for (int i = 0; i < length; i++) {
      MarkObj(roots[i].Obj());
    }
  }

  GcRoot* roots;
  int capacity;
  int length;
};

class GcList {
 public:
  void UnmarkList() const {
    GcObj* obj = head_;
    while (obj != nullptr) {
      obj->is_reachable = false;
      obj = obj->next_obj;
    }
  }

  void Sweep() {
    GcObj* obj = head_;
    while (obj != nullptr) {
      GcObj* next = obj->next_obj;

      if (!obj->is_reachable) {
        if (gc_is_verbose) {
          printf("Freeing an object of type: %s\n", obj->obj_typename);
        }

        gc_free_count++;
        Remove(obj);
        free(obj);
      }

      obj = next;
    }
  }

  void Remove(GcObj* obj) {
    GcObj* prev = obj->prev_obj;
    GcObj* next = obj->next_obj;

    assert(prev != nullptr || obj == head_);

    if (obj == head_) {
      head_ = obj->next_obj;
    }

    if (prev != nullptr) {
      prev->next_obj = next;
    }

    if (next != nullptr) {
      next->prev_obj = prev;
    }
  }

  void PushFront(GcObj* obj) {
    obj->next_obj = head_;
    if (head_ != nullptr) {
      head_->prev_obj = obj;
    }
    head_ = obj;
  }

  const GcObj* GetHead() const { return head_; };

 private:
  GcObj* head_ = nullptr;
};

GcList* gc_obj_list;
GcRootStack* gc_roots;

extern "C" void gc_system_init(int is_verbose) {
  gc_obj_list = new GcList;
  gc_roots = new GcRootStack;
  gc_is_verbose = is_verbose;
  gc_alloc_count = 0;
  gc_free_count = 0;
}

void Collect() {
  gc_obj_list->UnmarkList();
  gc_roots->MarkReachable();
  gc_obj_list->Sweep();
}

extern "C" void gc_system_destroy() {
  Collect();
  delete gc_roots;
  delete gc_obj_list;
  if (gc_alloc_count != gc_free_count) {
    printf("GC alloc count: %d\n", gc_alloc_count);
    printf("GC free count: %d\n", gc_free_count);
    printf("GC leak count: %d\n", gc_alloc_count - gc_free_count);
  }
}

extern "C" void* gc_malloc(int size, char* obj_typename) {
  Collect();

  auto* obj = static_cast<GcObj*>(malloc(size));
  gc_alloc_count++;

  if (gc_is_verbose) {
    printf("Allocated an object of type: %s\n", obj_typename);
  }

  obj->next_obj = nullptr;
  obj->prev_obj = nullptr;
  obj->is_reachable = false;

  gc_obj_list->PushFront(obj);

  return static_cast<void*>(obj);
}

extern "C" void gc_copy_obj(GcObj* copy_dst, GcObj* copy_src, int size) {
  // start the copy from &is_reachable so we don't copy next_obj and prev_obj
  // pointers. We want the copy to have it's own next_obj prev_obj not the same
  // as the original.
  const size_t dont_copy_size =
      reinterpret_cast<char*>(&copy_dst->is_reachable) -
      reinterpret_cast<char*>(copy_dst);
  memcpy(&copy_dst->is_reachable, &copy_src->is_reachable,
         size - dont_copy_size);
}

extern "C" void* gc_malloc_string(int size) {
  Collect();

  if (gc_is_verbose) {
    printf("Allocated a string\n");
  }
  char* str_alloc = static_cast<char*>(malloc(sizeof(GcObj) + size));
  gc_alloc_count++;

  GcObj* obj = reinterpret_cast<GcObj*>(str_alloc);

  obj->next_obj = nullptr;
  obj->prev_obj = nullptr;
  obj->is_reachable = false;
  obj->obj_typename = "String";
  obj->boxed_data = nullptr;
  obj->gc_ptrs_info.gc_ptr_count = 0;
  obj->gc_ptrs_info.gc_ptrs = nullptr;
  obj->gc_ptrs_info.gc_str_count = 0;
  obj->gc_ptrs_info.gc_strs = nullptr;
  obj->gc_ptrs_info.next_gc_info = nullptr;

  gc_obj_list->PushFront(obj);

  char* str_data_start = str_alloc + sizeof(GcObj);
  return static_cast<void*>(str_data_start);
}

extern "C" void gc_add_root(GcObj** root) { gc_roots->PushRoot(GcRoot(root)); }
extern "C" void gc_add_string_root(char** root) {
  gc_roots->PushRoot(GcRoot(root));
}

extern "C" void gc_remove_root(GcObj** root) {
  gc_roots->PopRoot(GcRoot(root));
}
extern "C" void gc_remove_string_root(char** root) {
  gc_roots->PopRoot(GcRoot(root));
}
