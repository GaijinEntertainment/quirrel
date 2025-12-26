#pragma once

namespace SQCompilation
{


enum ReturnTypeBits
{
  RT_NOTHING = 1 << 0,
  RT_NULL = 1 << 1,
  RT_BOOL = 1 << 2,
  RT_NUMBER = 1 << 3,
  RT_STRING = 1 << 4,
  RT_TABLE = 1 << 5,
  RT_ARRAY = 1 << 6,
  RT_CLOSURE = 1 << 7,
  RT_FUNCTION_CALL = 1 << 8,
  RT_UNRECOGNIZED = 1 << 9,
  RT_THROW = 1 << 10,
  RT_CLASS = 1 << 11,
};


static const SQChar *enumFqn(Arena *arena, const SQChar *enumName, const SQChar *cname) {
  int32_t l1 = strlen(enumName);
  int32_t l2 = strlen(cname);
  int32_t l = l1 + 1 + l2 + 1;
  SQChar *r = (SQChar *)arena->allocate(l);
  snprintf(r, l, "%s.%s", enumName, cname);
  return r;
}


static int32_t strhash(const SQChar *s) {
  int32_t r = 0;
  while (*s) {
    r *= 31;
    r += *s;
    ++s;
  }

  return r;
}

struct StringHasher {
  int32_t operator()(const SQChar *s) const {
    return strhash(s);
  }
};

struct StringEqualer {
  int32_t operator()(const SQChar *a, const SQChar *b) const {
    return strcmp(a, b) == 0;
  }
};

}
