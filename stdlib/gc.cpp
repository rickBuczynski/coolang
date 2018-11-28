extern "C" int printf(const char*, ...);
extern "C" void* malloc(int size);

// TODO GCwont work for Cool Strings
struct GcObj {
  GcObj* next_obj;
  GcObj* next_root;
  bool is_reachable;
  char* obj_typename;
};

GcObj* gc_obj_list = nullptr;
GcObj* gc_root_list = nullptr;

void PrintList(bool print_roots) {
  GcObj* list;
  if (print_roots) {
    list = gc_root_list;
  } else {
    list = gc_obj_list;
  }

  printf("%s start\n", print_roots ? "gc roots" : "gc objs");
  while (list != nullptr) {
    printf("obj=%d\n", (int)list);
    printf("  is_reachable=%d\n", (int)list->is_reachable);
    printf("  typename=%s\n", list->obj_typename);

    if (print_roots) {
      list = list->next_root;
    } else {
      list = list->next_obj;
    }
  }
  printf("%s end\n\n", print_roots ? "gc roots" : "gc objs");
}

extern "C" void* gc_malloc(int size) {
  // need to print before the new malloc because the new obj wont have typename set untill it's constructor is called
  PrintList(false);

  GcObj* obj = (GcObj*)malloc(size);

  obj->next_obj = gc_obj_list;
  obj->next_root = nullptr;
  obj->is_reachable = false;
  
  gc_obj_list = obj;

  return (void*)obj;
}

extern "C" void gc_add_root(GcObj* root) {
  // need to print before the new malloc because the new obj wont have typename set untill it's constructor is called
  PrintList(true);

  // TODO handle root is null
  root->next_root = gc_root_list;

  gc_root_list = root;
}