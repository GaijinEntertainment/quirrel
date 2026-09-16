class Turret { active = false constructor() { this.active = true } }
let blueprint = Turret.instance()
println("blueprint.active =", blueprint.active)
