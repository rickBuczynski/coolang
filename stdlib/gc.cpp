void* gc_malloc(int size) {
  printf("hello %s\n", "gc_malloc");
  return malloc(size);
}