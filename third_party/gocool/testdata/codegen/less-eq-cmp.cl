class Main inherits IO
{
  main() : Object
  {
    {
      if (2 <= 10) then out_string("2 <= 10 is true\n") else 0 fi;
      if (2 <= 2) then out_string("2 <= 2 is true\n") else 0 fi;
      if (2 <= 1) then 0 else out_string("2 <= 1 is false\n") fi;
    }
  };
};
