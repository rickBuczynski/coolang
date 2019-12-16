class Main inherits IO {
  main():Object {
    let
      t:Bool <- true,
      f:Bool <- false
    in {
      if (not not t) then out_string("not not true is true\n") else 0 fi;
      if (not f) then out_string("not false is true\n") else 0 fi;
    }
  };
};

