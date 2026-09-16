Returns the closure that is currently running.

## Return value

The closure of the function `callee()` was called from. This works for a
native closure too.

## Errors

Throws `no closure in the calls stack` when there is no enclosing closure.
This cannot happen from script code: even the top level of a
script runs inside a closure, so `callee()` there returns that closure rather
than throwing.

## Notes

Useful for a closure that calls itself without being bound to a name yet, such
as one still being built as a table value or a default argument.

## Example

{{example:callee}}
