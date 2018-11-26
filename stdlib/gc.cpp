extern "C" int printf(const char*, ...);
extern "C" void* malloc(int size);

struct GcObj {
  void* obj;
  bool is_reachable = false;
  GcObj* next = nullptr;
};

GcObj* gc_obj_list = nullptr;

void PrintList(GcObj* list) {
  printf("list start\n");
  while (list != nullptr) {
    printf("obj=%d\n", (int)list->obj);
    printf("is_reachable=%d\n", (int)list->is_reachable);
    // TOOD try getting the type name, need to cast obj pointer to int** and increment then cast to char*, should point to typename

    char** obj_vtable_ptr = (char**)list->obj;
    char** typename_ptr = obj_vtable_ptr + 1;

    printf("typename=%s\n", *typename_ptr);

    list = list->next;
  }
  printf("list end\n");
}

extern "C" void* gc_malloc(int size) {
  // need to print before the new malloc because the new obj wont have typename set untill it's constructor is called
  PrintList(gc_obj_list);

  void* obj = malloc(size);
 
  GcObj* old_head = gc_obj_list;
  GcObj* new_gc_obj = new GcObj;
  new_gc_obj->obj = obj;
  new_gc_obj->next = old_head;
  gc_obj_list = new_gc_obj;

  return obj;
}