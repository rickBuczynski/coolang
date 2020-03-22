class A {
 foo:Int;
 b:B <- new B;
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
