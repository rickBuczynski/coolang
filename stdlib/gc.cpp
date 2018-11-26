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
    list = list->next;
  }
  printf("list end\n");
}

extern "C" void* gc_malloc(int size) {
  //printf("qwe %s\n", "gc_malloc");
  void* obj = malloc(size);
 
  GcObj* old_head = gc_obj_list;
  GcObj* new_gc_obj = new GcObj;
  new_gc_obj->obj = obj;
  new_gc_obj->next = old_head;
  gc_obj_list = new_gc_obj;

  PrintList(gc_obj_list);
  
  return obj;
}