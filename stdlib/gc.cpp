#include <cstdio>
#include <cstdlib>

/*
class FILE;

extern "C" int printf(const char*, ...);
extern "C" int fprintf(FILE *stream, const char *format, ...);
extern "C" void* malloc(int size);
*/

bool gc_is_verbose = false;

struct GcObj;

struct GcPtrsInfo {
  int gc_ptr_count;
  GcObj** gc_ptrs;
  GcPtrsInfo* next_gc_info;
};

// TODO GCwont work for Cool Strings
struct GcObj {
  // initialize these during malloc
  // the cool program wont touch these so they will contain garbage
  GcObj* next_obj;
  GcObj* prev_obj;
  GcObj* next_root;
  GcObj* prev_root;
  bool is_reachable;
  // don't initialize this during malloc
  // the cool program itself handles initializing this
  char* obj_typename;
  void* vtable;
  int typesize;
  void* constructor;
  char* boxed_data;

  GcPtrsInfo gc_ptrs_info;
};

void PrintObj(const GcObj* obj) {
  printf("obj\n");
  // fprintf(stderr, "  address=%d\n", reinterpret_cast<int>(obj));
  printf("  is_reachable=%d\n", static_cast<int>(obj->is_reachable));
  printf("  typename=%s\n", obj->obj_typename);
}

void SetNext(GcObj* obj, GcObj* next) { obj->next_obj = next; }
void SetPrev(GcObj* obj, GcObj* prev) { obj->prev_obj = prev; }
GcObj* GetNext(GcObj* obj) { return obj->next_obj; }
GcObj* GetPrev(GcObj* obj) { return obj->prev_obj; }
const char* ListName() { return "gc objs"; }

void MarkObj(GcObj* obj) {
  if (obj == nullptr || obj->is_reachable) {
    return;
  }

  obj->is_reachable = true;

  GcPtrsInfo* gc_ptrs_info = &obj->gc_ptrs_info;
  while (gc_ptrs_info != nullptr) {
    for (int i = 0; i < gc_ptrs_info->gc_ptr_count; i++) {
      MarkObj(gc_ptrs_info->gc_ptrs[i]);
    }
    gc_ptrs_info = gc_ptrs_info->next_gc_info;
  }
}

using GcRoot = GcObj**;

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
    if (roots[length - 1] != root) {
      printf("BADBADBADBADBADBADBADBADBAD\n");
      printf("Root at top of stack points to:\n");
      PrintObj(*roots[length]);
      printf("Root to remove points to:\n");
      PrintObj(*root);
    }
    length--;
  }

  void PrintRoots() const {
    printf("Current GC roots:\n");
    for (int i = 0; i < length; i++) {
      printf("Root that points to:\n");
      PrintObj(*roots[i]);
    }
    printf("End of current GC roots\n\n");
  }

  void MarkReachable() const {
    for (int i = 0; i < length; i++) {
      MarkObj(*roots[i]);
    }
  }

  GcRoot* roots;
  int capacity;
  int length;
};

class GcList {
 public:
  void PrintList() const {
    GcObj* obj = head_;
    GcObj* prev = nullptr;

    printf("%s start\n", ListName());
    while (obj != nullptr) {
      PrintObj(obj);

      if (prev != GetPrev(obj)) {
        printf("  BADBADBADBADBADBADBADBADBADBADBADBADBADBAD\n");
      }

      prev = obj;
      obj = GetNext(obj);
    }
    printf("%s end\n\n", ListName());
  }

  void UnmarkList() const {
    GcObj* obj = head_;
    while (obj != nullptr) {
      obj->is_reachable = false;
      obj = GetNext(obj);
    }
  }

  void Sweep() {
    GcObj* obj = head_;
    while (obj != nullptr) {
      GcObj* next = GetNext(obj);

      if (!obj->is_reachable) {
        if (gc_is_verbose) {
          printf("Freeing an object of type: %s\n", obj->obj_typename);
        }

        Remove(obj);
        free(obj);
      }

      obj = next;
    }
  }

  void Remove(GcObj* obj) {
    GcObj* prev = GetPrev(obj);
    GcObj* next = GetNext(obj);

    if (prev == nullptr && obj != head_) {
      printf("Tried to remove an obj that's not in list: %s\n", ListName());
      return;
    }

    if (obj == head_) {
      head_ = GetNext(obj);
    }

    if (prev != nullptr) {
      SetNext(prev, next);
    }

    if (next != nullptr) {
      SetPrev(next, prev);
    }
  }

  void PushFront(GcObj* obj) {
    SetNext(obj, head_);
    if (head_ != nullptr) {
      SetPrev(head_, obj);
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
}

extern "C" void gc_system_destroy() {
  delete gc_roots;
  delete gc_obj_list;
}

void Collect() {
  gc_obj_list->UnmarkList();
  gc_roots->MarkReachable();
  gc_obj_list->Sweep();
}

extern "C" void* gc_malloc(int size) {
  Collect();

  auto* obj = static_cast<GcObj*>(malloc(size));

  obj->next_obj = nullptr;
  obj->prev_obj = nullptr;
  obj->next_root = nullptr;
  obj->prev_root = nullptr;
  obj->is_reachable = false;

  gc_obj_list->PushFront(obj);

  return static_cast<void*>(obj);
}

extern "C" void gc_add_root(GcObj** root) { gc_roots->PushRoot(root); }

extern "C" void gc_remove_root(GcObj** root) { gc_roots->PopRoot(root); }