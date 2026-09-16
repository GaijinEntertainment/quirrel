---
see_also: [error]
---

Throws an error when `condition` is false.

## Parameters

- `condition` - checked with the same rules as `if`
- `message` - text for the error, or a function that returns it

## Return value

`null` when `condition` is true.

## Errors

Throws `assertion failed` when `condition` is false and no `message` is given.
Otherwise throws `message` itself, or the string that a function `message`
returns.

## Notes

`message` is read lazily: when it is a function, the function runs only after
`condition` has already failed, and only then. A condition that holds never
calls it, so building the message can be as expensive as it needs to be.

## Example

{{example:assert}}
