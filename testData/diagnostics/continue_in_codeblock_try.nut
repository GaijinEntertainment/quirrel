// EXPECT_ERROR: "'continue' has to be in a loop block"
#allow-compiler-internals
for (local i = 0; i < 3; i++) {
  $${
    try {
      continue
    } catch (e) {
    }
    return null
  }
}
