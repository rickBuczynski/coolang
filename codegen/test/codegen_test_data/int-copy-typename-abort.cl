class Main inherits IO
{

  x:Int <- 1;
  y:Int <- 3;
  
  main() : Object
  {
    {
      out_int(x);
      out_string(x.type_name());
      
      out_int(y);
      y <- x.copy();
      out_int(y);
      
      y.abort();
    }
  };
};
