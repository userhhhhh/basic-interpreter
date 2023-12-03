#ifndef CODE_ERROR_HPP
#define CODE_ERROR_HPP

#include <string>
#include <exception>

/*
 * Class: ErrorException
 * ---------------------
 * This exception is thrown by calls to the <code>error</code>
 * function.  Typical code for catching errors looks like this:
 *
 *<pre>
 *    try {
 *       ... code in which an error might occur ...
 *    } catch (ErrorException & ex) {
 *       ... code to handle the error condition ...
 *    }
 *</pre>
 *
 * If an <code>ErrorException</code> is thrown at any point in the
 * range of the <code>try</code> (including in functions called from
 * that code), control will jump immediately to the error handler.
 */

class ErrorException : public std::exception {
public:
    explicit ErrorException(std::string message);

    std::string getMessage() const;

private:
    std::string message;
};

//------------------------------------------------------------------------------------------------

void error(std::string message);

#endif //CODE_ERROR_HPP

// 定义了一个名为 ErrorException 的异常类和一个辅助函数 error

// ErrorException 类包含以下成员：
// 构造函数：创建 ErrorException 对象，并传入错误信息作为参数。
// getMessage 方法：用于获取错误信息字符串。
// error 函数是一个辅助函数，用于抛出 ErrorException 异常。
// 该函数接受一个错误信息字符串作为参数，并将其作为参数创建一个 ErrorException 对象，然后抛出该异常对象。