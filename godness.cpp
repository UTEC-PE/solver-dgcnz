#include <iostream>
#include <string>

using namespace std;
int main() {
  int ancho, alto;
  char c;
  cin >> ancho >> alto >> c;

  for (int i = 0; i <= c - 'A'; i++) {
    for (int j = 0; j < alto; j++) {
      if (j == 0 || j == alto - 1) {
        std::cout << std::string(ancho, char(i + 'A')) << std::endl;
      } else {
        std::cout << (char)(i + 'A') << std::string(ancho - 2, ' ')
                  << (char)(i + 'A') << std::endl;
      }
    }
  }
  return 0;
}
