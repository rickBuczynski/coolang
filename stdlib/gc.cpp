extern "C" int printf(const char*, ...);
extern "C" void* malloc(int size);

// TODO GCwont work for Cool Strings
struct GcObj {
  // initialize these during malloc
  // the cool program wont touch these so they will contain garbage
  GcObj* next_obj;
  GcObj* next_root;
  bool is_reachable;
  // don't initialize this during malloc
  // the cool program itself handles initializing this
  char* obj_typename;
};

template <class T>
class GcList {
 public:
  void PrintList() {
    GcObj* obj = head_;

    printf("%s start\n", T::ListName());
    while (obj != nullptr) {
      printf("obj=%d\n", reinterpret_cast<int>(obj));
      printf("  is_reachable=%d\n", static_cast<int>(obj->is_reachable));
      printf("  typename=%s\n", obj->obj_typename);

      obj = T::GetNext(obj);
    }
    printf("%s end\n\n", T::ListName());
  }

  void PushFront(GcObj* obj) {
    T::SetNext(obj, head_);
    head_ = obj;
  }

 private:
  GcObj* head_ = nullptr;
};

class GcObjList {
 public:
  static void SetNext(GcObj* obj, GcObj* next) { obj->next_obj = next; }
  static GcObj* GetNext(GcObj* obj) { return obj->next_obj; }
  static const char* ListName() { return "gc objs"; }
};

class GcRootList {
 public:
  static void SetNext(GcObj* obj, GcObj* next) { obj->next_root = next; }
  static GcObj* GetNext(GcObj* obj) { return obj->next_root; }
  static const char* ListName() { return "gc roots"; }
};

GcList<GcObjList> gc_obj_list;
GcList<GcRootList> gc_root_list;

extern "C" void print_gc_obj_list() { gc_obj_list.PrintList(); }

extern "C" void* gc_malloc(int size) {
  auto* obj = static_cast<GcObj*>(malloc(size));

  obj->next_obj = nullptr;
  obj->next_root = nullptr;
  obj->is_reachable = false;

  gc_obj_list.PushFront(obj);

  return static_cast<void*>(obj);
}

extern "C" void gc_add_root(GcObj* root) {
  // TODO handle root being added is null
  gc_root_list.PushFront(root);

  gc_root_list.PrintList();
}