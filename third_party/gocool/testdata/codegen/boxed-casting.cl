class Main inherits IO
{

  i:Object <- 3;
  b:Object <- true;
  s:Object <- "castedstr";
  
  main() : Object
  {
    {
      case i of
        casted:Int => out_int(casted);
        badcast:Object => abort();
      esac;
      
      
      case s of
        casted:String => out_string(casted);
        badcast:Object => abort();
      esac;
      
      case b of
        casted:Bool => if casted then out_string("true") else out_string("false") fi;
        badcast:Object => abort();
      esac;
    }
  };
};
