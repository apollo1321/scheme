#pragma once

#include <istream>
#include <optional>
#include <variant>

#include <inttypes.h>
#include <scheme_error.h>

struct SymbolToken {
  std::string name;

  bool operator==(const SymbolToken& other) const {
    return other.name == name;
  }
};

struct QuoteToken {
  bool operator==(const QuoteToken&) const {
    return true;
  }
};

struct DotToken {
  bool operator==(const DotToken&) const {
    return true;
  }
};

enum class BracketToken { OPEN, CLOSE };

struct ConstantToken {
  int64_t value;

  bool operator==(const ConstantToken& other) const {
    return other.value == value;
  }
};

typedef std::variant<ConstantToken, BracketToken, SymbolToken, QuoteToken, DotToken> Token;

class Tokenizer {
 public:
  explicit Tokenizer(std::istream* in) : in_{in} {
    Next();
  }

  bool IsEnd() const {
    return in_->eof();
  }

  void Next();

  Token GetToken() const {
    return token_;
  }

 private:
  std::istream* in_;
  Token token_;
};