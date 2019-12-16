class Main inherits IO {
  main():Object {
    let
      x:Int <- 0,
    in {
      while x < 5 loop {
        out_int(x);
        x <- x + 1;
      } pool;
    }
  };
};

