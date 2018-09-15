#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::string detector(char c) {
  if (c >= 48 && c <= 57) {
    return "INT";
  } else if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122)) {
    return "CHAR";
  } else if (c == 94) {
    return "OP_^";
  } else if (c == 40) {
    return "LPAR";
  } else if (c == 41) {
    return "RPAR";
  } else if (c == 32) {
    return "WHITESPACE";
  } else if (c == 43) {
    return "OP_+";
  } else if (c == 45) {
    return "OP_-";
  } else if (c == 42) {
    return "OP_*";
  } else if (c == 47) {
    return "OP_/";
  } else if (c == 46) {
    return "DOT";
  } else if (c == 92) {
    return "BACKSLASH";
  } else if (c == 44) {
    return "COMMA";
  } else if (c == 0) {
    return "EOF";
  } else {
    return "UNSUPPORTED_CHAR";
  }
}

class Token {
  friend class Parser;

protected:
  std::string type;
  std::string value;

public:
  Token(std::string type, std::string value) : type(type), value(value){};
  Token(){};

  std::string __str__() {
    std::ostringstream os;
    os << "Token(" << type << ", " << value << ")";
    return os.str();
  }
};

class Interpreter {
  int pos;
  std::string text;
  std::string current_ctype;
  char current_c;

public:
  Interpreter(std::string text)
      : pos(0), text(text), current_ctype(::detector(text[0])),
        current_c(text[0]){};
  Interpreter(){};
  void reset() {
    this->pos = 0;
    this->current_c = this->text[0];
    this->current_ctype = detector(text[0]);
  }
  void next_char() {
    (this->pos)++;
    this->current_c = text[pos];
    this->current_ctype = detector(this->current_c);
  }
  Token get_next_token() {
    std::ostringstream result;

    if (current_ctype == "UNSUPPORTED_CHAR" || current_ctype == "WHITESPACE") {
      next_char();
      return get_next_token();
    } else if (current_ctype == "INT") {
      while (this->current_ctype == "INT") {
        result << this->current_c;
        next_char();
      }
      if (this->current_ctype == "DOT") {
        result << this->current_c;
        next_char();

        while (this->current_ctype == "INT") {
          result << this->current_c;
          next_char();
        }
      }
      Token current_Token("NUMBER", result.str());
      result.str(std::string()); // clears ostringstream
      return current_Token;

    } else if (current_ctype == "CHAR") {
      while (current_ctype == "CHAR") {
        result << this->current_c;
        next_char();
      }

      Token current_Token("VAR", result.str());
      return current_Token;
    }
    // Support for function tokenization, might add it in the parser
    else if (current_ctype == "BACKSLASH") {
      next_char();
      while (current_ctype == "CHAR") {
        result << this->current_c;
        next_char();
      }
      Token current_Token("FN_CALL", result.str());
      return current_Token;
    } else {
      std::string result;
      result.push_back(current_c);

      Token current_Token(current_ctype, result);
      next_char();
      return current_Token;
    }
  }
  void print() {

    reset();
    while (this->pos <= text.size()) {

      std::cout << this->pos << " ";
      std::cout << get_next_token().__str__() << std::endl;
    }
    reset();
  }
  std::vector<Token> Tokenize() {
    std::vector<Token> tokenized_text;

    while (this->pos <= text.size()) {
      tokenized_text.push_back(get_next_token());
    }
    return tokenized_text;
  }
};

#endif
