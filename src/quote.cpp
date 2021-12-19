#include <common.h>
#include <object.h>
#include <quote.h>
#include <scheme_error.h>

std::shared_ptr<Object> QuoteDatum(std::shared_ptr<Cell> quote_list) {
  if (!BeginsWith(quote_list, "quote")) {
    throw InterpreterInternalError("QuoteDatum: bad input");
  }
  auto arguments = ListToVector(quote_list->GetSecond());
  if (arguments.size() != 1) {
    throw SyntaxError("Wrong quote arguments count");
  }
  return arguments[0];
}
