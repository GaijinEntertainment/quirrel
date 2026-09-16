---
params: [index]
see_also: [types.Array.hasvalue, types.Array.len]
---

Reports whether `index` is a valid element index.

## Parameters

- `index` - the index to check

## Return value

`true` when `0 <= index < len()`, `false` otherwise, including for a
negative `index`.

## Notes

`hasindex` never throws, even for a far out-of-range `index`; indexing
the array directly does, with `the index '<value>' (type='integer') does
not exist`. Check with `hasindex` first when an out-of-range index is
expected rather than exceptional.

Unlike [slice](sym:types.Array.slice) and [swap](sym:types.Array.swap), a
negative `index` here is not counted back from the end; it is out of
range.

## Example

{{example:types.Array.hasindex}}
