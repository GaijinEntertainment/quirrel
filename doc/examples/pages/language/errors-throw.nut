function reload(ammoLeft) {
  if (ammoLeft < 0)
    throw "negative ammo count"
  if (ammoLeft == 0)
    throw { code = "empty", ammoLeft = ammoLeft }
  return ammoLeft - 1
}

try {
  reload(-1)
} catch (e) {
  println($"{typeof e}: {e}")
}

try {
  reload(0)
} catch (e) {
  println($"{typeof e}: {e.code}")
}
