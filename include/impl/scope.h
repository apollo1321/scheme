#pragma once

#include <map>
#include <memory>
#include <string>

class Object;

struct Scope {
  std::shared_ptr<Scope> outer_scope;
  std::map<std::string, std::shared_ptr<Object>> variables;
};
