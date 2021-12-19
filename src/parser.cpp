#include <parser.h>
#include <scheme_error.h>

bool CheckBracket(Tokenizer* tokenizer, BracketToken bracket_type) {
  Token token = tokenizer->GetToken();
  BracketToken* bracket = std::get_if<BracketToken>(&token);
  if (!bracket || *bracket != bracket_type) {
    return false;
  }
  return true;
}

std::shared_ptr<Object> ReadList(Tokenizer* tokenizer) {
  Token token = tokenizer->GetToken();
  if (std::get_if<DotToken>(&token)) {
    tokenizer->Next();
    return Read(tokenizer);
  }
  if (CheckBracket(tokenizer, BracketToken::CLOSE)) {
    return nullptr;
  }
  auto first = Read(tokenizer);
  auto second = ReadList(tokenizer);
  return std::make_shared<Cell>(first, second);
}

void ThrowIfEnd(Tokenizer* tokenizer) {
  if (tokenizer->IsEnd()) {
    throw SyntaxError{"Read: unexpected end"};
  }
}

void ThrowBadBracket() {
  throw SyntaxError{"Read: bad bracket sequence"};
}

std::shared_ptr<Object> Read(Tokenizer* tokenizer) {
  ThrowIfEnd(tokenizer);
  Token token = tokenizer->GetToken();
  if (ConstantToken* number = std::get_if<ConstantToken>(&token)) {
    tokenizer->Next();
    return std::make_shared<Number>(number->value);
  } else if (SymbolToken* symbol = std::get_if<SymbolToken>(&token)) {
    tokenizer->Next();
    if (symbol->name == "#f") {
      return std::make_shared<Bool>(false);
    }
    if (symbol->name == "#t") {
      return std::make_shared<Bool>(true);
    }
    return std::make_shared<Symbol>(symbol->name);
  } else if (std::get_if<QuoteToken>(&token)) {
    tokenizer->Next();
    auto quote_object = std::make_shared<Symbol>("quote");
    auto right = Read(tokenizer);
    return std::make_shared<Cell>(quote_object, std::make_shared<Cell>(right, nullptr));
  } else if (CheckBracket(tokenizer, BracketToken::OPEN)) {
    tokenizer->Next();
    ThrowIfEnd(tokenizer);
    if (CheckBracket(tokenizer, BracketToken::CLOSE)) {
      tokenizer->Next();
      return nullptr;
    }
    auto first = Read(tokenizer);
    auto second = ReadList(tokenizer);
    ThrowIfEnd(tokenizer);
    if (!CheckBracket(tokenizer, BracketToken::CLOSE)) {
      ThrowBadBracket();
    }
    tokenizer->Next();
    return std::make_shared<Cell>(first, second);
  } else if (CheckBracket(tokenizer, BracketToken::CLOSE)) {
    ThrowBadBracket();
  }
  throw SyntaxError{"Read: bad token sequence"};
}
