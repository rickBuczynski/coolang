class A {
 foo:Int;
};

class Main inherits IO {
  main() : Object { {
    if new A = new A then out_string("impossible\n") else out_string("compare done\n") fi;
	out_string("both As should die now\n\n");
	new Object;
  } };
};
