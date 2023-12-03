/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include "statement.hpp"
#include "evalstate.hpp"
#include "exp.hpp"
#include "program.hpp"
#include "Utils/strlib.hpp"
#include <string>


/* Implementation of the Statement class */

int stringToInt(std::string str);

Statement::Statement() = default;

Statement::~Statement() = default;

void REM::execute(EvalState &state,Program &program){}
LET::LET(std::string str_in,Expression* ex_in){
    str=str_in;
    ex=ex_in;
}
void LET::execute(EvalState &state,Program &program){
    try{
        int value1=ex->eval(state);
//        delete ex;
        //错误：要看这里有没有定义过这个变量
        if(str=="LET"||str=="REM"||str=="PRINT"||str=="INPUT"||str=="END"||str=="GOTO"||str=="IF"||str=="RUN"||str=="LIST"||str=="CLEAR"||str=="QUIT"||str=="HELP"){
            std::cout<<"SYNTAX ERROR"<<std::endl;
            return;
        }
        state.setValue(str,value1);
    }
    catch(...){throw;}
}
LET::~LET(){
    delete ex;
}
PRINT::PRINT(Expression* expression){
    a=expression;
}
void PRINT::execute(EvalState &state,Program &program){
    try{
        std::cout<<a->eval(state)<<std::endl;
    }
    catch(...){
        throw;
    }

//    std::cout<<a->eval(state)<<std::endl;
}
INPUT::INPUT(std::string variable){
    str=variable;
}
bool isNumeric(const std::string& str) {
    if(str[0]=='-'||str[0]=='+'){
        for(int i=1;i<str.size();++i){
            if(!std::isdigit(str[i])){
                return false;
            }
        }
    }
    else{
        for (char ch : str) {
            if (!std::isdigit(ch)) {
                return false;
            }
        }
    }
    return true;
}
void INPUT::execute(EvalState &state,Program &program){
    int num;
    std::string str_in;
    while(true){
        std::cout<<" ? ";
        getline(std::cin,str_in);
        if(isNumeric(str_in)){
            num=std::stoi(str_in);
            break;
        }
        else{
            std::cout<<"INVALID NUMBER"<<std::endl;
        }
    }
    state.setValue(str,num);
}
void END::execute(EvalState &state,Program &program){
    //错误：不会立即执行，会使RUN终止
    program.whether_stop=true;
}
GOTO::GOTO(int value_in){
    value=value_in;
}
void GOTO::execute(EvalState &state,Program &program){
    if(program.exist_line.count(value)==0){
        error("LINE NUMBER ERROR");
    }
    else program.current_line=value;
}
IF::IF(Expression* a,Expression* b,std::string str,int line_in){
    e1=a;
    e2=b;
    line=line_in;
    cmp=str;
}
void IF::execute(EvalState &state,Program &program){
    int value1=e1->eval(state);
    int value2=e2->eval(state);
//    delete e1;
//    delete e2;
    bool flag = false;
    if(cmp==">" && value1>value2) flag=true;
    if(cmp=="<" && value1<value2) flag=true;
    if(cmp=="=" && value1==value2) flag=true;
    if(flag==true){
        if(program.exist_line.count(line)==0){
            error("LINE NUMBER ERROR");
        }
        else{
            program.current_line=line;
        }
    }
}
IF::~IF(){
    delete e1;
    delete e2;
}
void RUN::execute(EvalState &state,Program &program){
    auto it=program.exist_line.begin();
    while(it!=program.exist_line.end()){
        program.processed_line[*it]->execute(state,program);
        if(program.whether_stop==true) break;
        if(program.current_line!=0){
            it = program.exist_line.find(program.current_line);
            program.current_line=0;
            //错误：continue不能漏
            continue;
        }
        it++;
    }
}
void LIST::execute(EvalState &state,Program &program){
    for(auto it=program.exist_line.begin();it!=program.exist_line.end();++it){
        std::cout<<program.original_line[*it]<<std::endl;
    }
}
void CLEAR::execute(EvalState &state,Program &program){
    program.clear();
    state.Clear();
}
void QUIT::execute(EvalState &state,Program &program){

    program.clear();
    state.Clear();
    exit(0);
}
//在quit的时候释放内存
void HELP::execute(EvalState &state,Program &program){
    std::cout << "Yet another basic interpreter" << std::endl;
}


