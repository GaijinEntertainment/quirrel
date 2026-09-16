let squad = ["healer", "tank"];
let backup = clone squad;
backup.append("mage");
println("squad =", ", ".join(squad));
println("backup =", ", ".join(backup));
