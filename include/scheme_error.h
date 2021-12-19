#pragma once

#include <stdexcept>

struct SyntaxError : public std::runtime_error {
  using std::runtime_error::runtime_error;
};

struct UndefinedVariableError : public std::runtime_error {
  using std::runtime_error::runtime_error;
};

struct RuntimeError : public std::runtime_error {
  using std::runtime_error::runtime_error;
};

struct InterpreterInternalError : public std::runtime_error {
  using std::runtime_error::runtime_error;
};
