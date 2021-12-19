#pragma once

#include <memory>

class Object;
class Cell;

struct Scope;

std::shared_ptr<Object> IsPair(std::shared_ptr<Cell> list, std::shared_ptr<Scope> scope);

std::shared_ptr<Object> IsNull(std::shared_ptr<Cell> list, std::shared_ptr<Scope> scope);

std::shared_ptr<Object> IsList(std::shared_ptr<Cell> list, std::shared_ptr<Scope> scope);

std::shared_ptr<Object> Cons(std::shared_ptr<Cell> list, std::shared_ptr<Scope> scope);

std::shared_ptr<Object> Car(std::shared_ptr<Cell> list, std::shared_ptr<Scope> scope);

std::shared_ptr<Object> Cdr(std::shared_ptr<Cell> list, std::shared_ptr<Scope> scope);

void SetCar(std::shared_ptr<Cell> list, std::shared_ptr<Scope> scope);

void SetCdr(std::shared_ptr<Cell> list, std::shared_ptr<Scope> scope);

std::shared_ptr<Object> List(std::shared_ptr<Cell> list, std::shared_ptr<Scope> scope);

std::shared_ptr<Object> ListRef(std::shared_ptr<Cell> list, std::shared_ptr<Scope> scope);

std::shared_ptr<Object> ListTail(std::shared_ptr<Cell> list, std::shared_ptr<Scope> scope);
