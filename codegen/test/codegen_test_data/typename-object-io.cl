-- Object.type_name returns a string representation of the dynamic
-- type of the dispatching object.


class Main inherits IO
{
  main() : Object
  {{
    out_string((new Object).type_name());
    out_string((new IO).type_name());
  }};
};
