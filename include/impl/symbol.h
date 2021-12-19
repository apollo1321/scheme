#pragma once

#include <memory>

class Cell;

struct Scope;

std::shared_ptr<Object> IsSymbol(std::shared_ptr<Cell> set_list, std::shared_ptr<Scope> scope);

void SetVariable(std::shared_ptr<Cell> set_list, std::shared_ptr<Scope> scope);

void DefineObject(std::shared_ptr<Cell> define_list, std::shared_ptr<Scope> scope);
