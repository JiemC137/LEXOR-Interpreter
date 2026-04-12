#ifndef VALUE_H
#define VALUE_H

#include <string>
#include <iostream>
using namespace std;

class Value {
public:
    enum Type {
        TYPE_INT,
        TYPE_FLOAT,
        TYPE_CHAR,
        TYPE_BOOL,
        TYPE_STRING,
        TYPE_VOID
    };
    
    Type type;
    double numValue;      // For INT and FLOAT
    char charValue;       // For CHAR
    bool boolValue;       // For BOOL
    string stringValue;   // For STRING
    
    // Constructors
    Value() : type(TYPE_VOID), numValue(0), charValue('\0'), boolValue(false) {}
    
    Value(int v) : type(TYPE_INT), numValue(v), charValue('\0'), boolValue(false) {}
    
    Value(double v) : type(TYPE_FLOAT), numValue(v), charValue('\0'), boolValue(false) {}
    
    Value(char v) : type(TYPE_CHAR), numValue(0), charValue(v), boolValue(false) {}
    
    Value(bool v) : type(TYPE_BOOL), numValue(0), charValue('\0'), boolValue(v) {}
    
    Value(string v) : type(TYPE_STRING), numValue(0), charValue('\0'), boolValue(false), stringValue(v) {}
    
    // Convert to string for output
    string toString() const {
        switch (type) {
            case TYPE_INT:
                return to_string((int)numValue);
            case TYPE_FLOAT:
                return to_string(numValue);
            case TYPE_CHAR:
                return string(1, charValue);
            case TYPE_BOOL:
                return boolValue ? "TRUE" : "FALSE";
            case TYPE_STRING:
                return stringValue;
            case TYPE_VOID:
                return "";
        }
        return "";
    }
    
    // Convert to number
    double toNumber() const {
        switch (type) {
            case TYPE_INT:
            case TYPE_FLOAT:
                return numValue;
            case TYPE_CHAR:
                return (double)charValue;
            case TYPE_BOOL:
                return boolValue ? 1.0 : 0.0;
            case TYPE_STRING:
                try {
                    return stod(stringValue);
                } catch (...) {
                    return 0.0;
                }
            default:
                return 0.0;
        }
    }
    
    // Convert to bool
    bool toBool() const {
        switch (type) {
            case TYPE_INT:
            case TYPE_FLOAT:
                return numValue != 0;
            case TYPE_CHAR:
                return charValue != '\0';
            case TYPE_BOOL:
                return boolValue;
            case TYPE_STRING:
                return !stringValue.empty();
            default:
                return false;
        }
    }
};

#endif