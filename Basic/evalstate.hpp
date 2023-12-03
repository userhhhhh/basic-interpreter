/*
 * File: evalstate.h
 * -----------------
 * This interface exports a class called EvalState, which keeps track
 * of additional information required by the evaluator, most notably
 * the values of variables.
 */

#ifndef _evalstate_h
#define _evalstate_h

#include <string>
#include <map>

/*
 * Class: EvalState
 * ----------------
 * This class is passed by reference through the recursive levels
 * of the evaluator and contains information from the evaluation
 * environment that the evaluator may need to know.  In this
 * version, the only information maintained by the EvalState class
 * is a symbol table that maps variable names into their values.
 * In your implementation, you may include additional information
 * in the EvalState class.
 */

class EvalState {

public:

/*
 * Constructor: EvalState
 * Usage: EvalState state;
 * -----------------------
 * Creates a new EvalState object with no variable bindings.
 */

    EvalState();

/*
 * Destructor: ~EvalState
 * Usage: usually implicit
 * -----------------------
 * Frees all heap storage associated with this object.
 */

    ~EvalState();

/*
 * Method: setValue
 * Usage: state.setValue(var, value);
 * ----------------------------------
 * Sets the value associated with the specified var.
 */

    void setValue(std::string var, int value);

/*
 * Method: getValue
 * Usage: int value = state.getValue(var);
 * ---------------------------------------
 * Returns the value associated with the specified variable.
 */

    int getValue(std::string var);

/*
 * Method: isDefined
 * Usage: if (state.isDefined(var)) . . .
 * --------------------------------------
 * Returns true if the specified variable is defined.
 */

    bool isDefined(std::string var);

    void Clear();

private:

    std::map<std::string, int> symbolTable;

};

#endif

// 定义了一个名为 EvalState 的类，该类用于跟踪评估器所需的额外信息，主要是变量的值。

// 类中的公有成员函数包括：

// setValue 方法：设置指定变量的值。
// getValue 方法：返回指定变量的值。
// isDefined 方法：检查指定变量是否已定义。
// Clear 方法用于清除符号表中的所有变量。

// 构造函数 EvalState()：创建一个新的 EvalState 对象，没有任何变量绑定。
// 析构函数 ~EvalState()：释放与对象相关的堆存储空间。
// 私有成员变量 symbolTable 是一个 std::map 类型的对象，用于存储变量名及其对应的值。

// 使用示例：
// EvalState state;
// state.setValue("x", 10);
// state.setValue("y", 5);

// int result = state.getValue("x") + state.getValue("y");
// std::cout << "Result: " << result << std::endl;
// state.Clear();

// if (state.isDefined("x")) {
//     int value = state.getValue("x");
//     std::cout << "The value of x is: " << value << std::endl;
// } else {
//     std::cout << "x is not defined." << std::endl;
// }

// 代码解释：
// state定义了一个符号类，将每个数字赋值给一个变量，内部就是用map将string与int关联起来
// setvalue是定义x为10，定义y为5
// getValue是对这个string进行使用，相当于得到了一个int
// clear函数用于清空符号表，删除所有已定义的标识符及其对应的值
// isDefined判断标识符是否已定义，就是检查有没有setvalue过x