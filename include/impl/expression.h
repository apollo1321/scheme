#pragma once

#include <memory>
#include <vector>

class Object;

struct Scope;

std::shared_ptr<Object> EvaluateExpression(std::shared_ptr<Object> expression,
                                           std::shared_ptr<Scope> scope);

std::vector<std::shared_ptr<Object>> EvaluateExpressions(
    std::vector<std::shared_ptr<Object>> expressions, std::shared_ptr<Scope> scope);
