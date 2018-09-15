#include "parser.h"
#include "tokenizer.h"
#include <iostream>
#include <string>
#include <vector>
int main() {
  std::string text = "1+x*x";
  Parser Solver(text);
  Solver.visualize();
  Solver.execute();
}
