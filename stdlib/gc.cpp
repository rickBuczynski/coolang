#include <cstdio>
#include <cstdlib>

/*
class FILE;

extern "C" int printf(const char*, ...);
extern "C" int fprintf(FILE *stream, const char *format, ...);
extern "C" void* malloc(int size);
*/

// TODO refactor extern api to just:
// gc_malloc(int size)
// print_malloc() maybe takes in the malloced obj, or maybe gc system keeps
// track of it similar api for roots only call printing apis from codegen if
// verbose flag is set

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
  int gc_pointer_count;
  char* obj_typename;
};

void PrintObj(const GcObj* obj) {
  printf("obj\n");
  // fprintf(stderr, "  address=%d\n", reinterpret_cast<int>(obj));
  printf("  is_reachable=%d\n", static_cast<int>(obj->is_reachable));
  printf("  gc_pointer_count=%d\n", obj->gc_pointer_count);
  printf("  typename=%s\n", obj->obj_typename);
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

  GcRoot* roots;
  int capacity;
  int length;
};

template <class T>
class GcList {
 public:
  void PrintList() {
    GcObj* obj = head_;
    GcObj* prev = nullptr;

    printf("%s start\n", T::ListName());
    while (obj != nullptr) {
      PrintObj(obj);

      if (prev != T::GetPrev(obj)) {
        printf("  BADBADBADBADBADBADBADBADBADBADBADBADBADBAD\n");
      }

      prev = obj;
      obj = T::GetNext(obj);
    }
    printf("%s end\n\n", T::ListName());
  }

  void Remove(GcObj* obj) {
    GcObj* prev = T::GetPrev(obj);
    GcObj* next = T::GetNext(obj);

    if (prev == nullptr && obj != head_) {
      printf("Tried to remove an obj that's not in list: %s\n", T::ListName());
      return;
    }

    if (obj == head_) {
      head_ = T::GetNext(obj);
    }

    if (prev != nullptr) {
      T::SetNext(prev, next);
    }

    if (next != nullptr) {
      T::SetPrev(next, prev);
    }
  }

  void PushFront(GcObj* obj) {
    T::SetNext(obj, head_);
    if (head_ != nullptr) {
      T::SetPrev(head_, obj);
    }
    head_ = obj;
  }

  const GcObj* GetHead() const { return head_; };

 private:
  GcObj* head_ = nullptr;
};

class GcObjList {
 public:
  static void SetNext(GcObj* obj, GcObj* next) { obj->next_obj = next; }
  static void SetPrev(GcObj* obj, GcObj* prev) { obj->prev_obj = prev; }
  static GcObj* GetNext(GcObj* obj) { return obj->next_obj; }
  static GcObj* GetPrev(GcObj* obj) { return obj->prev_obj; }
  static const char* ListName() { return "gc objs"; }
};

GcList<GcObjList>* gc_obj_list;
GcRootStack* gc_roots;

// TODO use gc_is_allowed to block GC while evaluating function args
// e.g. f(new A, 3, new B)
// we don't want "new B" to trigger GC of "new A" before we go into the function
// and add the function args as GC roots
bool gc_is_allowed = false;

bool gc_is_verbose = false;

extern "C" void gc_system_init(int is_verbose) {
  gc_obj_list = new GcList<GcObjList>;
  gc_roots = new GcRootStack;
  gc_is_allowed = true;
  gc_is_verbose = is_verbose;
}

extern "C" void gc_system_destroy() {
  gc_is_allowed = false;
  delete gc_roots;
  delete gc_obj_list;
}

extern "C" void* gc_malloc(int size) {
  auto* obj = static_cast<GcObj*>(malloc(size));

  obj->next_obj = nullptr;
  obj->prev_obj = nullptr;
  obj->next_root = nullptr;
  obj->prev_root = nullptr;
  obj->is_reachable = false;

  gc_obj_list->PushFront(obj);

  return static_cast<void*>(obj);
}

extern "C" void gc_malloc_print() {
  if (gc_is_verbose) {
    printf("Allocated:\n");
    PrintObj(gc_obj_list->GetHead());
    printf("\n");

    gc_obj_list->PrintList();
  }
}

extern "C" void gc_add_root(GcObj** root) {
  if (gc_is_verbose) {
    printf("Inserting a root that points to:\n");
    PrintObj(*root);
    printf("\n");
  }

  gc_roots->PushRoot(root);

  if (gc_is_verbose) {
    gc_roots->PrintRoots();
  }
}

extern "C" void gc_remove_root(GcObj** root) {
  if (gc_is_verbose) {
    printf("Removing a root that points to:\n");
    PrintObj(*root);
    printf("\n");
  }

  gc_roots->PopRoot(root);

  if (gc_is_verbose) {
    gc_roots->PrintRoots();
  }
}