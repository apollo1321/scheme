#pragma once

#include <memory>

class Object;
class Cell;

struct Scope;

std::shared_ptr<Object> IsBoolean(std::shared_ptr<Cell> list, std::shared_ptr<Scope> scope);

std::shared_ptr<Object> NotFunction(std::shared_ptr<Cell> list, std::shared_ptr<Scope> scope);

std::shared_ptr<Object> AndFunction(std::shared_ptr<Cell> list, std::shared_ptr<Scope> scope);

std::shared_ptr<Object> OrFunction(std::shared_ptr<Cell> list, std::shared_ptr<Scope> scope);
