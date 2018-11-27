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

void PrintList(GcObj* list, bool print_roots) {
  printf("list start\n");
  while (list != nullptr) {
    printf("obj=%d\n", (int)list);
    printf("is_reachable=%d\n", (int)list->is_reachable);
    printf("typename=%s\n", list->obj_typename);

    if (print_roots) {
      list = list->next_root;
    } else {
      list = list->next_obj;
    }
  }
  printf("list end\n\n");
}

extern "C" void* gc_malloc(int size) {
  // need to print before the new malloc because the new obj wont have typename set untill it's constructor is called
  PrintList(gc_obj_list, true);

  GcObj* obj = (GcObj*)malloc(size);

  obj->next_obj = gc_obj_list;
  obj->next_root = nullptr;
  obj->is_reachable = false;
  
  gc_obj_list = obj;

  return (void*)obj;
}

extern "C" void gc_add_root(GcObj* root) {
  // need to print before the new malloc because the new obj wont have typename set untill it's constructor is called
  PrintList(gc_root_list, true);

  root->next_root = gc_root_list;

  gc_root_list = root;
}