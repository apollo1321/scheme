#include <exception>
#include <iostream>

#include <scheme.h>

void RunInterpreter() {
  auto global_scope = CreateScope();

  while (std::cin) {
    std::cout << "> ";
    std::string command_line, result;
    std::getline(std::cin, command_line);
    if (command_line.empty()) {
      continue;
    }
    try {
      result = Interpret(command_line, global_scope);
    } catch (std::exception& exception) {
      std::cout << exception.what() << std::endl;
      continue;
    }
    if (!result.empty()) {
      std::cout << result << std::endl;
    }
  }
}

int main() {
  RunInterpreter();
  return 0;
}
