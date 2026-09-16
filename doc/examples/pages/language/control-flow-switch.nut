#allow-switch-statement

function describeHitZone(zone) {
  switch (zone) {
    case "head":
    case "neck":
      return "critical"
    case "chest":
      return "major"
    default:
      return "minor"
  }
}

println("describeHitZone(\"head\") =", describeHitZone("head"))
println("describeHitZone(\"chest\") =", describeHitZone("chest"))
println("describeHitZone(\"leg\") =", describeHitZone("leg"))
