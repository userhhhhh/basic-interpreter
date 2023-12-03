/*
 * File: parser.cpp
 * ----------------
 * Implements the parser.h interface.
 */

#include "parser.hpp"


/*
 * Implementation notes: parseExp
 * ------------------------------
 * This code just reads an expression and then checks for extra tokens.
 */

Expression *parseExp(TokenScanner &scanner) {
    Expression *exp = readE(scanner);
    if (scanner.hasMoreTokens()) {
        error("parseExp: Found extra token: " + scanner.nextToken());
    }
    return exp;
}

/*
 * Implementation notes: readE
 * Usage: exp = readE(scanner, prec);
 * ----------------------------------
 * This version of readE uses precedence to resolve the ambiguity in
 * the grammar.  At each recursive level, the parser reads operators and
 * subexpressions until it finds an operator whose precedence is greater
 * than the prevailing one.  When a higher-precedence operator is found,
 * readE calls itself recursively to read in that subexpression as a unit.
 */

Expression *readE(TokenScanner &scanner, int prec) {
    Expression *exp = readT(scanner);
    std::string token;
    while (true) {
        token = scanner.nextToken();
        int newPrec = precedence(token);
        if (newPrec <= prec) break;
        Expression *rhs = readE(scanner, newPrec);
        exp = new CompoundExp(token, exp, rhs);
    }
    scanner.saveToken(token);
    return exp;
}

/*
 * Implementation notes: readT
 * ---------------------------
 * This function scans a term, which is either an integer, an identifier,
 * or a parenthesized subexpression.
 */

Expression *readT(TokenScanner &scanner) {
    std::string token = scanner.nextToken();
    TokenType type = scanner.getTokenType(token);
    if (type == WORD) return new IdentifierExp(token);
    if (type == NUMBER) return new ConstantExp(stringToInteger(token));
    if (token == "-") return new CompoundExp(token, new ConstantExp(0), readE(scanner));
    if (token != "(") error("Illegal term in expression");
    Expression *exp = readE(scanner);
    if (scanner.nextToken() != ")") {
        error("Unbalanced parentheses in expression");
    }
    return exp;
}

/*
 * Implementation notes: precedence
 * --------------------------------
 * This function checks the token against each of the defined operators
 * and returns the appropriate precedence value.
 */

int precedence(std::string token) {
    if (token == "=") return 1;
    if (token == "+" || token == "-") return 2;
    if (token == "*" || token == "/") return 3;
    return 0;
}

// parseExp 函数读取一个表达式，并检查是否有额外的标记。如果有额外的标记，则抛出错误。

// readE 函数使用优先级解决文法中的模糊性。
// 在每个递归级别，解析器读取操作符和子表达式，直到找到优先级大于当前优先级的操作符。
// 当找到更高优先级的操作符时，readE 递归调用自身以读取该子表达式。
// 最后，将解析得到的表达式构建为一个 CompoundExp 对象。

// readT 函数扫描一个项（term），它可以是整数、标识符或括号之间的子表达式。
// 根据标记的类型，创建相应的表达式对象。

// precedence 函数根据给定的操作符标记返回其优先级值。根据具体的操作符，返回对应的优先级。

// 综上所述，这段代码提供了解析器的实现，包括解析表达式、解析项和确定操作符优先级的功能。new