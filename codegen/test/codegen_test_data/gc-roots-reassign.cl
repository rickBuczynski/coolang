class A {
 foo:Int;
};

class B inherits A {
 bar:Int;
};

class Killer {
 foo:Int;
};

class Main inherits IO {
  main() : Object { {
    let
      a:A <- new A,
    in {
      a <- new B;
	  out_string("A should die now\n");
	  new Killer;
    };
	out_string("B should die now\n");
	new Killer;
  } };
};
