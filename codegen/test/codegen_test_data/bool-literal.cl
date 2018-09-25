-- use a previously bound let value in the next binding

class Main inherits IO {
  main():Object {
    let
      t1:Bool <- true,
      t2:Bool <- true,
      f1:Bool <- false,
    in {
      if (t1) then out_string("true") else out_string("false") fi;
      if (t2) then out_string("true") else out_string("false") fi;
      if (f1) then out_string("true") else out_string("false") fi;
    }
  };
};
