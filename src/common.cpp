#include <common.h>
#include <error.h>
#include <object.h>

std::vector<std::shared_ptr<Object>> ListToVector(std::shared_ptr<Object> object) {
  std::vector<std::shared_ptr<Object>> result;
  while (Is<Cell>(object)) {
    auto cell = As<Cell>(object);
    result.push_back(cell->GetFirst());
    object = cell->GetSecond();
  }
  if (object) {
    result.push_back(object);
  }
  return result;
}

std::shared_ptr<Object> TryFindInScopes(const std::string& name, std::shared_ptr<Scope> scope) {
  if (scope == nullptr) {
    return nullptr;
  }
  if (scope->variables.count(name) > 0) {
    return scope->variables[name];
  }
  return TryFindInScopes(name, scope->outer_scope);
}

bool TrySetInScopes(const std::string& name, std::shared_ptr<Object> object,
                    std::shared_ptr<Scope> scope) {
  if (scope == nullptr) {
    return false;
  }
  if (scope->variables.count(name) > 0) {
    scope->variables[name] = object;
    return true;
  }
  return TrySetInScopes(name, object, scope->outer_scope);
}

bool BeginsWith(std::shared_ptr<Cell> cell, const std::string& key_word) {
  if (!Is<Symbol>(cell->GetFirst()) || As<Symbol>(cell->GetFirst())->GetName() != key_word) {
    return false;
  }
  return true;
}
