-- test roots in supertypes

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

class B inherits A {
 bar:Int;
};

class C  {
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

class CandBShouldLive {
 foo:Int;
};

class CandBShouldDieNow {
 foo:Int;
};

class Main inherits IO {
  main() : Object { {
    let
      b:B <- new B,
    in {
      let 
	    c:C <- new C,
		  in {
			c.setA(b);
			b.setC1(c);
		  };
	  new CandBShouldLive;
    };
	new CandBShouldDieNow;
  } };
};
