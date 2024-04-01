#include <iostream>
#include <stack>
#include <string>
#include <sstream>

using namespace std;

// Kiểm tra xem một ký tự có phải là toán tử hay không
bool isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

// Thực hiện phép toán cho hai toán hạng và một toán tử
double performOperation(char op, double operand1, double operand2) {
    switch (op) {
        case '+':
            return operand1 + operand2;
        case '-':
            return operand1 - operand2;
        case '*':
            return operand1 * operand2;
        case '/':
            if (operand2 != 0) // Kiểm tra chia cho 0
                return operand1 / operand2;
            else {
                cerr << "Loi: chia cho 0\n";
                exit(EXIT_FAILURE);
            }
        default:
            cerr << "Loi: toan tu khong hop le\n";
            exit(EXIT_FAILURE);
    }
}

// Tính toán biểu thức số học
double evaluateExpression(string expression) {
    stack<double> operandStack; // Stack để lưu trữ các toán hạng
    stack<char> operatorStack; // Stack để lưu trữ các toán tử

    stringstream ss(expression);
    string token;
    while (getline(ss, token, ' ')) { // Tách biểu thức thành các token
        if (token.empty()) continue; // Bỏ qua các khoảng trắng không cần thiết

        if (isdigit(token[0])) { // Nếu là toán hạng
            double operand = stod(token);
            operandStack.push(operand);
        } else if (isOperator(token[0])) { // Nếu là toán tử
            while (!operatorStack.empty() && 
                   operatorStack.top() != '(' &&
                   ((token[0] != '*' && token[0] != '/') ||
                   (operatorStack.top() != '+' && operatorStack.top() != '-'))) {
                // Thực hiện phép toán nếu độ ưu tiên của toán tử trên cùng trong stack cao hơn
                char op = operatorStack.top();
                operatorStack.pop();

                double operand2 = operandStack.top();
                operandStack.pop();

                double operand1 = operandStack.top();
                operandStack.pop();

                double result = performOperation(op, operand1, operand2);
                operandStack.push(result);
            }
            operatorStack.push(token[0]);
        } else if (token[0] == '(') { // Nếu là dấu ngoặc mở
            operatorStack.push(token[0]);
        } else if (token[0] == ')') { // Nếu là dấu ngoặc đóng
            while (!operatorStack.empty() && operatorStack.top() != '(') {
                char op = operatorStack.top();
                operatorStack.pop();

                double operand2 = operandStack.top();
                operandStack.pop();

                double operand1 = operandStack.top();
                operandStack.pop();

                double result = performOperation(op, operand1, operand2);
                operandStack.push(result);
            }
            if (!operatorStack.empty() && operatorStack.top() == '(')
                operatorStack.pop(); // Loại bỏ dấu ngoặc mở khỏi stack
        } else {
            cerr << "Loi: bieu thuc khong hop le\n";
            exit(EXIT_FAILURE);
        }
    }

    // Thực hiện phép toán còn lại (nếu có)
    while (!operatorStack.empty()) {
        char op = operatorStack.top();
        operatorStack.pop();

        double operand2 = operandStack.top();
        operandStack.pop();

        double operand1 = operandStack.top();
        operandStack.pop();

        double result = performOperation(op, operand1, operand2);
        operandStack.push(result);
    }

    // Kết quả cuối cùng là toán hạng duy nhất còn lại trên stack
    return operandStack.top();
}

int main() {
    string expression;
    cout << "Nhap bieu thuc so hoc: ";
    getline(cin, expression);

    double result = evaluateExpression(expression);
    cout << "Ket qua: " << result << endl;

    return 0;
}
