let team = {alpha = [1, 2], bravo = [1, 2]}   // two separate but equal arrays
deduplicate_object(team)

println("team.alpha == team.bravo =", team.alpha == team.bravo)  // merged into one
