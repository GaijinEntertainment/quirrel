---
see_also: [string.format]
---

Formats `fmt` with the following arguments and writes the result to the same
output as `print`, without building and returning a throwaway string.

## Parameters

- `fmt` - the format string
- `...` - one value per conversion in `fmt`

## Return value

`null`, always; the formatted text goes to the print output, not to the
caller.

## Errors

The same as `format`, since `printf` builds the string the same way before
writing it.

## Example

{{example:string.printf}}
