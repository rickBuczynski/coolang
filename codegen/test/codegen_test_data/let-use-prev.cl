-- use a previously bound let value in the next binding

class Main inherits IO {
  main():Object {
    let
      one:Int <- 1,
      two:Int <- one + 1,
      three:Int <- two + 1,
    in {
      out_int(one);
      out_int(two);
      out_int(three);
    }
  };
};

