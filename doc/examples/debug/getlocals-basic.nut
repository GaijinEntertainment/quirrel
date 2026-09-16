from "debug" import getlocals

function aimTurret() {
  let turretAngle = 45
  let frame = getlocals()
  println("\"turretAngle\" in getlocals() =", "turretAngle" in frame, "value =", frame.turretAngle)
}
aimTurret()
