#include "parser.h"
#include "tokenizer.h"
#include <iostream>
#include <string>
using namespace std;

void menu() {
  system("cls");
  cout << "===========================================================" << endl;
  cout << "\t\tPractica Operaciones" << endl;
  cout << "===========================================================" << endl;

  // char* str="7/4*((a+b)*a)+3";
  // char* str="7/4*((a+-+--b)*a)+3";
  // char* str="2+(3)";
  // char *str = "7/4*((a+b)^4*a)+3";
}

int main(int argc, char const *argv[]) {
  // menu();

  std::string text;
  getline(std::cin, text);
  Interpreter I(text);

  I.print();
  Parser Solver(text);
  Solver.Parse();
  Solver.visualize();
  Solver.execute();
  return EXIT_SUCCESS;
}
