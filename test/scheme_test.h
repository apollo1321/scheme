#pragma once

#include <memory>

#include <scheme.h>
#include <scheme_error.h>
#include <catch2/catch_test_macros.hpp>

struct SchemeTest {
  std::shared_ptr<Scope> global_scope;

  SchemeTest() : global_scope(CreateScope()) {
  }

  // Implement following methods.
  void ExpectEq(std::string expression, std::string result) {
    REQUIRE(Interpret(expression, global_scope) == result);
  }

  void ExpectNoError(std::string expression) {
    REQUIRE_NOTHROW(Interpret(expression, global_scope));
  }

  void ExpectSyntaxError(std::string expression) {
    REQUIRE_THROWS_AS(Interpret(expression, global_scope), SyntaxError);
  }

  void ExpectRuntimeError(std::string expression) {
    REQUIRE_THROWS_AS(Interpret(expression, global_scope), RuntimeError);
  }

  void ExpectNameError(std::string expression) {
    REQUIRE_THROWS_AS(Interpret(expression, global_scope), UndefinedVariableError);
  }
};
