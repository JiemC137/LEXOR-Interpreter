# Lexer Implementation Tutorial

## What is a Lexer?
The lexer (also called tokenizer or scanner) reads source code character-by-character and converts it into a stream of tokens that the parser can understand.

## Step-by-Step Lexer Development

### Step 1: Define Token Types

Create a file `token.h`:

```cpp
#ifndef TOKEN_H
#define TOKEN_H

#include <string>
using namespace std;

enum TokenType {
    // Keywords
    TOKEN_SCRIPT,
    TOKEN_AREA,
    TOKEN_START,
    TOKEN_END,
    TOKEN_DECLARE,
    TOKEN_INT,
    TOKEN_CHAR,
    TOKEN_BOOL,
    TOKEN_FLOAT,
    TOKEN_PRINT,
    TOKEN_SCAN,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_FOR,
    TOKEN_REPEAT,
    TOKEN_WHEN,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_NOT,
    
    // Literals
    TOKEN_NUMBER,        // 123, 45.67
    TOKEN_STRING,        // "hello"
    TOKEN_CHAR_LIT,      // 'a'
    TOKEN_TRUE,          // "TRUE"
    TOKEN_FALSE,         // "FALSE"
    
    // Identifiers
    TOKEN_IDENTIFIER,    // variable names
    
    // Operators and Punctuation
    TOKEN_PLUS,          // +
    TOKEN_MINUS,         // -
    TOKEN_MULTIPLY,      // *
    TOKEN_DIVIDE,        // /
    TOKEN_MODULO,        // %
    TOKEN_GT,            // >
    TOKEN_LT,            // <
    TOKEN_GTE,           // >=
    TOKEN_LTE,           // <=
    TOKEN_EQ,            // ==
    TOKEN_NEQ,           // <>
    TOKEN_ASSIGN,        // =
    TOKEN_CONCAT,        // &
    TOKEN_NEWLINE,       // $
    TOKEN_LPAREN,        // (
    TOKEN_RPAREN,        // )
    TOKEN_LBRACKET,      // [
    TOKEN_RBRACKET,      // ]
    TOKEN_COLON,         // :
    TOKEN_COMMA,         // ,
    
    // Special
    TOKEN_EOF,           // End of file
    TOKEN_ERROR          // Error token
};

struct Token {
    TokenType type;
    string value;
    int line;
    int column;
    
    Token(TokenType t, string v, int l, int c) 
        : type(t), value(v), line(l), column(c) {}
};

#endif
```

### Step 2: Create the Lexer Class

Create `lexer.h`:

```cpp
#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <string>
#include <vector>
using namespace std;

class Lexer {
private:
    string source;
    int position;      // Current character position
    int line;
    int column;
    char current;      // Current character
    
    // Helper functions
    void advance();                      // Move to next character
    char peek();                         // Look at next character without moving
    void skipWhitespace();               // Skip spaces and tabs (not newlines)
    void skipComment();                  // Skip %% comments
    bool isDigit(char c);
    bool isAlpha(char c);
    bool isAlphaNumeric(char c);
    
    // Token creation
    Token readNumber();                  // Read 123 or 45.67
    Token readString();                  // Read "hello"
    Token readCharLiteral();             // Read 'a'
    Token readIdentifierOrKeyword();     // Read variable names or keywords
    TokenType getKeywordType(string s);  // Check if identifier is keyword
    
public:
    Lexer(string sourceCode);
    Token nextToken();                   // Get next token
    vector<Token> tokenize();            // Get all tokens
};

#endif
```

### Step 3: Implement the Lexer

Create `lexer.cpp`:

```cpp
#include "lexer.h"
#include <cctype>
#include <iostream>

Lexer::Lexer(string sourceCode) {
    source = sourceCode;
    position = 0;
    line = 1;
    column = 1;
    current = (position < source.length()) ? source[0] : '\0';
}

void Lexer::advance() {
    if (current == '\n') {
        line++;
        column = 1;
    } else {
        column++;
    }
    position++;
    current = (position < source.length()) ? source[position] : '\0';
}

char Lexer::peek() {
    int nextPos = position + 1;
    return (nextPos < source.length()) ? source[nextPos] : '\0';
}

void Lexer::skipWhitespace() {
    while (current == ' ' || current == '\t') {
        advance();
    }
}

void Lexer::skipComment() {
    if (current == '%' && peek() == '%') {
        // Skip %% and everything until end of line
        advance(); // skip first %
        advance(); // skip second %
        while (current != '\n' && current != '\0') {
            advance();
        }
        if (current == '\n') advance(); // skip newline
    }
}

bool Lexer::isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool Lexer::isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool Lexer::isAlphaNumeric(char c) {
    return isAlpha(c) || isDigit(c);
}

Token Lexer::readNumber() {
    int startLine = line;
    int startColumn = column;
    string value = "";
    bool hasDecimal = false;
    
    while (isDigit(current) || (current == '.' && !hasDecimal)) {
        if (current == '.') hasDecimal = true;
        value += current;
        advance();
    }
    
    return Token(TOKEN_NUMBER, value, startLine, startColumn);
}

Token Lexer::readString() {
    int startLine = line;
    int startColumn = column;
    string value = "";
    advance(); // skip opening quote
    
    while (current != '"' && current != '\0') {
        if (current == '\\') {
            advance();
            if (current == 'n') value += '\n';
            else if (current == 't') value += '\t';
            else if (current == '"') value += '"';
            else value += current;
            advance();
        } else {
            value += current;
            advance();
        }
    }
    
    if (current == '"') advance(); // skip closing quote
    
    return Token(TOKEN_STRING, value, startLine, startColumn);
}

Token Lexer::readCharLiteral() {
    int startLine = line;
    int startColumn = column;
    advance(); // skip opening quote
    string value = "";
    
    if (current != '\'') {
        value += current;
        advance();
    }
    
    if (current == '\'') advance(); // skip closing quote
    
    return Token(TOKEN_CHAR_LIT, value, startLine, startColumn);
}

TokenType Lexer::getKeywordType(string s) {
    if (s == "SCRIPT") return TOKEN_SCRIPT;
    if (s == "AREA") return TOKEN_AREA;
    if (s == "START") return TOKEN_START;
    if (s == "END") return TOKEN_END;
    if (s == "DECLARE") return TOKEN_DECLARE;
    if (s == "INT") return TOKEN_INT;
    if (s == "CHAR") return TOKEN_CHAR;
    if (s == "BOOL") return TOKEN_BOOL;
    if (s == "FLOAT") return TOKEN_FLOAT;
    if (s == "PRINT") return TOKEN_PRINT;
    if (s == "SCAN") return TOKEN_SCAN;
    if (s == "IF") return TOKEN_IF;
    if (s == "ELSE") return TOKEN_ELSE;
    if (s == "FOR") return TOKEN_FOR;
    if (s == "REPEAT") return TOKEN_REPEAT;
    if (s == "WHEN") return TOKEN_WHEN;
    if (s == "AND") return TOKEN_AND;
    if (s == "OR") return TOKEN_OR;
    if (s == "NOT") return TOKEN_NOT;
    if (s == "TRUE") return TOKEN_TRUE;
    if (s == "FALSE") return TOKEN_FALSE;
    return TOKEN_IDENTIFIER;
}

Token Lexer::readIdentifierOrKeyword() {
    int startLine = line;
    int startColumn = column;
    string value = "";
    
    while (isAlphaNumeric(current)) {
        value += current;
        advance();
    }
    
    TokenType type = getKeywordType(value);
    return Token(type, value, startLine, startColumn);
}

Token Lexer::nextToken() {
    skipWhitespace();
    
    // Skip comments
    while (current == '%' && peek() == '%') {
        skipComment();
        skipWhitespace();
    }
    
    int startLine = line;
    int startColumn = column;
    
    if (current == '\0') {
        return Token(TOKEN_EOF, "", startLine, startColumn);
    }
    
    // Numbers
    if (isDigit(current)) {
        return readNumber();
    }
    
    // Strings
    if (current == '"') {
        return readString();
    }
    
    // Character literals
    if (current == '\'') {
        return readCharLiteral();
    }
    
    // Identifiers and keywords
    if (isAlpha(current)) {
        return readIdentifierOrKeyword();
    }
    
    // Special escape sequences [#], [$], etc.
    if (current == '[') {
        advance();
        if (current != ']') {
            string escape = "";
            escape += current;
            advance();
            if (current == ']') advance();
            return Token(TOKEN_STRING, escape, startLine, startColumn);
        }
        advance();
        return Token(TOKEN_RBRACKET, "]", startLine, startColumn - 1);
    }
    
    // Operators and punctuation
    char ch = current;
    advance();
    
    switch (ch) {
        case '+': return Token(TOKEN_PLUS, "+", startLine, startColumn);
        case '-': return Token(TOKEN_MINUS, "-", startLine, startColumn);
        case '*': return Token(TOKEN_MULTIPLY, "*", startLine, startColumn);
        case '/': return Token(TOKEN_DIVIDE, "/", startLine, startColumn);
        case '%': return Token(TOKEN_MODULO, "%", startLine, startColumn);
        case '&': return Token(TOKEN_CONCAT, "&", startLine, startColumn);
        case '$': return Token(TOKEN_NEWLINE, "$", startLine, startColumn);
        case '(': return Token(TOKEN_LPAREN, "(", startLine, startColumn);
        case ')': return Token(TOKEN_RPAREN, ")", startLine, startColumn);
        case '[': return Token(TOKEN_LBRACKET, "[", startLine, startColumn);
        case ']': return Token(TOKEN_RBRACKET, "]", startLine, startColumn);
        case ':': return Token(TOKEN_COLON, ":", startLine, startColumn);
        case ',': return Token(TOKEN_COMMA, ",", startLine, startColumn);
        case '=':
            if (current == '=') {
                advance();
                return Token(TOKEN_EQ, "==", startLine, startColumn);
            }
            return Token(TOKEN_ASSIGN, "=", startLine, startColumn);
        case '<':
            if (current == '=') {
                advance();
                return Token(TOKEN_LTE, "<=", startLine, startColumn);
            }
            if (current == '>') {
                advance();
                return Token(TOKEN_NEQ, "<>", startLine, startColumn);
            }
            return Token(TOKEN_LT, "<", startLine, startColumn);
        case '>':
            if (current == '=') {
                advance();
                return Token(TOKEN_GTE, ">=", startLine, startColumn);
            }
            return Token(TOKEN_GT, ">", startLine, startColumn);
        case '\n':
            return nextToken(); // Skip newlines in token stream
        default:
            return Token(TOKEN_ERROR, string(1, ch), startLine, startColumn);
    }
}

vector<Token> Lexer::tokenize() {
    vector<Token> tokens;
    Token token = nextToken();
    while (token.type != TOKEN_EOF) {
        tokens.push_back(token);
        token = nextToken();
    }
    tokens.push_back(token); // Add EOF token
    return tokens;
}
```

## Testing the Lexer

Create a simple test in `main.cpp`:

```cpp
#include "lexer.h"
#include <iostream>

int main() {
    string code = R"(
%% Sample LEXOR program
SCRIPT AREA
START SCRIPT
DECLARE INT x=5
DECLARE CHAR a='n'
PRINT: x & [#] & a
END SCRIPT
)";

    Lexer lexer(code);
    vector<Token> tokens = lexer.tokenize();
    
    cout << "Tokens:\n";
    for (const auto& token : tokens) {
        cout << "Line " << token.line << ", Col " << token.column 
             << ": " << token.value << endl;
    }
    
    return 0;
}
```

## Key Points

1. **Comments**: Skip `%%` and everything until end of line
2. **Escape Sequences**: `[#]` becomes just `#`
3. **Keywords**: Compare identifiers against known keywords
4. **Line Tracking**: Track line/column for error reporting
5. **Two-character operators**: Check for `<=`, `>=`, `==`, `<>`
6. **Strings vs Chars**: `"hello"` is string, `'a'` is char

Next: Parser Implementation (03_PARSER_TUTORIAL.md)
