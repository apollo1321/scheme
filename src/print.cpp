#include <string>

#include <object.h>
#include <print.h>
#include <scheme_error.h>

std::string PrintExpression(std::shared_ptr<Object> object) {
  if (object == nullptr) {
    return "()";
  }
  if (Is<Cell>(object)) {
    auto cell = As<Cell>(object);
    std::string result = "(";
    while (Is<Cell>(cell->GetSecond())) {
      result += PrintExpression(cell->GetFirst()) + " ";
      cell = As<Cell>(cell->GetSecond());
    }
    result += PrintExpression(cell->GetFirst());
    if (cell->GetSecond()) {
      result += " . " + PrintExpression(cell->GetSecond());
    }
    result += ")";
    return result;
  }
  if (Is<Bool>(object)) {
    return As<Bool>(object)->IsTrue() ? "#t" : "#f";
  }
  if (Is<Number>(object)) {
    auto value = As<Number>(object)->GetValue();
    return std::to_string(value);
  }
  if (Is<Symbol>(object)) {
    return As<Symbol>(object)->GetName();
  }
  return "";
}
