## Call these with .$

A table's slots and its methods share one namespace, and a slot wins. A table with a
key called `len`, `rawget` or `keys` hides the method of that name, and the ordinary
call then fails with `attempt to call 'string'` or whatever the slot happens to hold.
Nothing warns, because a table with a key called `len` is legal.

Writing the call as `t.$len()` reaches the built-in type method directly and never
looks at the slots, so it cannot be broken by the data:

```nut
let squad = { len = "a field, not a method" }
squad.len()     // throws: attempt to call 'string'
squad.$len()    // 1
```

Plain `t.len()` is fine for a table whose keys you wrote yourself. For anything parsed
from a file, received over a network, or handed in by a caller, prefer `.$`. The same
applies to [class](page:language/classes) and instance members, which share their
namespace the same way. See [the .$ operator](page:language/operators#the-type-method-operator).
