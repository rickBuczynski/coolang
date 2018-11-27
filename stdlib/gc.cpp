extern "C" int printf(const char*, ...);
extern "C" void* malloc(int size);

struct GcObj {
  GcObj* next;
  bool is_reachable;
  char* obj_typename;
};

GcObj* gc_obj_list = nullptr;

void PrintList(GcObj* list) {
  printf("list start\n");
  while (list != nullptr) {
    printf("obj=%d\n", (int)list);
    printf("is_reachable=%d\n", (int)list->is_reachable);
    printf("typename=%s\n", list->obj_typename);

    list = list->next;
  }
  printf("list end\n");
}

// TODO wont work for Cool Strings
extern "C" void* gc_malloc(int size) {
  // need to print before the new malloc because the new obj wont have typename set untill it's constructor is called
  PrintList(gc_obj_list);

  GcObj* old_head = gc_obj_list;

  GcObj* obj = (GcObj*)malloc(size);

  obj->next = old_head;
  obj->is_reachable = false;
  
  gc_obj_list = obj;

  return (void*)obj;
}