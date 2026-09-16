---
see_also: [string.strip, types.String.join]
---

Splits `str` into an array of strings at every character that occurs in
`separators`.

## Parameters

- `str` - the string to split
- `separators` - the set of characters that split `str`, each one its own separator; not a substring to match as a whole
- `skip_empty` - when true, a piece with zero characters is left out of the result instead of being added as `""`

## Return value

An array of the pieces of `str`, in the order they occur, with every
separator character removed.

## Errors

Throws `empty separators string` when `separators` is `""`.

## Notes

`separators` is a set, not a literal string: `split_by_chars("1-2/3", "-/")`
splits on `-` and on `/` separately, not on the two-character run `-/`.

A leading separator makes an empty first piece, unless `skip_empty` is true.
A trailing separator never makes an empty last piece, with `skip_empty` true
or false: the scan only emits a final piece when something follows the last
separator, so nothing is left to emit. Two separators in a row make an empty
piece between them, again unless `skip_empty` is true.

## Example

{{example:string.split_by_chars}}
