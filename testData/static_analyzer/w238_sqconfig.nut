//expect:w238

function x() { //-declared-never-used
  ::a._must_be_utilized(::table2);
}

//-file:undefined-global