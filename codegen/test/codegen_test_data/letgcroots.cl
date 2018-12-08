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

class Main inherits IO {
  main() : Object { {
    new A;
    new B;

    let
      a:A <- new A,
      b:B <- new B,
    in {
      let c:C <- new C in {
	    c.setA(a);
	    a.setC1(c);
	  };
    };
  } };
};
