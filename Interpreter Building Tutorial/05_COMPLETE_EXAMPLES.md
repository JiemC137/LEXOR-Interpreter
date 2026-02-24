# Complete Example Programs for LEXOR

## Example 1: Hello World

**File: hello.lexor**
```
%% Simple hello world program
SCRIPT AREA
START SCRIPT
PRINT: "Hello" & $ & "World"
END SCRIPT
```

**Expected Output:**
```
Hello
World
```

---

## Example 2: Arithmetic Operations

**File: arithmetic.lexor**
```
%% Arithmetic operations example
SCRIPT AREA
START SCRIPT
DECLARE INT a=10, b=3
DECLARE INT sum, difference, product, quotient, remainder

sum = a + b
difference = a - b
product = a * b
quotient = a / b
remainder = a % b

PRINT: "a = " & a & $ & "b = " & b & $
PRINT: "Sum: " & sum & $
PRINT: "Difference: " & difference & $
PRINT: "Product: " & product & $
PRINT: "Quotient: " & quotient & $
PRINT: "Remainder: " & remainder
END SCRIPT
```

**Expected Output:**
```
a = 10
b = 3
Sum: 13
Difference: 7
Product: 30
Quotient: 3
Remainder: 1
```

---

## Example 3: Comparison Operators

**File: comparison.lexor**
```
%% Comparison operators
SCRIPT AREA
START SCRIPT
DECLARE INT x=5, y=10
DECLARE BOOL result

result = x < y
PRINT: "5 < 10: " & result & $

result = x > y
PRINT: "5 > 10: " & result & $

result = x == 5
PRINT: "5 == 5: " & result & $

result = x <> y
PRINT: "5 <> 10: " & result
END SCRIPT
```

**Expected Output:**
```
5 < 10: TRUE
5 > 10: FALSE
5 == 5: TRUE
5 <> 10: TRUE
```

---

## Example 4: Logical Operations

**File: logical.lexor**
```
%% Logical operations (AND, OR, NOT)
SCRIPT AREA
START SCRIPT
DECLARE BOOL a="TRUE", b="FALSE"
DECLARE BOOL result

result = a AND b
PRINT: "TRUE AND FALSE = " & result & $

result = a OR b
PRINT: "TRUE OR FALSE = " & result & $

result = NOT a
PRINT: "NOT TRUE = " & result
END SCRIPT
```

**Expected Output:**
```
TRUE AND FALSE = FALSE
TRUE OR FALSE = TRUE
NOT TRUE = FALSE
```

---

## Example 5: If Statement

**File: ifstatement.lexor**
```
%% If statement example
SCRIPT AREA
START SCRIPT
DECLARE INT age=25

IF (age >= 18)
START IF
PRINT: "You are an adult"
END IF
END SCRIPT
```

**Expected Output:**
```
You are an adult
```

---

## Example 6: If-Else Statement

**File: ifelse.lexor**
```
%% If-else statement
SCRIPT AREA
START SCRIPT
DECLARE INT number=15

IF (number % 2 == 0)
START IF
PRINT: "Even number"
END IF
ELSE
START IF
PRINT: "Odd number"
END IF
END SCRIPT
```

**Expected Output:**
```
Odd number
```

---

## Example 7: Chained If-Else

**File: chained_ifelse.lexor**
```
%% Chained if-else statements
SCRIPT AREA
START SCRIPT
DECLARE INT score=85

IF (score >= 90)
START IF
PRINT: "Grade: A"
END IF
ELSE IF (score >= 80)
START IF
PRINT: "Grade: B"
END IF
ELSE IF (score >= 70)
START IF
PRINT: "Grade: C"
END IF
ELSE
START IF
PRINT: "Grade: F"
END IF
END SCRIPT
```

**Expected Output:**
```
Grade: B
```

---

## Example 8: For Loop

**File: forloop.lexor**
```
%% For loop example
SCRIPT AREA
START SCRIPT
DECLARE INT i=1

START FOR
PRINT: i & $ 
i = i + 1
IF (i > 5)
START IF
END IF
END IF
END FOR
END SCRIPT
```

**Expected Output:**
```
1
2
3
4
5
```

---

## Example 9: Repeat-When Loop

**File: repeatloop.lexor**
```
%% Repeat-when loop example
SCRIPT AREA
START SCRIPT
DECLARE INT counter=0

REPEAT WHEN (counter < 5)
START REPEAT
PRINT: "Count: " & counter & $
counter = counter + 1
END REPEAT
END SCRIPT
```

**Expected Output:**
```
Count: 0
Count: 1
Count: 2
Count: 3
Count: 4
```

---

## Example 10: Factorial Program

**File: factorial.lexor**
```
%% Calculate factorial of a number
SCRIPT AREA
START SCRIPT
DECLARE INT n=5
DECLARE INT result=1
DECLARE INT i=1

REPEAT WHEN (i <= n)
START REPEAT
result = result * i
i = i + 1
END REPEAT

PRINT: "Factorial of " & n & " is " & result
END SCRIPT
```

**Expected Output:**
```
Factorial of 5 is 120
```

---

## Example 11: Sum of Numbers with User Input

**File: summing.lexor**
```
%% Sum numbers with user input
SCRIPT AREA
START SCRIPT
DECLARE INT count, sum=0, i=1, number

PRINT: "Enter number of values: "
SCAN: count

PRINT: "Enter " & count & " numbers (separated by commas):" & $

REPEAT WHEN (i <= count)
START REPEAT
PRINT: "Number " & i & ": "
SCAN: number
sum = sum + number
i = i + 1
END REPEAT

PRINT: "Sum: " & sum
END SCRIPT
```

**Expected Interaction:**
```
Enter number of values: 3
Enter 3 numbers (separated by commas):
Number 1: 5
Number 2: 10
Number 3: 15
Sum: 30
```

---

## Example 12: Character and String Operations

**File: characters.lexor**
```
%% Character operations
SCRIPT AREA
START SCRIPT
DECLARE CHAR letter='A'
DECLARE STRING message="Hello LEXOR"

PRINT: "Character: " & letter & $
PRINT: "Message: " & message & $
PRINT: "Combined: " & message & [#] & letter
END SCRIPT
```

**Expected Output:**
```
Character: A
Message: Hello LEXOR
Combined: Hello LEXOR#A
```

---

## Example 13: Escape Sequences

**File: escapes.lexor**
```
%% Using escape sequences
SCRIPT AREA
START SCRIPT
PRINT: "Line 1" & $ & "Line 2" & $
PRINT: "Hash: " & [#] & $
PRINT: "Dollar: " & [$] & $
PRINT: "[Bracket]" & [[]  & " text " & []]
END SCRIPT
```

**Expected Output:**
```
Line 1
Line 2
Hash: #
Dollar: $
[Bracket][ text ]
```

---

## Example 14: Temperature Converter

**File: tempconvert.lexor**
```
%% Convert Celsius to Fahrenheit
SCRIPT AREA
START SCRIPT
DECLARE FLOAT celsius, fahrenheit

PRINT: "Enter temperature in Celsius: "
SCAN: celsius

fahrenheit = (celsius * 9) / 5 + 32

PRINT: celsius & "C = " & fahrenheit & "F"
END SCRIPT
```

**Expected Interaction:**
```
Enter temperature in Celsius: 0
0C = 32F
```

---

## Example 15: Complex Expression

**File: complex_expr.lexor**
```
%% Complex expression evaluation
SCRIPT AREA
START SCRIPT
DECLARE INT a=10, b=5, c=3
DECLARE INT result

result = ((a + b) * c) - 2 * a
PRINT: "((10 + 5) * 3) - 2 * 10 = " & result
END SCRIPT
```

**Expected Output:**
```
((10 + 5) * 3) - 2 * 10 = 25
```

---

## How to Run These Examples

### With File Input:
```
interpreter.exe program.lexor
```

### With Standard Input:
```
interpreter.exe < program.lexor
```

### Interactive:
```
interpreter.exe
%% Type your program, press Ctrl+D (EOF) when done
```

---

## Testing Checklist

When implementing your interpreter, test with these examples in order:

1. ✓ Example 1 - Basic output
2. ✓ Example 2 - Arithmetic
3. ✓ Example 3 - Comparisons
4. ✓ Example 4 - Logical operations
5. ✓ Example 5 - Simple if
6. ✓ Example 6 - If-else
7. ✓ Example 7 - Chained if-else
8. ✓ Example 8 - For loops
9. ✓ Example 9 - While/repeat loops
10. ✓ Example 10 - Complex logic
11. ✓ Example 11 - User input
12. ✓ Example 12 - Characters
13. ✓ Example 13 - Escape sequences
14. ✓ Example 14 - Real-world program
15. ✓ Example 15 - Complex expressions

---

## Original Sample Program (From Specification)

**File: original_sample.lexor**
```
%% This is the sample program from the specification
SCRIPT AREA
START SCRIPT
DECLARE INT x, y, z=5
DECLARE CHAR a_1='n'
DECLARE BOOL t="TRUE"
x=y=4
a_1='c'
%% This is a comment
PRINT: x & t & z & $ & a_1 & [#] & "last"
END SCRIPT
```

**Expected Output:**
```
4TRUE5
c#last
```

Next: Building and Compilation Guide (06_BUILD_GUIDE.md)
