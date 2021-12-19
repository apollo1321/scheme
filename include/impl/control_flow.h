#pragma once

#include <memory>

class Object;
class Cell;

struct Scope;

std::shared_ptr<Object> IfExpression(std::shared_ptr<Cell> if_list, std::shared_ptr<Scope> scope);
