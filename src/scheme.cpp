#include <memory>
#include <sstream>

#include <expression.h>
#include <object.h>
#include <parser.h>
#include <print.h>
#include <scheme_error.h>
#include <symbol.h>
#include <tokenizer.h>

#include <scheme.h>

std::string Interpret(const std::string& input, std::shared_ptr<Scope> scope) {
  std::stringstream stream(input);
  Tokenizer tokenizer(&stream);
  auto top_object = Read(&tokenizer);
  if (!tokenizer.IsEnd()) {
    throw SyntaxError("Bad input");
  }

  if (Is<Cell>(top_object)) {
    auto cell = As<Cell>(top_object);
    if (Is<Symbol>(cell->GetFirst())) {
      auto symbol = As<Symbol>(cell->GetFirst());
      if (symbol->GetName() == "define") {
        DefineObject(As<Cell>(top_object), scope);
        return "";
      }
    }
  }

  auto result = EvaluateExpression(top_object, scope);

  return PrintExpression(result);
}

std::shared_ptr<Scope> CreateScope() {
  return std::make_shared<Scope>();
}
