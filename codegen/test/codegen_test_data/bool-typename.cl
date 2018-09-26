-- use a previously bound let value in the next binding

class Main inherits IO {
  main():Object {
    let
      t1:Bool <- true,
      t2:Bool <- true,
    in {
      out_string(t1.type_name());
      out_string(t2.type_name());
    }
  };
};
