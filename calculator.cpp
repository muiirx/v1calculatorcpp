#include <iostream>
#include <mpfr.h>
#include <vector>
#include <regex>
#include <cctype>
#include <stack>
#include <string>
#include <queue>
#include <thread>
std::string logg;
class MPFRWrapper {
public:
    mpfr_t value;

    MPFRWrapper() {
        mpfr_init2(value, 512);  // Initialize with your choice precision
    }

    MPFRWrapper(const MPFRWrapper& other) {  // Copy constructor
        mpfr_init2(value, mpfr_get_prec(other.value));
        mpfr_set(value, other.value, MPFR_RNDN);  // Copy the value
    }

    MPFRWrapper& operator=(const MPFRWrapper& other) {  // Copy assignment
        if (mpfr_get_prec(value) != mpfr_get_prec(other.value)) {
            mpfr_set_prec(value, mpfr_get_prec(other.value));
        }
        return *this;
    }

    ~MPFRWrapper() {
        mpfr_clear(value);
    }
};

static const std::regex implicitreg(R"(\)(\d+|\())");

enum class TokenType { Number, Operator, Parenthesis, Function};
struct Token {
    TokenType type;
    std::string value;
    MPFRWrapper mpValue;

    Token(TokenType type, const std::string& value) : type(type), value(value) {
        if (type == TokenType::Number) {
            mpfr_set_str(mpValue.value, value.c_str(), 10, MPFR_RNDN);
        }
    }
};
 

std::vector<Token> tokenize(std::string& expression) {
    std::vector<Token> tokens;
    expression = std::regex_replace(expression, implicitreg, ")*$1");
    std::regex pattern(R"(\d+(\.\d+)?|[+*/()-^]|ln|sin|cos|tan|sqrt)");
    auto begin = std::sregex_iterator(expression.begin(), expression.end(), pattern);
    auto end = std::sregex_iterator();
    logg = expression;

    for (std::sregex_iterator i = begin; i != end; ++i) {
        std::smatch match = *i;
        std::string token = match.str(0);
        if (std::isdigit(token[0]) || token == ".") {
            tokens.emplace_back(TokenType::Number, token);
        } else if (token == "("|| token== ")") {
            tokens.emplace_back(TokenType::Parenthesis, token);
        }  else if (token == "ln" || token == "sin" || token == "cos" || token == "tan" || token == "sqrt") {
            tokens.emplace_back(TokenType::Function, token);
        } else {
            tokens.emplace_back(TokenType::Operator, token);
        }
    }
    return tokens;
}

std::vector<Token> postfix(const std::vector<Token>& tokens) {
    std::vector<Token> result;
    std::stack<Token> opstack;
    std::vector<Token> finalout;
    for(auto& token : tokens) {
        if(token.type == TokenType::Operator) {
            opstack.push(token);
        } else if(token.type == TokenType::Number) {
            finalout.push_back(token);
        } else if (token.type == TokenType::Parenthesis && token.value == "(") {
            opstack.push(token);
        } else if (token.type == TokenType::Parenthesis && token.value == ")") {
                while (opstack.top().value != "(") {
                    finalout.push_back(opstack.top());
                    opstack.pop();
                }
                if (opstack.top().value == "(") {
                    opstack.pop();
                } 
        } else if (token.type == TokenType::Function) {
            opstack.push(token);
        }
    }
    while (!opstack.empty()) {
        finalout.push_back(opstack.top());
        opstack.pop();
    }
    for (auto& token : finalout) {
        std::cout<< token.value << " ";
    }
    //finalopstack is the operators from outputstack and nmbstack is the numbers from outputstack (given outputstack is full postfix format)
    //vec notes: push=push_back, top=back, pop=pop_back
    return finalout;
}
//vec notes: push=push_back, top=back, pop=pop_back
std::string solvit(const std::vector<Token>& tokens) {
    std::vector<Token> reader;
    mpfr_t op1;
    mpfr_init2(op1, 512);
    mpfr_t op2;
    mpfr_init2(op2, 512);
    mpfr_t op3;
    mpfr_init2(op3, 512);
    Token tempstore{TokenType::Number, ""};
        for (auto& token : tokens) {
            if(token.type == TokenType::Number) {
                reader.push_back(token);
            } else if (token.type == TokenType::Operator) {

                
                mpfr_set(op1, reader.back().mpValue.value, MPFR_RNDN);
                reader.pop_back();
                mpfr_set(op2, reader.back().mpValue.value, MPFR_RNDN);
                reader.pop_back();

                if (token.value == "+") {mpfr_add(tempstore.mpValue.value, op1, op2, MPFR_RNDN); reader.push_back(tempstore);}
                if (token.value == "-") {mpfr_sub(tempstore.mpValue.value, op1, op2, MPFR_RNDN); reader.push_back(tempstore);}
                if (token.value == "*") {mpfr_mul(tempstore.mpValue.value, op1, op2, MPFR_RNDN); reader.push_back(tempstore);}
                if (token.value == "/") {mpfr_div(tempstore.mpValue.value, op1, op2, MPFR_RNDN); reader.push_back(tempstore);}
                if (token.value == "^") {mpfr_pow(tempstore.mpValue.value, op1, op2, MPFR_RNDN); reader.push_back(tempstore);}
            } else if (token.type == TokenType::Function) {
                mpfr_set(op3, reader.back().mpValue.value, MPFR_RNDN);
                reader.pop_back();
                if (token.value == "ln") {mpfr_log(tempstore.mpValue.value, op3, MPFR_RNDN); reader.push_back(tempstore);}
                if (token.value == "sin") {mpfr_sin(tempstore.mpValue.value, op3, MPFR_RNDN); reader.push_back(tempstore);}
                if (token.value == "cos") {mpfr_cos(tempstore.mpValue.value, op3, MPFR_RNDN); reader.push_back(tempstore);}
                if (token.value == "tan") {mpfr_tan(tempstore.mpValue.value, op3, MPFR_RNDN); reader.push_back(tempstore);}
                if (token.value == "sqrt") {mpfr_sqrt (tempstore.mpValue.value, op3, MPFR_RNDN); reader.push_back(tempstore);}
            }
        }
    char buffer[100];
    mpfr_sprintf(buffer, "%.20Rf", reader.back().mpValue.value);
    return buffer;
}

int main(int argc, char* argv[]) {
    std::string expression;
    if (argc >0) {
        expression = argv[1];
    }

    std::vector<Token> tokenized;
    std::thread t1([&]() {tokenized = tokenize(expression);});
    t1.join();

    std::vector<Token> postfixed;
    std::thread t2([&]() {postfixed = postfix(tokenized);});
    t2.join();

    std::string result;
    std::thread t3([&]() {result = solvit(postfixed);});
    t3.join();

    std::cout <<result;
}