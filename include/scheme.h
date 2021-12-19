#pragma once

#include <memory>
#include <string>

struct Scope;
std::shared_ptr<Scope> CreateScope();

std::string Interpret(const std::string& input, std::shared_ptr<Scope> scope);
