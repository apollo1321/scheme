#include <common.h>
#include <lambda.h>
#include <object.h>
#include <scheme_error.h>
#include <scope.h>

std::shared_ptr<Object> DefineLambda(std::shared_ptr<Cell> list, std::shared_ptr<Scope> scope) {
  if (!BeginsWith(list, "lambda")) {
    throw InterpreterInternalError("DefineLambda: bad input");
  }
  auto objects = ListToVector(list->GetSecond());
  if (objects.empty() || objects.size() < 2) {
    throw SyntaxError("Invalid lambda syntax");
  }
  decltype(objects) arguments;
  if (Is<Symbol>(objects[0])) {
    arguments.emplace_back(objects[0]);
  } else if (Is<Cell>(objects[0])) {
    arguments = ListToVector(objects[0]);
  } else if (objects[0] != nullptr) {
    throw SyntaxError("Invalid lambda syntax");
  }
  auto body = ListToVector(As<Cell>(list->GetSecond())->GetSecond());
  return std::make_shared<Procedure>(scope, arguments, body);
}
