#pragma once

#include <string>
#include <algorithm>
#include "compiler/compilationcontext.h"
#include "compiler/ast.h"
#include "config.h"
#include <sq_char_class.h>


namespace SQCompilation
{


static const char *terminatorOpToName(TreeOp op) {
  switch (op)
  {
  case TO_BREAK: return "break";
  case TO_CONTINUE: return "continue";
  case TO_RETURN: return "return";
  case TO_THROW: return "throw";
  default:
    assert(0);
    return "<unknown terminator>";
  }
}


static bool isValidId(const SQChar *id) {
  assert(id != nullptr);

  if (!sq_isalpha(id[0]) && id[0] != '_')
    return false;

  for (int i = 1; id[i]; ++i) {
    SQChar c = id[i];
    if (!sq_isalpha(c) && !sq_isdigit(c) && c != '_')
      return false;
  }

  return true;
}


static bool hasAnyPrefix(const SQChar *str, const std::vector<std::string> &prefixes) {
  for (auto &prefix : prefixes) {
    size_t length = prefix.length();
    bool hasPrefix = strncmp(str, prefix.c_str(), length)==0;
    if (hasPrefix) {
      SQChar c = str[length];
      if (!c || c == '_' || c != tolower(c)) {
        return true;
      }
    }
  }

  return false;
}

static bool hasAnyEqual(const SQChar *str, const std::vector<std::string> &candidates) {
  for (auto &candidate : candidates) {
    if (strcmp(str, candidate.c_str()) == 0) {
      return true;
    }
  }

  return false;
}

static bool hasAnySubstring(const SQChar *str, const std::vector<std::string> &candidates) {
  for (auto &candidate : candidates) {
    if (strstr(str, candidate.c_str())) {
      return true;
    }
  }

  return false;
}

static bool nameLooksLikeResultMustBeBoolean(const SQChar *funcName) {
  if (!funcName)
    return false;

  return hasAnyPrefix(funcName, function_should_return_bool_prefix);
}

static bool nameLooksLikeFunctionMustReturnResult(const SQChar *funcName) {
  if (!funcName)
    return false;

  bool nameInList = nameLooksLikeResultMustBeBoolean(funcName) ||
    hasAnyPrefix(funcName, function_should_return_something_prefix);

  if (!nameInList)
    if ((strstr(funcName, "_ctor") || strstr(funcName, "Ctor")) && strstr(funcName, "set") != funcName)
      nameInList = true;

  return nameInList;
}

static bool nameLooksLikeResultMustBeUtilized(const SQChar *name) {
  return hasAnyEqual(name, function_result_must_be_utilized);
}

static bool nameLooksLikeResultMustBeString(const SQChar *name) {
  return hasAnyEqual(name, function_can_return_string);
}

static bool nameLooksLikeCallsLambdaInPlace(const SQChar *name) {
  return hasAnyEqual(name, function_calls_lambda_inplace);
}

static bool canFunctionReturnNull(const SQChar *n) {
  return hasAnyEqual(n, function_can_return_null);
}

static bool isForbiddenFunctionName(const SQChar *n) {
  return hasAnyEqual(n, function_forbidden);
}

static bool nameLooksLikeMustBeCalledFromRoot(const SQChar *n) {
  return hasAnyEqual(n, function_must_be_called_from_root);
}

static bool nameLooksLikeForbiddenParentDir(const SQChar *n) {
  return hasAnyEqual(n, function_forbidden_parent_dir);
}

static bool nameLooksLikeFormatFunction(const SQChar *n) {
  std::string transformed(n);
  std::transform(transformed.begin(), transformed.end(), transformed.begin(), ::tolower);

  return hasAnySubstring(transformed.c_str(), format_function_name);
}

static bool nameLooksLikeModifiesObject(const SQChar *n) {
  return hasAnyEqual(n, function_modifies_object);
}

static bool nameLooksLikeFunctionTakeBooleanLambda(const SQChar *n) {
  return hasAnyEqual(n, function_takes_boolean_lambda);
}

bool looksLikeElementCount(const Expr *e);

bool isUpperCaseIdentifier(const Expr *e);

}
