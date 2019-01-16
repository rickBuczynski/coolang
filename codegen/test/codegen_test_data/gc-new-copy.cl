class A {
 foo:Int;
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
  main() : Object { {
	(new A).copy();
  } };
};
