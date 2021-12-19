#include <cctype>

#include <tokenizer.h>

static bool IsValidFirstSymbol(int symbol) {
  return std::isalpha(symbol) || symbol == '<' || symbol == '=' || symbol == '>' || symbol == '*' ||
         symbol == '#' || symbol == '/';
}

static bool IsValidInternalSymbol(int symbol) {
  return IsValidFirstSymbol(symbol) || std::isdigit(symbol) || symbol == '?' || symbol == '!' ||
         symbol == '-';
}

void Tokenizer::Next() {
  while (std::isspace(in_->peek())) {
    in_->get();
  }
  int symbol = in_->get();
  if (symbol == EOF) {
    return;
  }
  if (symbol == '.') {
    token_ = DotToken();
  } else if (symbol == '\'') {
    token_ = QuoteToken();
  } else if (symbol == '(') {
    token_ = BracketToken::OPEN;
  } else if (symbol == ')') {
    token_ = BracketToken::CLOSE;
  } else if (symbol == '+' || symbol == '-') {
    in_->unget();
    int64_t value;
    if (!(*in_ >> value)) {
      token_ = SymbolToken{symbol == '-' ? "-" : "+"};
    } else {
      token_ = ConstantToken{value};
    }
  } else if (std::isdigit(symbol)) {
    in_->unget();
    int64_t value;
    *in_ >> value;
    token_ = ConstantToken{value};
  } else if (IsValidFirstSymbol(symbol)) {
    std::string str;
    str += symbol;
    while (IsValidInternalSymbol(in_->peek()) && in_->good()) {
      str.push_back(in_->get());
    }
    token_ = SymbolToken{str};
  } else {
    throw SyntaxError{"Bad format"};
  }
  in_->clear();
}