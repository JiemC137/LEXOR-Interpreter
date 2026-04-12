#include "lexer.hpp"
#include "parser.hpp"
#include "interpreter.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

string readFile(string filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Cannot open file: " + filename);
    }
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main(int argc, char* argv[]) {
    try {
        string code;
        
        if (argc > 1) {
            // Read from file
            code = readFile(argv[1]);
        } else {
            // Read from standard input
            string line;
            while (getline(cin, line)) {
                code += line + "\n";
            }
        }
        
        // Phase 1: Lexing
        cout << "=== LEXING ===" << endl;
        Lexer lexer(code);
        auto tokens = lexer.tokenize();
        cout << "Tokens generated: " << tokens.size() << endl;
        
        // Phase 2: Parsing
        cout << "\n=== PARSING ===" << endl;
        Parser parser(tokens);
        auto ast = parser.parse();
        
        if (!ast) {
            cerr << "Parsing failed" << endl;
            return 1;
        }
        cout << "AST constructed successfully" << endl;
        
        // Phase 3: Interpretation
        cout << "\n=== EXECUTION ===" << endl;
        Interpreter interpreter;
        interpreter.execute(ast);
        
        // Output results
        cout << "\n=== OUTPUT ===" << endl;
        cout << interpreter.getOutput();
        
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}