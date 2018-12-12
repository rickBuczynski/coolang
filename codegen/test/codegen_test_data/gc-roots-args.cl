class A {
 foo:Int;
 --b:B <- new B; -- this messes up the type name when print As allocation since its allocated after A but before As allocation is printed
};

class B {
 foo:Int;
};

class C {
 foo:Int;
};

class D {
 foo:Int;
};

class Main inherits IO {
  arg_roots(a: A, c: C) : Object {{
    out_string("A and C still alive\n\n");
  }};

  main() : Object { {
    arg_roots(new A, new C);
	out_string("A and C should die now\n\n");
	new Object;
  } };
};
