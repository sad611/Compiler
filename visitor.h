#pragma once
#include <memory>
#include <string>
#include <vector>
#include <map>

class Const;
class BooleanLiteral;
class Variable;
class OpBin;
class ComparisonExpression;
class LogicalExpression;
class UnaryExpression;
class AssignmentExpression;
class IfStatement;
class WhileStatement;
class ReturnStatement;
class FunctionDeclaration;
class FunctionCall;
class Program;
class BlockStatement;
class MainFunction;
class ExpressionStatement;
class VarDeclaration;
enum class Operador;
enum class ComparisonOperator;
enum class LogicalOperator;

class Visitor {
public:
    virtual ~Visitor() = default;
    virtual void visit(const Program& node) = 0;
    virtual void visit(const BlockStatement& node) = 0;
    virtual void visit(const MainFunction& node) = 0;
    virtual void visit(const ExpressionStatement& node) = 0;
    virtual void visit(const VarDeclaration& node) = 0;
    virtual void visit(const IfStatement& node) = 0;
    virtual void visit(const WhileStatement& node) = 0;
    virtual void visit(const ReturnStatement& node) = 0;
    virtual void visit(const FunctionDeclaration& node) = 0;
    virtual void visit(const Const& node) = 0;
    virtual void visit(const BooleanLiteral& node) = 0;
    virtual void visit(const Variable& node) = 0;
    virtual void visit(const OpBin& node) = 0;
    virtual void visit(const ComparisonExpression& node) = 0;
    virtual void visit(const LogicalExpression& node) = 0;
    virtual void visit(const UnaryExpression& node) = 0;
    virtual void visit(const AssignmentExpression& node) = 0;
    virtual void visit(const FunctionCall& node) = 0;
};

class PrintVisitor : public Visitor {
private:
    int depth;

public:
    PrintVisitor(int d = 0) : depth(d) {}

    void visit(const Program& node) override;
    void visit(const BlockStatement& node) override;
    void visit(const MainFunction& node) override;
    void visit(const ExpressionStatement& node) override;
    void visit(const VarDeclaration& node) override;
    void visit(const IfStatement& node) override;
    void visit(const WhileStatement& node) override;
    void visit(const ReturnStatement& node) override;
    void visit(const FunctionDeclaration& node) override;
    void visit(const Const& node) override;
    void visit(const BooleanLiteral& node) override;
    void visit(const Variable& node) override;
    void visit(const OpBin& node) override;
    void visit(const ComparisonExpression& node) override;
    void visit(const LogicalExpression& node) override;
    void visit(const UnaryExpression& node) override;
    void visit(const AssignmentExpression& node) override;
    void visit(const FunctionCall& node) override;
};

class CodeGenerationVisitor : public Visitor {
private:
    std::vector<std::string> declaredVariables;
    std::vector<std::string> localVariables;
    std::map<std::string, int> parameterOffsets;
    std::map<std::string, int> localOffsets;
    int currentFunctionStackSize = 0;
    bool collectingVariables = false;
    bool isAssignmentExpression = false;
    bool insideFunction = false;
    bool hasReturn = false;
    int labelCounter = 0;

    std::string generateLabel(const std::string& prefix) {
        return prefix + std::to_string(labelCounter++);
    }

    void generateBSSSection();
    void generateTextSection(const Program& node);

public:
    void visit(const Program& node) override;
    void visit(const BlockStatement& node) override;
    void visit(const MainFunction& node) override;
    void visit(const ExpressionStatement& node) override;
    void visit(const VarDeclaration& node) override;
    void visit(const IfStatement& node) override;
    void visit(const WhileStatement& node) override;
    void visit(const ReturnStatement& node) override;
    void visit(const FunctionDeclaration& node) override;
    void visit(const Const& node) override;
    void visit(const BooleanLiteral& node) override;
    void visit(const Variable& node) override;
    void visit(const OpBin& node) override;
    void visit(const ComparisonExpression& node) override;
    void visit(const LogicalExpression& node) override;
    void visit(const UnaryExpression& node) override;
    void visit(const AssignmentExpression& node) override;
    void visit(const FunctionCall& node) override;
};