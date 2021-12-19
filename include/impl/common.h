#pragma once

#include <memory>
#include <vector>

class Object;
class Cell;

struct Scope;

std::vector<std::shared_ptr<Object>> ListToVector(std::shared_ptr<Object> object);

std::shared_ptr<Object> TryFindInScopes(const std::string& name, std::shared_ptr<Scope> scope);

bool TrySetInScopes(const std::string& name, std::shared_ptr<Object> object,
                    std::shared_ptr<Scope> scope);

bool BeginsWith(std::shared_ptr<Cell> cell, const std::string& key_word);
