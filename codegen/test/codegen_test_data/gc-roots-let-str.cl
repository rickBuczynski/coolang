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

class E {
 foo:Int;
};

class Main inherits IO {
  main() : Object { {
    new A;
    new B;
	new String;

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
			out_int(i);
			out_string("\n");
			a.setC1(c);
		  };
	  out_string("D should die now\n");
	  new E;
    };
	out_string("A, B, C, E should die now\n");
	new E;
  } };
};
