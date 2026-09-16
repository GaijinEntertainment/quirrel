---
title: Standard library
group: C API
order: 138
summary: Registering the standard modules from a host.
---

Registering the standard modules, and the C entry points they expose to a host.

No part of the standard library is loaded automatically. A host registers the
modules it wants a script to reach. This is how a sandbox is built: leave out
`io` and `system`, and no script can open a file or run a command.

Each `sqstd_register_*lib` expects the target table on the stack. The other
functions here let C code use what those modules implement: read a blob's
bytes, open a file as a stream, run a regular expression.

## Registration and error reporting

Most hosts want `sqstd_seterrorhandlers` early. Without it, an unhandled error
leaves no trace.

{{capi:stdaux}}

## Blobs

{{capi:stdblob}}

## Files and streams

{{capi:stdio}}

## Strings and regular expressions

The `sqstd_rex_*` family is the regular expression engine behind
[string.regexp](sym:string.regexp), usable directly from C.

{{capi:stdstring}}

## The remaining modules

{{capi:stdmath,stddatetime,stddebug,stdsystem}}

## Extras

Object-handle shortcuts that skip the stack, for reads a host does often.

{{capi:ext}}
