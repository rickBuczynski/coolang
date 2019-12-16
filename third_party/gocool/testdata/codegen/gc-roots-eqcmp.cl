class A {
 foo:Int;
};

class BothAsShouldDieNow {
 foo:Int;
};



class Main inherits IO {
  main() : Object { {
    if new A = new A then out_string("impossible\n") else new BothAsShouldDieNow fi;
  } };
};
