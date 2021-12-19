#include <boolean.h>
#include <common.h>
#include <expression.h>
#include <object.h>
#include <scheme_error.h>

std::shared_ptr<Object> IsBoolean(std::shared_ptr<Cell> list, std::shared_ptr<Scope> scope) {
  if (!BeginsWith(list, "boolean?")) {
    throw InterpreterInternalError("IsBoolean: bad input");
  }
  auto arguments = ListToVector(list->GetSecond());
  if (arguments.size() != 1) {
    throw RuntimeError("Wrong number of arguments in boolean? statement");
  }
  auto result = EvaluateExpression(arguments[0], scope);
  if (Is<Bool>(result)) {
    return std::make_shared<Bool>(true);
  }
  return std::make_shared<Bool>(false);
}

std::shared_ptr<Object> NotFunction(std::shared_ptr<Cell> list, std::shared_ptr<Scope> scope) {
  if (!BeginsWith(list, "not")) {
    throw InterpreterInternalError("NotFunction: bad input");
  }
  auto arguments = ListToVector(list->GetSecond());
  if (arguments.size() != 1) {
    throw RuntimeError("Wrong number of arguments in not statement");
  }
  auto result = EvaluateExpression(arguments[0], scope);
  if (Is<Bool>(result) && !As<Bool>(result)->IsTrue()) {
    return std::make_shared<Bool>(true);
  }
  return std::make_shared<Bool>(false);
}

std::shared_ptr<Object> AndFunction(std::shared_ptr<Cell> list, std::shared_ptr<Scope> scope) {
  if (!BeginsWith(list, "and")) {
    throw InterpreterInternalError("AndFunction: bad input");
  }
  auto arguments = ListToVector(list->GetSecond());
  for (size_t index = 0; index + 1 < arguments.size(); ++index) {
    auto result = EvaluateExpression(arguments[index], scope);
    if (Is<Bool>(result) && !As<Bool>(result)->IsTrue()) {
      return std::make_shared<Bool>(false);
    }
  }
  return arguments.empty() ? std::make_shared<Bool>(true)
                           : EvaluateExpression(arguments[arguments.size() - 1], scope);
}

std::shared_ptr<Object> OrFunction(std::shared_ptr<Cell> list, std::shared_ptr<Scope> scope) {
  if (!BeginsWith(list, "or")) {
    throw InterpreterInternalError("OrFunction: bad input");
  }
  auto arguments = ListToVector(list->GetSecond());
  for (size_t index = 0; index + 1 < arguments.size(); ++index) {
    auto result = EvaluateExpression(arguments[index], scope);
    if (Is<Bool>(result) && As<Bool>(result)->IsTrue()) {
      return std::make_shared<Bool>(true);
    }
  }
  return arguments.empty() ? std::make_shared<Bool>(false)
                           : EvaluateExpression(arguments[arguments.size() - 1], scope);
}
