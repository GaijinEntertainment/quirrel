//expect:w205

let function y() { //-declared-never-used
  return
  let t = {
    a = 1
  }
}
