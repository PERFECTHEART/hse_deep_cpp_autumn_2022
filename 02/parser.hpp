#pragma once

#include <functional>
#include <locale>
#include <sstream>
#include <vector>

std::string MAX_UINT64 = std::to_string((uint64_t) - 1);

typedef enum {
  none,
  start,
  end
} parseState;

using fParseState = void(*)(parseState*);
using fStringPtr = void(*)(std::string);
using fInt64Ptr = void(*)(uint64_t);

class TokenParser {
public:
  parseState PState = none;
  fParseState startCallbackPtr = nullptr;
  fParseState endCallbackPtr = nullptr;
  fStringPtr stringTokenCallbackPtr = nullptr;
  fInt64Ptr digitTokenCallbackPtr = nullptr;

public: 
  parseState getState() {
    return PState;
  }

  // Устанавливаем callback-функцию перед стартом парсинга.
  void SetStartCallback(const fParseState& fPtr) {
    startCallbackPtr = fPtr;
  }

  // Устанавливаем callback-функцию после окончания парсинга.
  void SetEndCallback(const fParseState& fPtr) {
    endCallbackPtr = fPtr;
  }

  // Устанавливаем callback-функцию для обработки строк.
  void SetStringTokenCallback(const fStringPtr& fPtr) {
    stringTokenCallbackPtr = fPtr;
  }

  // Устанавливаем callback-функцию для обработки чисел.
  void SetDigitTokenCallback(const fInt64Ptr& fPtr) {
    digitTokenCallbackPtr = fPtr;
  }

  size_t skip_blanc(std::string str, size_t pos) {
    std::locale loc;

    while (std::isspace(str[pos], loc) && pos < str.length())
      ++pos;
    return pos;
  }

  void Parse(const std::string& str) {
    std::locale loc;
    size_t fin;
    size_t numpos;
    size_t pos = 0;
    bool isnumber;

    if (startCallbackPtr != nullptr) {
      startCallbackPtr( & PState); // Set parser state
    }

    while (pos < str.length()) {
      pos = skip_blanc(str, pos); // Skip leading blanks
      // Token start at pos position
      fin = pos;
      isnumber = true;
      while (!std::isspace(str[fin], loc) && fin < str.length()) {
        if (!std::isdigit(str[fin], loc)) {
          isnumber = false;
        }
        fin++; // Token finish position
      }
      if (fin != pos) {
#ifdef DEBUG
	std::cout << "token[" << str.substr(pos, fin-pos) << "]" << std::endl;
#endif
        if (isnumber) {
          // Save pos to keep leading zeros if digits should be parsed as string
          numpos = pos;
          // Truncate leading zeros
          while (str[numpos] == '0' && std::isdigit(str[numpos + 1], loc)) {
            numpos++;
          }
#ifdef DEBUG
	  std::cout << "numpos..fin-numpos#" << str.substr(numpos, fin-numpos) << "#" << std::endl;
#endif
          if (fin - numpos > MAX_UINT64.length() ||
            (fin - numpos == MAX_UINT64.length() && (MAX_UINT64.compare(str.substr(numpos, fin-numpos)) < 0))) {
            isnumber = false;
          }

        }
        if (isnumber) {
          if (digitTokenCallbackPtr != nullptr) {
            uint64_t x;
            std::istringstream iss(str.substr(numpos, fin - numpos));
            iss >> x;
            digitTokenCallbackPtr(x);
          }
        } else { // Строковый токен
          if (stringTokenCallbackPtr != nullptr) {
            stringTokenCallbackPtr(str.substr(pos, fin - pos));
          }
        }
      } // if( fin != pos )
      pos = fin;
    } // while( pos < str.length() ) {
    if (endCallbackPtr != nullptr) {
      endCallbackPtr(&PState); // Set parser state
    }
  } // void Parse...
};
