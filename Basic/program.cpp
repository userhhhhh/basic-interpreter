/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include "program.hpp"
#include "evalstate.hpp"
#include "statement.hpp"
#include <string>


Program::Program() = default;

Program::~Program(){
    //错误：默认析构函数不会清除动态内存
    clear();
}
void Program::clear() {
    // std::cout<<exist_line.size()<<'\n';
    original_line.clear();
    for(auto it=exist_line.begin();it!=exist_line.end();++it){
        // std::cout<<*it<<'\n';
        delete processed_line[*it];
    }
    processed_line.clear();
    exist_line.clear();
}
void Program::addSourceLine(int lineNumber, const std::string &line) {
    if(exist_line.find(lineNumber)!=exist_line.end()) {
        original_line.erase(lineNumber);
        delete processed_line[lineNumber];
        processed_line[lineNumber] = nullptr;
    }
    original_line.insert(std::make_pair(lineNumber, line));
    exist_line.insert(lineNumber);
    // processed_line 在Basic文件中实现
}

void Program::removeSourceLine(int lineNumber) {
    if(exist_line.find(lineNumber)==exist_line.end()) return;
    original_line.erase(lineNumber);
    delete processed_line[lineNumber];
    processed_line.erase(lineNumber);
    exist_line.erase(lineNumber);
}

std::string Program::getSourceLine(int lineNumber) {
    if(exist_line.find(lineNumber)==exist_line.end())  return "";
    return original_line[lineNumber];
}
// hhuihuihui
void Program::setParsedStatement(int lineNumber, Statement *stmt) {
    // auto it2=processed_line.find(lineNumber);
    // if(it2!=processed_line.end()){
    //     if(it2!=nullptr){
    //         delete it2;
    //     }
    //     it2->second=stmt;
    //     return;
    // }
    if(processed_line.count(lineNumber)){
        // if(processed_line[lineNumber]!=NULL)std::cout<<"??";
        // exit(0);
        // processed_line[lineNumber]->execute(, Program &program)
        delete  processed_line[lineNumber];
        processed_line[lineNumber]=stmt;
    }
        // processed_line.erase(lineNumber);
        //processed_line.insert(std::make_pair(lineNumber,stmt));
        
    else {
        exist_line.insert(lineNumber);
        processed_line[lineNumber]=stmt;
    }
}

Statement *Program::getParsedStatement(int lineNumber) {
    if(processed_line.find(lineNumber)==processed_line.end()) return nullptr;
    return processed_line[lineNumber];
}

int Program::getFirstLineNumber() {
    if(exist_line.empty()) return -1;
    return *exist_line.begin();
}

int Program::getNextLineNumber(int lineNumber) {
    if(lineNumber==*exist_line.rbegin()) return -1;
    else return *exist_line.upper_bound(lineNumber);
}

//more func to add
//todo



// program用来存每一行的信息以及对每一行的操作

// Program() 构造函数：初始化 currentline 成员变量为 0。
// ~Program() 析构函数：释放程序中所有语句对象的内存。
// clear() 函数：清空程序中的所有行和与之相关联的数据结构，并释放相应的内存。
// setParsedStatement() 函数：将解析后的语句对象设置给指定的行号。
// 如果该行号已经存在，则先删除原有的语句对象，再存储新的语句对象。
// getParsedStatement() 函数：根据给定的行号返回对应的语句对象。
// getFirstLineNumber() 函数：获取程序中第一行的行号。如果程序为空，则返回 -1。
// getNextLineNumber() 函数：返回大于给定行号的下一个行号。如果没有更多行，则返回 -1。
// run() 函数：执行程序中的语句。从第一行开始逐行执行，直到遇到 END 语句。
// 如果遇到 GOTO 或者 IF 语句，会跳转到对应的行号；如果遇到无效的行号，则抛出错误。
// list() 函数：打印程序中的所有源代码行。
// addSourceLine() 函数：向程序中添加源代码行，
// 将行号和对应的源代码内容保存在 sourceline_map 中。
// removeLine() 函数：移除指定行号的源代码行和对应的语句对象，并释放相关的内存。
// getSourceLine() 函数：根据给定的行号返回相应的源代码内容。

// 综上所述，这些函数对程序进行操作，包括添加、删除、获取源代码行和解析后的语句对象。