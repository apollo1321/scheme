#pragma once

#include <memory>

class Object;

struct Scope;

std::shared_ptr<Object> DefineLambda(std::shared_ptr<Cell> list, std::shared_ptr<Scope> scope);
