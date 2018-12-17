class Killer {
 foo:Int;
};

class Main inherits IO {
  main() : Object { {
	new String;
	out_int(1); -- no allocation yet

    let
      str:String <- "this is a string",
    in {
      out_int(2); -- now theres been an allocation
    };
	out_int(3); -- now String should die
	new Killer;
  } };
};
