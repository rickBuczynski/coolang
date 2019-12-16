-- reads a line with no \n at the end, should stop at EOF character

class Main inherits IO {
  main() : Object {
    let line : String <- in_string() in {
      out_string(line);
    }
  };
};
