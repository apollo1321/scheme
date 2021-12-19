#include <common.h>
#include <expression.h>
#include <object.h>
#include <procedure.h>
#include <scheme_error.h>
#include <symbol.h>

std::vector<std::string> GetProcedureArgumentNames(std::shared_ptr<Procedure> procedure) {
  std::vector<std::string> arguments_names;
  for (auto argument : procedure->GetFormalArguments()) {
    if (!Is<Symbol>(argument)) {
      throw SyntaxError("procedure call: invalid procedure argument names");
    }
    arguments_names.emplace_back(As<Symbol>(argument)->GetName());
  }
  return arguments_names;
}

void DefineArgumentsInLocalScope(std::vector<std::string> names,
                                 std::vector<std::shared_ptr<Object>> argument_values,
                                 std::shared_ptr<Scope> local_scope) {
  if (argument_values.size() != names.size()) {
    throw SyntaxError("Wrong number of arguments");
  }
  for (size_t argument_index = 0; argument_index < names.size(); ++argument_index) {
    local_scope->variables[names[argument_index]] = argument_values[argument_index];
  }
}

void AddProcedureBodyDefinitions(std::shared_ptr<Procedure> procedure,
                                 std::shared_ptr<Scope> local_scope) {
  for (auto object : procedure->GetBody()) {
    if (!Is<Cell>(object)) {
      break;
    }
    if (!BeginsWith(As<Cell>(object), "define")) {
      break;
    }
    DefineObject(As<Cell>(object), local_scope);
  }
}

std::shared_ptr<Scope> DefineProcedureScope(std::shared_ptr<Procedure> procedure,
                                            std::vector<std::shared_ptr<Object>> argument_values) {
  auto local_scope = std::make_shared<Scope>();
  local_scope->outer_scope = procedure->GetScope();
  DefineArgumentsInLocalScope(GetProcedureArgumentNames(procedure), argument_values, local_scope);
  AddProcedureBodyDefinitions(procedure, local_scope);
  return local_scope;
}

std::shared_ptr<Object> EvaluateProcedureExpressions(std::shared_ptr<Procedure> procedure,
                                                     std::shared_ptr<Scope> scope) {
  auto body = procedure->GetBody();
  size_t expression_index = 0;
  for (size_t index = 0; index < body.size(); ++index) {
    if (!Is<Cell>(body[index]) || !BeginsWith(As<Cell>(body[index]), "define")) {
      break;
    }
    ++expression_index;
  }
  if (expression_index == body.size()) {
    throw SyntaxError("No expressions in procedure");
  }
  for (size_t index = expression_index; index + 1 < body.size(); ++index) {
    EvaluateExpression(body[index], scope);
  }
  return EvaluateExpression(body[body.size() - 1], scope);
}

std::shared_ptr<Object> ExecuteProcedure(std::shared_ptr<Cell> procedure_call,
                                         std::shared_ptr<Scope> scope) {
  auto found_object = EvaluateExpression(procedure_call->GetFirst(), scope);
  if (!Is<Procedure>(found_object)) {
    if (Is<Symbol>(procedure_call->GetFirst())) {
      throw RuntimeError("Variable " + As<Symbol>(procedure_call->GetFirst())->GetName() +
                         " is not procedure");
    } else {
      throw RuntimeError("Not valid procedure name");
    }
  }
  auto procedure = As<Procedure>(found_object);
  std::shared_ptr<Cell> actual_arguments;
  if (procedure_call->GetSecond()) {
    if (!Is<Cell>(procedure_call->GetSecond())) {
      throw SyntaxError("Bad procedure call");
    }
    actual_arguments = As<Cell>(procedure_call->GetSecond());
  }
  auto argument_values = EvaluateExpressions(ListToVector(actual_arguments), scope);
  auto local_scope = DefineProcedureScope(procedure, argument_values);
  return EvaluateProcedureExpressions(procedure, local_scope);
}
