---
see_also: [freeze, getobjflags]
---

Walks `obj` and merges equal tables and arrays it finds inside.

## Parameters

- `obj` - table, array, or null to walk

## Return value

Always `null`, for any `obj`. The call does its work in place and returns
no result.

## Errors

None. `obj` outside `table`, `array` or `null` fails the signature's own type
check before the body runs.

## Notes

Every table and array reachable from `obj` - its values, and their values, and
so on - is marked immutable, the same flag `freeze` sets. Two of them get
merged into one shared instance when they are equal, hold at most 20 items,
and every value in them is `null`, `bool`, `int`, `float` or `string`; a
smaller memory footprint is the purpose of the call.

The mark lands on what `obj` holds, not on `obj` itself: `getobjflags(obj)`
right after the call is unchanged. Keep using the same variable; the call
does not return a frozen replacement.

## Example

{{example:deduplicate_object}}
