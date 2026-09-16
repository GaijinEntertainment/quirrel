async function requestResupply(ammoCount) {
  if (ammoCount <= 0)
    throw "resupply request must be positive"
  return ammoCount * 2
}

async function commander() {
  try {
    let delivered = await requestResupply(-5)
    println("delivered:", delivered)
  } catch (e) {
    println("resupply denied:", e)
  }
  let delivered = await requestResupply(3)
  println("delivered:", delivered)
}

commander()
