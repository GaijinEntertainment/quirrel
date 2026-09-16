from "debug" import format_call_stack_string

function inner() {
  return format_call_stack_string()
}

println(inner())
