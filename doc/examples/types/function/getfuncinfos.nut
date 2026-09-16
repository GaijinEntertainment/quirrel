function add(a, b = 10) { return a + b }
let info = add.getfuncinfos()
println($"{info.name} required_params={info.required_params}")
println($"parameters={", ".join(info.parameters)}")
println($"varargs={info.varargs} native={info.native}")
