from "deep" import is_relaxed_equal, is_strict_equal
from "math" import sqrt

println(is_relaxed_equal([1, 0.0], [1.0, -0.0]), is_strict_equal([1, 0.0], [1.0, -0.0]))
println(is_strict_equal({ id = 1, at = [0.5, 2] }, { id = 1, at = [0.5, 2] }))

let nan = sqrt(-1.0)
println(nan == nan, is_strict_equal([nan], [nan]))
