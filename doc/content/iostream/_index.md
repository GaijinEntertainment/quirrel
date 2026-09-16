## Streams and the cursor

A **stream** is a cursor over a run of bytes. `readn` and `writen` act at the
cursor and move it forward by as many bytes as they touched, `tell` reports
where it is, and `seek` puts it somewhere else. Nothing here reads or writes at
an address; every operation is relative to where the cursor is.

Two things are streams, and they share the same method set, listed on the
[stream](sym:iostream.stream) page: a [blob](sym:iostream.blob), whose bytes are in
memory, and a [file](sym:io.file) from the [io](sym:io) module. Code written against
the stream methods works with either, which is the usual reason to parse a
downloaded buffer and a file on disk with the same function.

## What a blob is

A blob is a resizable block of memory with a cursor on it. `blob(n)` gives `n`
zero bytes with the cursor at the start; `blob(0)` is empty.

Length and cursor are independent, and mixing them up is the common bug:

- Writing past the end grows the blob, so a blob is an output buffer that never
  needs sizing up front.
- [resize](sym:iostream.blob.resize) changes the length and leaves the cursor where
  it was. After growing a blob you are not at the new end; `seek(0, 'e')` goes
  there.
- [len](sym:iostream.blob.len) is the size, not the amount written. A `blob(1024)`
  that has taken four bytes still reports 1024.

## Indexing bytes directly

A blob is also indexable, which the stream methods are not. `b[i]` reads the byte
at position `i` as an integer from 0 to 255, and `b[i] = v` writes one. Neither
moves the cursor, so byte access and stream access can be mixed freely.

```nut
let b = blob(3)
b[0] = 72; b[1] = 105; b[2] = 33
b.as_string()   // "Hi!", and the cursor is still at 0
```

`foreach` over a blob yields the position and the byte, in order.

There are two edge cases. An index outside the blob throws
`Error in '_get' metamethod: index out of range`, since indexing is implemented as a
[metamethod](page:language/metamethods). A value outside 0 to 255 does not
throw: only the low byte is stored, so `b[0] = 300` stores 44. Mask
or range-check the value yourself when it comes from arithmetic.

## Text and bytes

[as_string](sym:iostream.blob.as_string) is the conversion to text: it returns the
bytes as a string. [tostring](sym:iostream.blob.tostring) is the ordinary object
printer and gives an address, so it does not show the contents in a log.
