#pragma once

namespace SQCompilation
{

struct FunctionInfo {

  FunctionInfo(const FunctionDecl *d, const FunctionDecl *o) : declaration(d), owner(o) {}

  ~FunctionInfo() = default;

  struct Modifiable {
    const FunctionDecl *owner;
    const SQChar *name;
  };

  const FunctionDecl *owner;
  std::vector<Modifiable> modifiable;
  const FunctionDecl *declaration;
  std::vector<const SQChar *> parameters;

  void joinModifiable(const FunctionInfo *other);
  void addModifiable(const SQChar *name, const FunctionDecl *o);

};

}
