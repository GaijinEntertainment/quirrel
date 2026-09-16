## Call these with .$

A instance's members and its built-in methods share one namespace, and a member wins. A
instance that declares a field called `rawget`, `tostring` or `weakref` hides the method of
that name, and the ordinary call then fails with `attempt to call 'string'` or whatever
the field happens to hold.

Writing the call as `obj.$rawget(key)` reaches the built-in type method directly and
never looks at the members, so it cannot be broken by however the instance was declared.
Plain `obj.rawget(key)` is fine for a instance you wrote yourself.

See [the .$ operator](page:language/operators#the-type-method-operator) and
[types.Table](page:language/containers).
