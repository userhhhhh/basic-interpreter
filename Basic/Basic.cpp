/*
 * File: Basic.cpp
 * ---------------
 * This file is the starter project for the BASIC interpreter.
 */

#include <cctype>
#include <iostream>
#include <string>
#include "exp.hpp"
#include "parser.hpp"
#include "program.hpp"
#include "Utils/error.hpp"
#include "Utils/tokenScanner.hpp"
#include "Utils/strlib.hpp"

/* Function prototypes */

void processLine(std::string line, Program &program, EvalState &state);

/* Main program */

int main() {
   // freopen("../Test/trace87.txt","r",stdin);
    // freopen("class_code/Homework/Basic-Interpreter-main/Test/trace07.txt","r",stdin);
    // freopen("class_code/Homework/Basic-Interpreter-main/Basic/0.out","w",stdout);
    EvalState state;
    Program program;
    //cout << "Stub implementation of BASIC" << endl;
    while (true) {
        try {
            std::string input;
            getline(std::cin, input);
            if (input.empty())
                return 0;
            processLine(input, program, state);
        } catch (ErrorException &ex) {
            std::cout << ex.getMessage() << std::endl;
        }
    }
    return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version of
 * implementation, the program reads a line, parses it as an expression,
 * and then prints the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */


void processLine(std::string line, Program &program, EvalState &state) {
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.setInput(line);

    std::string it1=scanner.nextToken();
    int lineNumber;
    Statement* stmt;
    std::string token;
    if(!isNumeric(it1)){
        lineNumber = -1;
        token = it1;
    }
    else {
        lineNumber = std::stoi(it1);
        //错误：处理只输入了一个数字的情况
        if(line==it1){
            if(program.exist_line.find(lineNumber)!=program.exist_line.end()
            || program.processed_line.find(lineNumber)!=program.processed_line.end()){
                program.original_line.erase(lineNumber);
                program.exist_line.erase(lineNumber);
                delete program.processed_line[lineNumber];
                program.processed_line.erase(lineNumber);
            }
            //错误：return要放在里层if的外面
            return;
        }
        program.addSourceLine(lineNumber,line);
        stmt = nullptr;
        token = scanner.nextToken();
    }
    
    // 根据不同类型的标记进行处理
    if (token == "LET") {
        std::string str_in = scanner.nextToken();
        scanner.nextToken();
        Expression* expression = parseExp(scanner);

        //注意如果没有定义，那么会输出 VARIABLE NOT DEFINED
        //错误：value_in不能放在外面，应该放在里面，只能传入expression*

        stmt = new LET(str_in,expression);
        if(lineNumber==-1){
            try{
                stmt->execute(state,program);
                delete stmt;
            }
            catch(...){delete stmt;throw;}//错误：要接收
            return;
        }
    } else if (token == "PRINT") {
        Expression* expression = parseExp(scanner);
        stmt = new PRINT(expression);
        if(lineNumber==-1){
            try{
                stmt->execute(state,program);
                delete stmt;
                delete expression;
            }
            catch(...){
                delete stmt;
                delete expression;
                throw;
            }
            return;
        }
    } else if (token == "INPUT") {
        std::string variable = scanner.nextToken();
        stmt = new INPUT(variable);
        if(lineNumber==-1){
            stmt->execute(state,program);
            delete stmt;
            return;
        }
    } else if (token == "RUN") {
        auto it=program.exist_line.begin();
        while(it!=program.exist_line.end()){
            program.processed_line[*it]->execute(state,program);
            if(program.whether_stop==true) {
                //错误：要重置 whether_stop
                program.whether_stop= false;
                break;
            }
            if(program.current_line!=0){
                it = program.exist_line.find(program.current_line);
                program.current_line=0;
                //错误：continue不能漏
                continue;
            }
            it++;
        }
//        stmt = new RUN();
//        stmt->execute(state,program);
//        delete stmt;
        return;
    } else if (token == "END") {
        stmt = new END();
        if(lineNumber==-1){
            stmt->execute(state,program);
            delete stmt;
            return;
        }
    } else if (token == "GOTO") {
        std::string str1=scanner.nextToken();
        int value_in=std::stoi(str1);
        stmt = new GOTO(value_in);
        if(lineNumber==-1){
            stmt->execute(state,program);
            delete stmt;
            return;
        }
    } else if (token == "LIST") {
        stmt = new LIST();
        if(lineNumber==-1){
            stmt->execute(state,program);
            delete stmt;
            return;
        }
    } else if (token == "REM") {
        // 错误：这里只要构造一个REM就行，不用再进行其他操作
        stmt = new REM ();
        if(lineNumber==-1){
            stmt->execute(state,program);
            delete stmt;
            std::cout<<"SYNTAX ERROR"<<std::endl;
            return;
        }
    } else if (token == "IF") {
        // int pos=line.find_first_of("=<>");
        // std::string str1=line.substr(0,pos-1);

        Expression* a = readE(scanner,1);
        std::string str= scanner.nextToken();
        Expression* b = readE(scanner,1);
        scanner.nextToken();
        Expression* c = readE(scanner);
        int line_in = c->eval(state);
        delete c;
        //错误：这里使用了 readE，而 readE里面没有释放内存，所以要自己去释放内存
        //错误：在传入时只能传入Expression*类型的，因为不然可能还没有创建这个变量
        stmt = new IF (a, b, str, line_in);
        if(lineNumber==-1){
            stmt->execute(state,program);
            delete stmt;
            return;
        }
    } else if (token == "QUIT") {
        stmt = new QUIT();
        if(lineNumber==-1){
            delete stmt;
            exit(0);
        }
    } else if (token == "HELP") {
        stmt = new HELP();
        if(lineNumber==-1){
            stmt->execute(state,program);
            delete stmt;
            return;
        }
    } else if (token == "CLEAR") {
        stmt = new CLEAR;
        if(lineNumber==-1){
            stmt->execute(state,program);
            delete stmt;
            return;
        }
    }

    // 将解析后的语句存储到容器中

    program.setParsedStatement(lineNumber,stmt);
    //todo
}

