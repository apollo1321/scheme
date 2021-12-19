#pragma once

#include <memory>

class Object;
class Cell;

struct Scope;

std::shared_ptr<Object> IsNumber(std::shared_ptr<Cell> list, std::shared_ptr<Scope> scope);

std::vector<int64_t> GetIntegers(std::shared_ptr<Cell> list, std::shared_ptr<Scope> scope);

template <class BinaryPredicate>
bool BinaryOperator(const std::vector<int64_t>& values, BinaryPredicate predicate) {
    if (values.empty()) {
        return true;
    }
    for (size_t index = 1; index < values.size(); ++index) {
        if (!predicate(values[index - 1], values[index])) {
            return false;
        }
    }
    return true;
}
