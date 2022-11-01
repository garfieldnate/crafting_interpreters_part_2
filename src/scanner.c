#include <stdio.h>
#include <string.h>

#include "common.h"
#include "scanner.h"

typedef struct {
  // beginning of current lexeme being scanned
  const char* start;
  // current character being examined
  const char* current;
  int line;
} Scanner;

// global scanner object
// TODO: pass around a scanner pointer instead
Scanner scanner;

void initScanner(const char* source) {
  scanner.start = source;
  scanner.current = source;
  scanner.line = 1;
}

static bool isIdentifierChar(char c) {
  return (c >= 'a' && c <= 'z') ||
    (c >= 'A' && c <= 'Z') ||
    c == '_';
}

static bool isDigit(char c) {
  return c >= '0' && c <= '9';
}

static bool isAtEnd() {
  return *scanner.current == '\0';
}

static char advance() {
  scanner.current++;
  return scanner.current[-1];
}

static char peek() {
  return *scanner.current;
}

static char peekNext() {
  if (isAtEnd()) {
    return '\0';
  }
  return scanner.current[1];
}

// if we have expected char, consume it and return true; else return false
static bool match(char expected) {
  if (isAtEnd()) {
    return false;
  }
  if(*scanner.current != expected) {
    return false;
  }
  scanner.current++;
  return true;
}

static Token makeToken(TokenType type) {
  Token token;
  token.type = type;
  token.start = scanner.start;
  token.length = (int)(scanner.current - scanner.start);
  // note: for mult-line strings, this sets the token to the last line the string was on, not the first
  token.line = scanner.line;
  return token;
}

// must only ever be called with C string literals to ensure the lifetime of
// the message is long enough for the compiler to utilize it later
static Token errorToken(const char* message) {
  Token token;
  token.type = TOKEN_ERROR;
  token.start = message;
  token.length = (int)strlen(message);
  token.line = scanner.line;
  return token;
}

// TODO: there are definitely more possible whitespace tokens,
// but we don't seem to be handling non-ASCII input, anyway
static void skipWhitespace() {
  for (;;) {
    char c = peek();
    switch (c) {
      case ' ':
      case '\r':
      case '\t': {
        advance();
        break;
      }
      case '\n': {
        scanner.line++;
        advance();
        break;
      }
      // end-of-line comment
      case '/': {
        if (peekNext() == '/') {
          // check for but don't consume newline;
          // let next iteration handle it so the line number gets updated
          while (peek() != '\n' && !isAtEnd()) {
            advance();
          }
        } else {
            return;
        }
        break;
      }
      default: {
        return;
      }
    }
  }
}

static TokenType checkKeyword(int start, int length, const char* rest, TokenType type) {
  if (
    // test token length
    scanner.current - scanner.start == start + length &&
    // and then token content
    memcmp(scanner.start + start, rest, length) == 0) {
      return type;
    }

    // didn't match keyword, return user-defined identifier
    return TOKEN_IDENTIFIER;
}

// The code forms a trie/FSM searching for Lox keywords; if none are found, then we have
// a user-defined identifier
static TokenType identifierType() {
  switch (scanner.start[0]) {
    case 'a': return checkKeyword(1, 2, "nd", TOKEN_AND);
    case 'c': return checkKeyword(1, 4, "lass", TOKEN_CLASS);
    case 'e': return checkKeyword(1, 3, "lse", TOKEN_ELSE);
    case 'f': {
      // three keywords start with 'f'
      if (scanner.current - scanner.start > 1) {
        switch (scanner.start[1]) {
          case 'a': return checkKeyword(2, 3, "lse", TOKEN_FALSE);
          case 'o': return checkKeyword(2, 1, "r", TOKEN_FOR);
          case 'u': return checkKeyword(2, 1, "n", TOKEN_FUN);
        }
      }
      break;
    }
    case 'i': return checkKeyword(1, 1, "f", TOKEN_IF);
    case 'n': return checkKeyword(1, 2, "il", TOKEN_NIL);
    case 'o': return checkKeyword(1, 1, "r", TOKEN_OR);
    case 'p': return checkKeyword(1, 4, "rint", TOKEN_PRINT);
    case 'r': return checkKeyword(1, 5, "eturn", TOKEN_RETURN);
    case 's': return checkKeyword(1, 4, "uper", TOKEN_SUPER);
    case 't': {
      // two keywords start with t
      if (scanner.current - scanner.start > 1) {
        switch (scanner.start[1]) {
          case 'h': return checkKeyword(2, 2, "is", TOKEN_THIS);
          case 'r': return checkKeyword(2, 2, "ue", TOKEN_TRUE);
        }
      }
      break;
    }
    case 'v': return checkKeyword(1, 2, "ar", TOKEN_VAR);
    case 'w': return checkKeyword(1, 4, "hile", TOKEN_WHILE);
  }

  return TOKEN_IDENTIFIER;
}

static Token identifier() {
  while (isIdentifierChar(peek()) || isDigit(peek())) {
    advance();
  }
  return makeToken(identifierType());
}

static Token number() {
  while(isDigit(peek())) {
    advance();
  }

  // Look for decimal
  if (peek() == '.' && isDigit(peekNext())) {
    // consume '.'
    advance();
    while(isDigit(peek())) {
      advance();
    }
  }

  return makeToken(TOKEN_NUMBER);
}

static Token string() {
  while(peek() != '"' && !isAtEnd()) {
    printf("%c", peek());
    if(peek() == '\n') {
      scanner.line++;
    }
    advance();
  }

  if(isAtEnd()) return errorToken("Unterminated string.");

  // consume closing quote
  advance();
  return makeToken(TOKEN_STRING);
}

Token scanToken() {
  skipWhitespace();
  scanner.start = scanner.current;

  if(isAtEnd()) {
    return makeToken(TOKEN_EOF);
  }

  char c = advance();

  if(isIdentifierChar(c)) {
    return identifier();
  }
  if (isDigit(c)) {
    return number();
  }

  switch (c) {
    case '(': {
      return makeToken(TOKEN_LEFT_PAREN);
    }
    case ')': {
      return makeToken(TOKEN_RIGHT_PAREN);
    }
    case '{': {
      return makeToken(TOKEN_LEFT_BRACE);
    }
    case '}': {
      return makeToken(TOKEN_RIGHT_BRACE);
    }
    case ';': {
      return makeToken(TOKEN_SEMICOLON);
    }
    case ',': {
      return makeToken(TOKEN_COMMA);
    }
    case '.': {
      return makeToken(TOKEN_DOT);
    }
    case '-': {
      return makeToken(TOKEN_MINUS);
    }
    case '+': {
      return makeToken(TOKEN_PLUS);
    }
    case '/': {
      return makeToken(TOKEN_SLASH);
    }
    case '*': {
      return makeToken(TOKEN_STAR);
    }
    case '!': {
      return makeToken(
        match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
    }
    case '=': {
      return makeToken(
          match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
    }
    case '<': {
      return makeToken(
          match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
    }
    case '>': {
      return makeToken(
          match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
    }
    case '"': {
      return string();
    }
  }

  printf("What is '%d'?", c);
  return errorToken("Unexpected character.");
}
