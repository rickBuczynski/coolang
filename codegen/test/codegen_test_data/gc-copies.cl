-- Test FC for of Object.copy

class DontKillB
{
  i : Int <- 2;
};

class A
{
  i : Int <- 2;
};

class B inherits A
{
  j : Int <- 2;
};

class Foo
{
  i : Int <- 2;
  a : A <- new A;

  setA(newa : A) : Object
  { 
    {
      a <- newa;
    }
  };
};


class Main 
{
  main() : Object
  {
    let foo : Foo <- (new Foo), foocp1 : Foo in {
      foocp1 <- foo.copy();
      foocp1.setA(new B);
	  
      -- B should not die here
	  new DontKillB;
    }
  };
};
