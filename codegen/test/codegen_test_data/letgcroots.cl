class A {
 foo:Int;
};

class B {
 foo:Int;
};

class C {
 foo:Int;
};

class Main inherits IO {
  main() : Object { {
    new A;
    new B;

    let
      a:A <- new A,
      b:B <- new B,
    in {
      let c:C <- new C in 1;
    };
  } };
};
