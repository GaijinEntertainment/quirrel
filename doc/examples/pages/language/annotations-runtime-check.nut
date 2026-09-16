function reload(ammoLeft: int) {
  return ammoLeft - 1
}
try { reload("full") } catch (e) { println(e) }

function unpack(data) {
  let { weaponName: string, ammoLeft: int } = data
  return ammoLeft
}
let badLoadout = { weaponName = "mg42", ammoLeft = "full" }
try { unpack(badLoadout) } catch (e) { println(e) }
