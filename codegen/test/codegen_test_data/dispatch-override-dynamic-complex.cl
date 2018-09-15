-- Dynamic dispatch chooses a method based on the dynamic type of the
-- dispatching object.


class Base inherits IO
{
  base_and_derived() : Object
  {
    out_string( "base_and_derived=base\n" )
  };
  base_only() : Object
  {
    out_string( "base_only=base\n" )
  };
  base_and_derived_two() : Object
  {
    out_string( "base_and_derived_two=base\n" )
  };
};


class Derived inherits Base
{
  base_and_derived_two() : Object
  {
    out_string( "base_and_derived_two=derived\n" )
  };
  base_and_derived() : Object
  {
    out_string( "base_and_derived=derived\n" )
  };
  derived_only() : Object
  {
    out_string( "derived_only=derived\n" )
  };
};


class Main
{
  main() : Object
  {
    {
      let me : Base <- new Base in {
	    me.base_and_derived();
        me.base_only();
        me.base_and_derived_two();
      };
      
      let me : Base <- new Derived in {
	    me.base_and_derived();
        me.base_only();
        me.base_and_derived_two();
      };
      
      let me : Derived <- new Derived in {
	    me.base_and_derived();
        me.derived_only();
        me.base_and_derived_two();
      };
    }
  };
};
