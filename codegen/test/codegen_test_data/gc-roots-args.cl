class A {
 foo:Int;
 --b:B <- new B; -- TODO this messes up the type name when print As allocation since its allocated after A but before As allocation is printed
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

class AandCstillAlive {
 foo:Int;
};

class AandCshouldDieNow {
 foo:Int;
};

class Main inherits IO {
  arg_roots(a: A, c: C) : Object {{
    new AandCstillAlive;
  }};

  main() : Object { {
    arg_roots(new A, new C);
	new AandCshouldDieNow;
  } };
};
