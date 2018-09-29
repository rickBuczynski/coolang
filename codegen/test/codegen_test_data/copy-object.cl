-- Test of Object.copy

class Foo inherits IO
{
  i : Int <- 2;

  print() : Object
  { 
    {
      out_int(i);
      out_string( "\n" );
    }
  };
  
  addone() : Int
  { 
    {
      i <- i + 1;
    }
  };
};


class Main 
{
  main() : Object
  {
    let foo : Foo <- (new Foo), foocp1 : Foo, foocp2 : Foo  in {
      foo.print();
      
      foo.addone();
      foocp1 <- foo.copy();
      
      foo.addone();
      foocp2 <- foo.copy();
      
      foo.addone();
      
      foo.print();
      foocp1.print();
      foocp2.print();
    }
  };
};
