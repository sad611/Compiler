#pragma once
#include <memory>
#include <stdexcept>
#include <iostream>
#include <string>
#include <vector>

class Visitor;

enum class Operador { SOMA, SUB, MULT, DIV };

enum class ComparisonOperator { EQUAL, NOT_EQUAL, LESS, GREATER, LESS_EQUAL, GREATER_EQUAL };

enum class LogicalOperator { AND, OR };

std::string operadorToString(Operador op);
std::string comparisonOperatorToString(ComparisonOperator op);
std::string logicalOperatorToString(LogicalOperator op);

class Exp {
public:
    virtual ~Exp() = default;
    virtual void accept(Visitor& visitor) const = 0;
};

class Statement {
public:
    virtual ~Statement() = default;
    virtual void accept(Visitor& visitor) const = 0;
};

class Program {
public:
    std::vector<std::unique_ptr<Statement>> globalDeclarations;
    std::unique_ptr<Statement> mainFunction;
    
    void addGlobalDeclaration(std::unique_ptr<Statement> decl) {
        globalDeclarations.push_back(std::move(decl));
    }
    
    void setMainFunction(std::unique_ptr<Statement> main) {
        mainFunction = std::move(main);
    }
    
    void accept(Visitor& visitor) const;
};

class BlockStatement : public Statement {
public:
    std::vector<std::unique_ptr<Statement>> statements;
    
    void addStatement(std::unique_ptr<Statement> stmt) {
        statements.push_back(std::move(stmt));
    }
    
    void accept(Visitor& visitor) const override;
};

class MainFunction : public Statement {
public:
    std::unique_ptr<BlockStatement> body;
    
    explicit MainFunction(std::unique_ptr<BlockStatement> b) 
        : body(std::move(b)) {}
    
    void accept(Visitor& visitor) const override;
};

class ExpressionStatement : public Statement {
public:
    std::unique_ptr<Exp> expression;
    
    explicit ExpressionStatement(std::unique_ptr<Exp> expr) 
        : expression(std::move(expr)) {}
    
    void accept(Visitor& visitor) const override;
};

class VarDeclaration : public Statement {
public:
    std::string identifier;
    std::unique_ptr<Exp> initializer;
    
    VarDeclaration(std::string name, std::unique_ptr<Exp> init = nullptr)
        : identifier(std::move(name)), initializer(std::move(init)) {}
    
    void accept(Visitor& visitor) const override;
};

class Const : public Exp {
public:
    int valor;

    explicit Const(int val) : valor(val) {}
    
    void accept(Visitor& visitor) const override;
};

class BooleanLiteral : public Exp {
public:
    bool value;
    
    explicit BooleanLiteral(bool val) : value(val) {}
    
    void accept(Visitor& visitor) const override;
};

class Variable : public Exp {
public:
    std::string name;

    explicit Variable(std::string n) : name(std::move(n)) {}
    
    void accept(Visitor& visitor) const override;
};

class OpBin : public Exp {
public:
    Operador op;
    std::unique_ptr<Exp> opEsq;
    std::unique_ptr<Exp> opDir;

    OpBin(std::unique_ptr<Exp> esq, Operador o, std::unique_ptr<Exp> dir)
        : op(o), opEsq(std::move(esq)), opDir(std::move(dir)) {}

public:
    void accept(Visitor& visitor) const override;
};

class ComparisonExpression : public Exp {
public:
    ComparisonOperator op;
    std::unique_ptr<Exp> left;
    std::unique_ptr<Exp> right;

    ComparisonExpression(std::unique_ptr<Exp> l, ComparisonOperator o, std::unique_ptr<Exp> r)
        : op(o), left(std::move(l)), right(std::move(r)) {}

    void accept(Visitor& visitor) const override;
};

class LogicalExpression : public Exp {
public:
    LogicalOperator op;
    std::unique_ptr<Exp> left;
    std::unique_ptr<Exp> right;

    LogicalExpression(std::unique_ptr<Exp> l, LogicalOperator o, std::unique_ptr<Exp> r)
        : op(o), left(std::move(l)), right(std::move(r)) {}

    void accept(Visitor& visitor) const override;
};

class UnaryExpression : public Exp {
public:
    std::unique_ptr<Exp> operand;
    bool isNot;

    UnaryExpression(std::unique_ptr<Exp> operand, bool isNot = false)
        : operand(std::move(operand)), isNot(isNot) {}

    void accept(Visitor& visitor) const override;
};

class AssignmentExpression : public Exp {
public:
    std::string variable;
    std::unique_ptr<Exp> value;

    AssignmentExpression(std::string var, std::unique_ptr<Exp> val)
        : variable(std::move(var)), value(std::move(val)) {}

    void accept(Visitor& visitor) const override;
};

class IfStatement : public Statement {
public:
    std::unique_ptr<Exp> condition;
    std::unique_ptr<Statement> thenBranch;
    std::unique_ptr<Statement> elseBranch;

    IfStatement(std::unique_ptr<Exp> cond, std::unique_ptr<Statement> thenStmt, std::unique_ptr<Statement> elseStmt = nullptr)
        : condition(std::move(cond)), thenBranch(std::move(thenStmt)), elseBranch(std::move(elseStmt)) {}

    void accept(Visitor& visitor) const override;
};

class WhileStatement : public Statement {
public:
    std::unique_ptr<Exp> condition;
    std::unique_ptr<Statement> body;

    WhileStatement(std::unique_ptr<Exp> cond, std::unique_ptr<Statement> bodyStmt)
        : condition(std::move(cond)), body(std::move(bodyStmt)) {}

    void accept(Visitor& visitor) const override;
};

class ReturnStatement : public Statement {
public:
    std::unique_ptr<Exp> expression;

    explicit ReturnStatement(std::unique_ptr<Exp> expr)
        : expression(std::move(expr)) {}

    void accept(Visitor& visitor) const override;
};

class Parameter {
public:
    std::string name;
    
    explicit Parameter(std::string n) : name(std::move(n)) {}
};

class FunctionDeclaration : public Statement {
public:
    std::string name;
    std::vector<Parameter> parameters;
    std::unique_ptr<BlockStatement> body;
    
    FunctionDeclaration(std::string n, std::vector<Parameter> params, std::unique_ptr<BlockStatement> b)
        : name(std::move(n)), parameters(std::move(params)), body(std::move(b)) {}
    
    void accept(Visitor& visitor) const override;
};

class FunctionCall : public Exp {
public:
    std::string name;
    std::vector<std::unique_ptr<Exp>> arguments;
    
    FunctionCall(std::string n, std::vector<std::unique_ptr<Exp>> args)
        : name(std::move(n)), arguments(std::move(args)) {}
    
    void accept(Visitor& visitor) const override;
};
