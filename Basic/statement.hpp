/*
 * File: statement.h
 * -----------------
 * This file defines the Statement abstract type.  In
 * the finished version, this file will also specify subclasses
 * for each of the statement types.  As you design your own
 * version of this class, you should pay careful attention to
 * the exp.h interface, which is an excellent model for
 * the Statement class hierarchy.
 */

#ifndef _statement_h
#define _statement_h

#include <string>
#include <sstream>
#include "evalstate.hpp"
#include "exp.hpp"
#include "Utils/tokenScanner.hpp"
#include"parser.hpp"
#include "Utils/error.hpp"
#include "Utils/strlib.hpp"
#include "program.hpp"

class Program;

/*
 * Class: Statement
 * ----------------
 * This class is used to represent a statement in a program.
 * The model for this class is Expression in the exp.h interface.
 * Like Expression, Statement is an abstract class with subclasses
 * for each of the statement and command types required for the
 * BASIC interpreter.
 */

class Statement {

public:

/*
 * Constructor: Statement
 * ----------------------
 * The base class constructor is empty.  Each subclass must provide
 * its own constructor.
 */

    Statement();

/*
 * Destructor: ~Statement
 * Usage: delete stmt;
 * -------------------
 * The destructor deallocates the storage for this expression.
 * It must be declared virtual to ensure that the correct subclass
 * destructor is called when deleting a statement.
 */

    virtual ~Statement();

/*
 * Method: execute
 * Usage: stmt->execute(state);
 * ----------------------------
 * This method executes a BASIC statement.  Each of the subclasses
 * defines its own execute method that implements the necessary
 * operations.  As was true for the expression evaluator, this
 * method takes an EvalState object for looking up variables or
 * controlling the operation of the interpreter.
 */

    virtual void execute(EvalState &state,Program &program) = 0;
// 这是 Statement 类中的一个纯虚函数声明。
// 它接受两个参数，一个是 EvalState 对象，用于查找变量或控制解释器的操作；
// 另一个是 Program 对象，用于执行程序中其他语句的操作。
// 该函数没有具体的实现，因为它是一个纯虚函数，由子类来实现具体的执行逻辑。
// 子类需要重写 execute() 函数，并根据特定语句的要求实现相应的操作。
// 每个子类都会定义自己的 execute() 方法来执行特定类型的语句。

    virtual void erase_print(){};

};


/*
 * The remainder of this file must consists of subclass
 * definitions for the individual statement forms.  Each of
 * those subclasses must define a constructor that parses a
 * statement from a scanner and a method called execute,
 * which executes that statement.  If the private data for
 * a subclass includes data allocated on the heap (such as
 * an Expression object), the class implementation must also
 * specify its own destructor method to free that memory.
 */
bool isNumeric(const std::string& str);
class REM:public Statement{
    public:
        virtual void execute(EvalState &state,Program &program) override;
};
class LET:public Statement{
    public:
    std::string str;
    Expression* ex;
    LET(std::string,Expression*);
    virtual void execute(EvalState &state,Program &program) override;
    ~LET();
};
class PRINT:public Statement{
    public:
    Expression* a;
    PRINT(Expression*);
    virtual void execute(EvalState &state,Program &program) override;
    void erase_print(){delete a;}
};
class INPUT:public Statement{
    public:
    std::string str;
    INPUT(std::string variable);
    virtual void execute(EvalState &state,Program &program) override;
};
class END:public Statement{
    public:
    virtual void execute(EvalState &state,Program &program) override;
};
class GOTO:public Statement{
    public:
    int value;
    GOTO(int);
    virtual void execute(EvalState &state,Program &program) override;
};
class IF:public Statement{
    public:
    Expression* e1;
    Expression* e2;
    std::string cmp;
    int line;
    IF (Expression*, Expression*, std::string, int);
    virtual void execute(EvalState &state,Program &program) override;
    virtual ~IF();
};
class RUN:public Statement{
    public:
    virtual void execute(EvalState &state,Program &program) override;
};
class LIST:public Statement{
    public:
    virtual void execute(EvalState &state,Program &program) override;
};
class CLEAR:public Statement{
    public:
    virtual void execute(EvalState &state,Program &program) override;
};
class QUIT:public Statement{
    public:
    virtual void execute(EvalState &state,Program &program) override;
};
class HELP:public Statement{
    public:
    virtual void execute(EvalState &state,Program &program) override;
};

#endif
