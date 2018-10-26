-- reads and outputs two lines from stdin
-- outputs the first line twice

class Main inherits IO {
  main() : Object {
    let line : String <- in_string() in {
      out_string(line);
      out_string(in_string());
      out_string(line);
    }
  };
};
