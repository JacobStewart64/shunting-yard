#include <iostream>
#include <sstream>
#include <string>
#include <queue>
#include <stack>
#include <limits>
#include <assert.h>
#include <math.h>

bool op_is_left_associative(char op)
{
    if (op == '/') return true;
    if (op == '-') return true;
    if (op == '^') return true;
    if (op == '!') return true;
    return false;
}

int get_precedence(char op)
{
    if (op == '(') return 4;
    if (op == ')') return 4;
    if (op == '!') return 3;
    if (op == '^') return 2;
    if (op == '*') return 1;
    if (op == '/') return 1;
    if (op == '+') return 0;
    if (op == '-') return 0;
    assert(false);
    return -1;
}

bool op_is_equal_precedence(char op, char op2)
{
    return get_precedence(op) == get_precedence(op2);
}

bool op_is_equal_precedence_and_left_associative(char op, char op2)
{
    return op_is_left_associative(op) && op_is_equal_precedence(op, op2);
}

bool op_is_lower_precedence(char op, char op2)
{
    return get_precedence(op) < get_precedence(op2);
}

std::queue<int> shunting_yard(std::string& expression_str)
{
    std::queue<int> output_queue;
    std::stack<char> operator_stack;
    std::stringstream expression(expression_str);

    while (expression.good())
    {
        if (!(expression >> std::ws))
            break;
        
        if (std::isdigit(expression.peek()))
        {
            //it is a number
            int number;
            if (!(expression >> number))
                break;
            
            assert(number < (INT32_MAX - 128));
            output_queue.push(number);
        }
        else if (expression.peek() == '(')
        {
            //left paren
            char left_paren;
            if (!(expression >> left_paren))
                break;
            
            operator_stack.push(left_paren);
        }
        else if (expression.peek() == ')')
        {
            //right paren
            char c;
            if (!(expression >> c))
                break;
            
            while (operator_stack.top() != '(')
            {
                char op = operator_stack.top();
                operator_stack.pop();
                output_queue.push(INT32_MAX - op);
            }

            assert(operator_stack.size() > 0);
            operator_stack.pop();
        }
        else
        {
            //it is an operator
            char op;
            if (!(expression >> op))
                break;
            
            if (operator_stack.size() > 0)
            {
                while ((op_is_lower_precedence(op, operator_stack.top()) ||
                    op_is_equal_precedence_and_left_associative(operator_stack.top(), op)) &&
                    operator_stack.top() != '(')
                {
                    char opc = operator_stack.top();
                    operator_stack.pop();
                    output_queue.push(INT32_MAX - opc);

                    if (operator_stack.size() == 0)
                        break;
                }
            }

            operator_stack.push(op);
        }
    }

    while (operator_stack.size() > 0)
    {
        char op = operator_stack.top();
        operator_stack.pop();
        output_queue.push(INT32_MAX - op);
    }

    return output_queue;
}

bool is_unary_op(char op)
{
    //if (op == '!') return 3; //idk how right now
    return false;
}

int do_op(int num1, int num2, char op)
{
    if (op == '^') return pow(num2, num1);
    if (op == '*') return num2 * num1;
    if (op == '/') return num2 / num1;
    if (op == '+') return num2 + num1;
    if (op == '-') return num2 - num1;
    assert(false);
    return 1;
}

int evaluate_queue(std::queue<int> output_queue)
{
    std::stack<int> number_stack;
    std::queue<char> op_queue;

    assert(output_queue.size() > 0);

    while (output_queue.size() > 0 && output_queue.front() < (INT32_MAX - 128))
    {
        number_stack.push(output_queue.front());
        output_queue.pop();
    }

    assert(output_queue.size() > 0);

    while (output_queue.size() > 0 && output_queue.front() >= (INT32_MAX - 128))
    {
        op_queue.push(INT32_MAX - output_queue.front());
        output_queue.pop();
    }

    assert(output_queue.size() == 0);

    int last_num;
    if (number_stack.size() > 0)
    {
        last_num = number_stack.top();
        number_stack.pop();
    }

    while (number_stack.size() > 0)
    {
        int num = number_stack.top();
        number_stack.pop();
        
        if (!is_unary_op(op_queue.front()))
        {
            last_num = do_op(last_num, num, op_queue.front());
            op_queue.pop();
        }
        else
        {
            //do unary op?
        }
    }

    return last_num;
}

int main(int argc, char* argv[])
{
    std::string expression("3 + 4 * (2 - 1)");
    std::cout << expression << std::endl;
    std::queue<int> output_queue = shunting_yard(expression);
    int result = evaluate_queue(output_queue);
    std::cout << result << std::endl;

    return 0;
}