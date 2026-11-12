// `async` is not allowed on a metamethod name inside a table literal either:
// a table has no metamethods, but the parser has one path for a table and
// a class.
let delegate = {
  async function _add(other) {
    return other
  }
}
print(delegate)
