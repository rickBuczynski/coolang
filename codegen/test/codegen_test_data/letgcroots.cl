class A {
 c1:C;
 foo:Int;
 c2:C;
};

class B {
 foo:Int;
};

class C {
 a:A;
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
