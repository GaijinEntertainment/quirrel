## Call these with .$

An instance's members and its built-in methods share one namespace, and a member wins. An
instance that declares a field called `rawget`, `tostring` or `weakref` hides the method of
that name, and the ordinary call then fails with `attempt to call 'string'` or whatever
the field happens to hold.

Writing the call as `obj.$rawget(key)` reaches the built-in type method directly and
never looks at the members, so the class declaration cannot break it.
Plain `obj.rawget(key)` is fine for an instance of a class you wrote yourself.

See [the .$ operator](page:language/operators#the-type-method-operator) and
[types.Table](page:language/containers).
