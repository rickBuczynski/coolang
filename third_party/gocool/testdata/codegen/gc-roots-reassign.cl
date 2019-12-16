class A {
 foo:Int;
};

class B inherits A {
 bar:Int;
};

class AshouldDieNow {
 foo:Int;
};

class BshouldDieNow {
 foo:Int;
};

class Main inherits IO {
  main() : Object { {
    let
      a:A <- new A,
    in {
      a <- new B;
	  new AshouldDieNow;
    };
	new BshouldDieNow;
  } };
};
