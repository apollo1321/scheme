#include <functional>
#include <numeric>

#include <boolean.h>
#include <common.h>
#include <control_flow.h>
#include <expression.h>
#include <integer.h>
#include <lambda.h>
#include <list.h>
#include <object.h>
#include <procedure.h>
#include <quote.h>
#include <scheme_error.h>
#include <symbol.h>

// TODO: Remove the implementation of operators from the function
std::shared_ptr<Object> EvaluateExpression(std::shared_ptr<Object> expression,
                                           std::shared_ptr<Scope> scope) {
  if (Is<Number>(expression) || Is<Bool>(expression)) {
    return expression;
  }
  if (Is<Symbol>(expression)) {
    auto name = As<Symbol>(expression)->GetName();
    auto object = TryFindInScopes(name, scope);
    if (!object) {
      throw UndefinedVariableError("Name " + name + " is not defined");
    }
    return object;
  }
  if (Is<Cell>(expression)) {
    auto cell = As<Cell>(expression);
    if (BeginsWith(cell, "boolean?")) {
      return IsBoolean(cell, scope);
    }
    if (BeginsWith(cell, "not")) {
      return NotFunction(cell, scope);
    }
    if (BeginsWith(cell, "and")) {
      return AndFunction(cell, scope);
    }
    if (BeginsWith(cell, "or")) {
      return OrFunction(cell, scope);
    }
    if (BeginsWith(cell, "if")) {
      return IfExpression(cell, scope);
    }
    if (BeginsWith(cell, "number?")) {
      return IsNumber(cell, scope);
    }
    if (BeginsWith(cell, "=")) {
      bool result = BinaryOperator(GetIntegers(cell, scope), std::equal_to());
      return std::make_shared<Bool>(result);
    }
    if (BeginsWith(cell, ">")) {
      bool result = BinaryOperator(GetIntegers(cell, scope), std::greater());
      return std::make_shared<Bool>(result);
    }
    if (BeginsWith(cell, ">=")) {
      bool result = BinaryOperator(GetIntegers(cell, scope), std::greater_equal());
      return std::make_shared<Bool>(result);
    }
    if (BeginsWith(cell, "<")) {
      bool result = BinaryOperator(GetIntegers(cell, scope), std::less());
      return std::make_shared<Bool>(result);
    }
    if (BeginsWith(cell, "<=")) {
      bool result = BinaryOperator(GetIntegers(cell, scope), std::less_equal());
      return std::make_shared<Bool>(result);
    }
    if (BeginsWith(cell, "+")) {
      auto numbers = GetIntegers(cell, scope);
      int64_t result = std::accumulate(numbers.begin(), numbers.end(), 0ll, std::plus());
      return std::make_shared<Number>(result);
    }
    if (BeginsWith(cell, "-")) {
      auto numbers = GetIntegers(cell, scope);
      if (numbers.empty()) {
        throw RuntimeError("- requires at least one argument");
      }
      int64_t result =
          std::accumulate(std::next(numbers.begin()), numbers.end(), numbers[0], std::minus());
      return std::make_shared<Number>(result);
    }
    if (BeginsWith(cell, "*")) {
      auto numbers = GetIntegers(cell, scope);
      int64_t result = std::accumulate(numbers.begin(), numbers.end(), 1ll, std::multiplies());
      return std::make_shared<Number>(result);
    }
    if (BeginsWith(cell, "/")) {
      auto numbers = GetIntegers(cell, scope);
      if (numbers.empty()) {
        throw RuntimeError("/ requires at least one argument");
      }
      int64_t result =
          std::accumulate(std::next(numbers.begin()), numbers.end(), numbers[0], std::divides());
      return std::make_shared<Number>(result);
    }
    if (BeginsWith(cell, "max")) {
      auto numbers = GetIntegers(cell, scope);
      if (numbers.empty()) {
        throw RuntimeError("max requires at least one argument");
      }
      int64_t result = *std::max_element(numbers.begin(), numbers.end());
      return std::make_shared<Number>(result);
    }
    if (BeginsWith(cell, "min")) {
      auto numbers = GetIntegers(cell, scope);
      if (numbers.empty()) {
        throw RuntimeError("max requires at least one argument");
      }
      int64_t result = *std::min_element(numbers.begin(), numbers.end());
      return std::make_shared<Number>(result);
    }
    if (BeginsWith(cell, "abs")) {
      auto numbers = GetIntegers(cell, scope);
      if (numbers.size() != 1) {
        throw RuntimeError("abs requires one element");
      }
      return std::make_shared<Number>(std::abs(numbers[0]));
    }
    if (BeginsWith(cell, "lambda")) {
      return DefineLambda(cell, scope);
    }
    if (BeginsWith(cell, "quote")) {
      return QuoteDatum(cell);
    }
    if (BeginsWith(cell, "set!")) {
      SetVariable(cell, scope);
      return nullptr;
    }
    if (BeginsWith(cell, "pair?")) {
      return IsPair(cell, scope);
    }
    if (BeginsWith(cell, "null?")) {
      return IsNull(cell, scope);
    }
    if (BeginsWith(cell, "list?")) {
      return IsList(cell, scope);
    }
    if (BeginsWith(cell, "cons")) {
      return Cons(cell, scope);
    }
    if (BeginsWith(cell, "car")) {
      return Car(cell, scope);
    }
    if (BeginsWith(cell, "cdr")) {
      return Cdr(cell, scope);
    }
    if (BeginsWith(cell, "set-car!")) {
      SetCar(cell, scope);
      return nullptr;
    }
    if (BeginsWith(cell, "set-cdr!")) {
      SetCdr(cell, scope);
      return nullptr;
    }
    if (BeginsWith(cell, "list")) {
      return List(cell, scope);
    }
    if (BeginsWith(cell, "list-ref")) {
      return ListRef(cell, scope);
    }
    if (BeginsWith(cell, "list-tail")) {
      return ListTail(cell, scope);
    }
    if (BeginsWith(cell, "symbol?")) {
      return IsSymbol(cell, scope);
    }
    return ExecuteProcedure(cell, scope);
  }
  throw RuntimeError("Bad input");
}

std::vector<std::shared_ptr<Object>> EvaluateExpressions(
    std::vector<std::shared_ptr<Object>> expressions, std::shared_ptr<Scope> scope) {
  std::vector<std::shared_ptr<Object>> result;
  for (auto expression : expressions) {
    result.emplace_back(EvaluateExpression(expression, scope));
  }
  return result;
}
