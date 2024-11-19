# Rational Calculator

This project is a C++ calculator for evaluating arithmetic expressions. The calculator uses infix notation. 
Built with the Qt Framework.

## Features

### Operations
- **Supports arithmetic operations**: `+`, `-`, `*`, `/`, and `^`.
- **Handles rational numbers** as well as integers and floating-point numbers.
- **Parses expressions with parentheses** and handles nested expressions.
- **Supports unary minus** (e.g., `-5` or `(-3) + 4`).
- **Exponentiation support**: Implemented with the `^` operator, respecting right-associativity.

## Calculator Buttons and Features
### **Digit Buttons (0-9)**
- These buttons allow you to input numbers into the calculator. You can click any number button to add the corresponding digit to the current expression.

### **Operator Buttons**
- **Addition `+`**: Performs addition between two numbers.
- **Subtraction `-`**: Performs subtraction between two numbers.
- **Multiplication `*`**: Performs multiplication between two numbers.
- **Division `/`**: Performs division between two numbers.
- **Exponentiation `^`**: Raises a number to the power of another number (e.g., `2^3` equals `8`).

### **Parentheses Buttons**
- **Left Parenthesis  `(`**: Opens a new sub-expression for grouping, allowing for nested operations.
- **Right Parenthesis `)`**: Closes the current sub-expression.

### **Special Buttons**
- **Backspace**: Deletes the last character or operator entered in the expression.
- **Evaluate `=`**: Calculates the result of the current expression.
- **All Clear `AC`**: Clears the entire expression.

### **Settings**
- **Open Settings**: Opens the settings window, where you can configure output format and precision.

### **Clipboard Operations**
- **Copy**: Copies the current result or expression to your system clipboard.

### **Memory Operations**
The calculator provides memory functionality. There are 5 memory slots available for saving results:

- **Memory Recall (MR)**: Recalls the value stored in the selected memory slot.
- **Memory Save (MS)**: Saves the current result or expression to the selected memory slot.
- **Memory Add (M+)**: Adds the value stored in selcted memory to the last result.
- **Memory Clear (MC)**: Clears the value stored in the selected memory slot.

### **Displays**
The calculator has 3 displays.
- **Main display**: For entering the current expression.
- **History display**: Shows previous results and error in the current expression if present.
- **Memory display**: Shows the value of the selected memory slot.

## Expression Grammar

The following is the expression grammar supported by the calculator:

- **Expression** ::= Term ((PLUS | MINUS) Term)*
- **Term** ::= Factor ((MUL | DIV) Factor)*
- **Factor** ::= Unary (POW Unary)*
- **Unary** ::= (MINUS)? Primary
- **Primary** ::= INTEGER | FLOAT | LPAREN Expression RPAREN | Constant


### Supported Tokens
- **INTEGER**: Whole numbers (e.g., `42`, `-3`)
- **FLOAT**: Decimal numbers (e.g., `3.14`, `-0.25`)
- **PLUS** (`+`): Addition operator
- **MINUS** (`-`): Subtraction or unary minus operator
- **MUL** (`*`): Multiplication operator
- **DIV** (`/`): Division operator
- **POW** (`^`): Exponentiation operator
- **LPAREN** (`(`): Left parenthesis for grouping
- **RPAREN** (`)`): Right parenthesis for grouping

### Operator Precedence and Associativity

The operator precedence and associativity rules are defined as follows:

| Operator     | Precedence | Associativity   |
|--------------|------------|-----------------|
| `UNARY_MINUS`| 4          | Right           |
| `POW` (`^`)  | 3          | Right           |
| `*`, `/`     | 2          | Left            |
| `+`, `-`     | 1          | Left            |

## Getting Started

### Prerequisites

- **C++ compiler**: A compatible C++ compiler that supports C++20 or later.
- **Qt**: Version 6.0 or higher.
- **CMake**: Version 3.16 or hgiher.
- **Make**

### Building and Running
1. **Clone the repository** 
```bash
git clone https://github.com/Walec75/rational-calculator.git
cd rational-calculator
```
2. **Configure the project**
```bash
cmake -S. -Bbuild
```
3. **Build the project**
```bash
cd build && make
```
4. **Run the demostration**
```bash
./calculator # Runs the calculator
./tests # Runs tests
```

## Licenses
This project uses the **Qt Framework** and **Catch2** framework.

**Qt Framework**, which is licensed under the **Qt Open Source License**.

- For more details, please review the full license terms on the official Qt website. **[Qt Open Source License](https://www.qt.io/terms-conditions)**



**Catch2** framework for testing. 
- You can find more about Catch2 on its **[GitHub repository](https://github.com/catchorg/Catch2)** and view the **[license details](https://github.com/catchorg/Catch2/blob/devel/LICENSE.txt)**.

