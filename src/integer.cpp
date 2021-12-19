#include <common.h>
#include <expression.h>
#include <integer.h>
#include <object.h>
#include <scheme_error.h>

std::shared_ptr<Object> IsNumber(std::shared_ptr<Cell> list, std::shared_ptr<Scope> scope) {
  if (!BeginsWith(list, "number?")) {
    throw InterpreterInternalError("IsNumber: bad input");
  }
  auto arguments = ListToVector(list->GetSecond());
  if (arguments.size() != 1) {
    throw SyntaxError("Wrong number of arguments in number? statement");
  }
  auto expression_result = EvaluateExpression(arguments[0], scope);
  return std::make_shared<Bool>(Is<Number>(expression_result));
}

std::vector<int64_t> GetIntegers(std::shared_ptr<Cell> list, std::shared_ptr<Scope> scope) {
  auto arguments = EvaluateExpressions(ListToVector(list->GetSecond()), scope);
  std::vector<int64_t> result;
  for (auto argument : arguments) {
    if (!Is<Number>(argument)) {
      throw RuntimeError("Integer required");
    }
    result.push_back(As<Number>(argument)->GetValue());
  }
  return result;
}
