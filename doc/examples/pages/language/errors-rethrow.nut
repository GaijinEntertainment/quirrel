// there is no 'finally': cleanup that must always run goes right here,
// then a bare 'throw e' rethrows the same value onward
function deploySquad() {
  println("releasing staging area lock")
  try {
    throw "landing zone is hot"
  } catch (e) {
    println("logged:", e)
    throw e
  }
}

try {
  deploySquad()
} catch (e) {
  println("mission control caught:", e)
}
