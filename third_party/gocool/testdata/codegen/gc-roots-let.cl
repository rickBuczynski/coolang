class A {
 c1:C;
 foo:Int;
 c2:C;
 
 setC1( newC1 : C ) : Object
  {
    {
      c1 <- newC1;
    }
  };
};

class B {
 foo:Int;
};

class C {
 a:A;
 setA( newA : A ) : Object
  {
    {
      a <- newA;
    }
  };
};

class D {
 foo:Int;
};

class DshouldDieNow {
 foo:Int;
};

class ABCshouldDieNow {
 foo:Int;
};

class Main inherits IO {
  main() : Object { {
    new A;
    new B;

    let
      a:A <- new A,
	  i:Int <- 7, -- make sure this doesnt add a gc root
      b:B <- new B,
    in {
      let 
	    c:C <- new C,
		d:D <- new D,
		  in {
			c.setA(a);
			a.setC1(c);
		  };
	  new DshouldDieNow;
    };
	new ABCshouldDieNow;
  } };
};
