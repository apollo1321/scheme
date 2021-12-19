#include <common.h>
#include <expression.h>
#include <object.h>
#include <scheme_error.h>
#include <scope.h>
#include <symbol.h>

std::shared_ptr<Object> IsSymbol(std::shared_ptr<Cell> set_list, std::shared_ptr<Scope> scope) {
  if (!BeginsWith(set_list, "symbol?")) {
    throw InterpreterInternalError("IsSymbol: bad input");
  }
  auto arguments = EvaluateExpressions(ListToVector(set_list->GetSecond()), scope);
  if (arguments.size() != 1) {
    RuntimeError("symbol? requires 1 argument");
  }
  return std::make_shared<Bool>(Is<Symbol>(arguments[0]));
}

void SetVariable(std::shared_ptr<Cell> set_list, std::shared_ptr<Scope> scope) {
  if (!BeginsWith(set_list, "set!")) {
    throw InterpreterInternalError("SetVariable: bad input");
  }
  auto arguments = ListToVector(set_list->GetSecond());
  if (arguments.size() != 2) {
    throw SyntaxError("Wrong number of arguments in set! statement");
  }
  if (!Is<Symbol>(arguments[0])) {
    throw SyntaxError("Invalid set! syntax");
  }
  auto expression_result = EvaluateExpression(arguments[1], scope);
  if (!TrySetInScopes(As<Symbol>(arguments[0])->GetName(), expression_result, scope)) {
    throw UndefinedVariableError("The variable is not defined");
  }
}

void DefineObject(std::shared_ptr<Cell> define_list, std::shared_ptr<Scope> scope) {
  if (!BeginsWith(define_list, "define")) {
    throw InterpreterInternalError("DefineObject: bad input data");
  }
  if (!Is<Cell>(define_list->GetSecond())) {
    throw SyntaxError("Bad define format");
  }
  auto second_cell = As<Cell>(define_list->GetSecond());
  if (Is<Cell>(second_cell->GetFirst())) {
    auto header_cell = As<Cell>(second_cell->GetFirst());
    if (!Is<Symbol>(header_cell->GetFirst())) {
      throw SyntaxError("Bad procedure name");
    }
    auto procedure_name = As<Symbol>(header_cell->GetFirst())->GetName();
    auto arguments = ListToVector(header_cell->GetSecond());
    auto body = ListToVector(second_cell->GetSecond());
    scope->variables[procedure_name] = std::make_shared<Procedure>(scope, arguments, body);
    return;
  }
  if (!Is<Symbol>(second_cell->GetFirst())) {
    throw SyntaxError("Invalid name");
  }
  auto rest = ListToVector(second_cell->GetSecond());
  if (rest.size() != 1) {
    throw SyntaxError("Wrong arguments count");
  }
  auto variable_name = As<Symbol>(second_cell->GetFirst())->GetName();
  auto expression_result = EvaluateExpression(rest[0], scope);
  scope->variables[variable_name] = expression_result;
}
