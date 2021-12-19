#include <common.h>
#include <control_flow.h>
#include <expression.h>
#include <object.h>
#include <scheme_error.h>

std::shared_ptr<Object> IfExpression(std::shared_ptr<Cell> if_list, std::shared_ptr<Scope> scope) {
  if (!BeginsWith(if_list, "if")) {
    throw InterpreterInternalError("IfExpression: bad input");
  }
  auto arguments = ListToVector(if_list->GetSecond());
  if (arguments.size() < 2 || arguments.size() > 3) {
    throw SyntaxError("Invalid argument count in if statement");
  }
  auto object_result = EvaluateExpression(arguments[0], scope);
  if (!Is<Bool>(object_result)) {
    return EvaluateExpression(arguments[1], scope);
  }
  auto result = As<Bool>(object_result);
  if (result->IsTrue()) {
    return EvaluateExpression(arguments[1], scope);
  }
  if (arguments.size() == 3) {
    return EvaluateExpression(arguments[2], scope);
  } else {
    return nullptr;
  }
}
