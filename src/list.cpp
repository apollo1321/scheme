#include <common.h>
#include <expression.h>
#include <list.h>
#include <object.h>
#include <scheme_error.h>

std::shared_ptr<Object> IsPair(std::shared_ptr<Cell> list, std::shared_ptr<Scope> scope) {
  if (!BeginsWith(list, "pair?")) {
    throw InterpreterInternalError("IsPair: bad input");
  }
  auto objects = EvaluateExpressions(ListToVector(list->GetSecond()), scope);
  if (objects.size() != 1 || (!Is<Cell>(objects[0]) && objects[0] != nullptr)) {
    throw SyntaxError("pair? statement requires only one cell argument");
  }
  if (objects[0] == nullptr) {
    return std::make_shared<Bool>(false);
  }
  auto cell = As<Cell>(objects[0]);
  auto objects_in_cell = ListToVector(cell);
  if (objects_in_cell.size() == 2 && cell->GetSecond() != nullptr) {
    return std::make_shared<Bool>(true);
  }
  return std::make_shared<Bool>(false);
}

std::shared_ptr<Object> IsNull(std::shared_ptr<Cell> list, std::shared_ptr<Scope> scope) {
  if (!BeginsWith(list, "null?")) {
    throw InterpreterInternalError("IsNull: bad input");
  }
  auto objects = EvaluateExpressions(ListToVector(list->GetSecond()), scope);
  if (objects.size() != 1) {
    throw SyntaxError("null? requires only one argument");
  }
  return std::make_shared<Bool>(objects[0] == nullptr);
}

std::shared_ptr<Object> IsList(std::shared_ptr<Cell> list, std::shared_ptr<Scope> scope) {
  if (!BeginsWith(list, "list?")) {
    throw InterpreterInternalError("IsList: bad input");
  }
  auto objects = EvaluateExpressions(ListToVector(list->GetSecond()), scope);
  if (objects.size() != 1) {
    throw SyntaxError("list? statement requires only one argument");
  }
  auto current_cell = objects[0];
  while (Is<Cell>(current_cell)) {
    current_cell = As<Cell>(current_cell)->GetSecond();
  }
  return std::make_shared<Bool>(current_cell == nullptr);
}

std::shared_ptr<Object> Cons(std::shared_ptr<Cell> list, std::shared_ptr<Scope> scope) {
  if (!BeginsWith(list, "cons")) {
    throw InterpreterInternalError("Cons: bad input");
  }
  auto objects = EvaluateExpressions(ListToVector(list->GetSecond()), scope);
  if (objects.size() != 2) {
    throw SyntaxError("cons requires two arguments");
  }
  return std::make_shared<Cell>(objects[0], objects[1]);
}

std::shared_ptr<Object> Car(std::shared_ptr<Cell> list, std::shared_ptr<Scope> scope) {
  if (!BeginsWith(list, "car")) {
    throw InterpreterInternalError("car: bad input");
  }
  auto objects = EvaluateExpressions(ListToVector(list->GetSecond()), scope);
  if (objects.size() != 1 || !Is<Cell>(objects[0])) {
    throw SyntaxError("car requires only one list argument");
  }
  return As<Cell>(objects[0])->GetFirst();
}

std::shared_ptr<Object> Cdr(std::shared_ptr<Cell> list, std::shared_ptr<Scope> scope) {
  if (!BeginsWith(list, "cdr")) {
    throw InterpreterInternalError("cdr: bad input");
  }
  auto objects = EvaluateExpressions(ListToVector(list->GetSecond()), scope);
  if (objects.size() != 1 || !Is<Cell>(objects[0])) {
    throw SyntaxError("cdr requires only one list argument");
  }
  return As<Cell>(objects[0])->GetSecond();
}

void SetCar(std::shared_ptr<Cell> list, std::shared_ptr<Scope> scope) {
  if (!BeginsWith(list, "set-car!")) {
    throw InterpreterInternalError("SetCar: bad input");
  }
  auto objects = EvaluateExpressions(ListToVector(list->GetSecond()), scope);
  if (objects.size() != 2 || !Is<Cell>(objects[0])) {
    throw SyntaxError("set-car! requires two arguments, the first is list");
  }
  As<Cell>(objects[0])->SetFirst(objects[1]);
}

void SetCdr(std::shared_ptr<Cell> list, std::shared_ptr<Scope> scope) {
  if (!BeginsWith(list, "set-cdr!")) {
    throw InterpreterInternalError("SetCdr: bad input");
  }
  auto objects = EvaluateExpressions(ListToVector(list->GetSecond()), scope);
  if (objects.size() != 2 || !Is<Cell>(objects[0])) {
    throw SyntaxError("set-cdr! requires two arguments, the first is list");
  }
  As<Cell>(objects[0])->SetSecond(objects[1]);
}

std::shared_ptr<Object> List(std::shared_ptr<Cell> list, std::shared_ptr<Scope> scope) {
  if (!BeginsWith(list, "list")) {
    throw InterpreterInternalError("List: bad input");
  }
  auto objects = EvaluateExpressions(ListToVector(list->GetSecond()), scope);
  std::shared_ptr<Object> result = nullptr;
  for (auto iterator = objects.rbegin(); iterator != objects.rend(); ++iterator) {
    result = std::make_shared<Cell>(*iterator, result);
  }
  return result;
}

std::shared_ptr<Object> ListRef(std::shared_ptr<Cell> list, std::shared_ptr<Scope> scope) {
  if (!BeginsWith(list, "list-ref")) {
    throw InterpreterInternalError("ListRef: bad input");
  }
  auto objects = EvaluateExpressions(ListToVector(list->GetSecond()), scope);
  if (objects.size() != 2 || !Is<Cell>(objects[0]) || !Is<Number>(objects[1])) {
    throw SyntaxError("list-ref requires two arguments: list and number");
  }

  auto list_objects = ListToVector(objects[0]);
  auto index = As<Number>(objects[1])->GetValue();
  if (index < 0 || static_cast<size_t>(index) >= list_objects.size()) {
    throw RuntimeError("list-ref: invalid index");
  }
  return list_objects[index];
}

std::shared_ptr<Object> ListTail(std::shared_ptr<Cell> list, std::shared_ptr<Scope> scope) {
  if (!BeginsWith(list, "list-tail")) {
    throw InterpreterInternalError("ListTail: bad input");
  }
  auto objects = EvaluateExpressions(ListToVector(list->GetSecond()), scope);
  if (objects.size() != 2 || !Is<Cell>(objects[0]) || !Is<Number>(objects[1])) {
    throw SyntaxError("list-tail requires two arguments: list and number");
  }
  auto index = As<Number>(objects[1])->GetValue();
  if (index < 0) {
    throw RuntimeError("list-tail: invalid index");
  }
  auto current_cell = objects[0];
  while (index != 0 && Is<Cell>(current_cell)) {
    --index;
    current_cell = As<Cell>(current_cell)->GetSecond();
  }
  if (index != 0) {
    throw RuntimeError("list-tail: invalid index");
  }
  return current_cell;
}
