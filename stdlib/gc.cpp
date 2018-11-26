extern "C" int printf(const char*, ...);
extern "C" void* malloc(int size);

extern "C" void* gc_malloc(int size) {
  printf("hello %s\n", "gc_malloc");
  return malloc(size);
}