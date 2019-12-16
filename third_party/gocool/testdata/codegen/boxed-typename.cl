class Main inherits IO
{

  i:Object <- 1;
  b:Object <- true;
  s:Object <- "a string";
  
  main() : Object
  {
    {
      out_string(i.type_name());
      out_string(b.type_name());
      out_string(s.type_name());
    }
  };
};
