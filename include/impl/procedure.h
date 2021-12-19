#pragma once

#include <memory>
#include <vector>

class Object;
class Cell;

struct Scope;

std::shared_ptr<Object> ExecuteProcedure(std::shared_ptr<Cell> procedure_call,
                                         std::shared_ptr<Scope> scope);
