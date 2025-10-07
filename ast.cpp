#include "ast.h"
#include "visitor.h"

std::string operadorToString(Operador op) {
    switch (op) {
        case Operador::SOMA: return "+";
        case Operador::SUB:  return "-";
        case Operador::MULT: return "*";
        case Operador::DIV:  return "/";
        default: return "?";
    }
}

std::string comparisonOperatorToString(ComparisonOperator op) {
    switch (op) {
        case ComparisonOperator::EQUAL: return "==";
        case ComparisonOperator::NOT_EQUAL: return "!=";
        case ComparisonOperator::LESS: return "<";
        case ComparisonOperator::GREATER: return ">";
        case ComparisonOperator::LESS_EQUAL: return "<=";
        case ComparisonOperator::GREATER_EQUAL: return ">=";
        default: return "?";
    }
}

std::string logicalOperatorToString(LogicalOperator op) {
    switch (op) {
        case LogicalOperator::AND: return "&&";
        case LogicalOperator::OR: return "||";
        default: return "?";
    }
}

void Program::accept(Visitor& visitor) const {
    visitor.visit(*this);
}

void BlockStatement::accept(Visitor& visitor) const {
    visitor.visit(*this);
}

void MainFunction::accept(Visitor& visitor) const {
    visitor.visit(*this);
}

void ExpressionStatement::accept(Visitor& visitor) const {
    visitor.visit(*this);
}

void VarDeclaration::accept(Visitor& visitor) const {
    visitor.visit(*this);
}

void Const::accept(Visitor& visitor) const {
    visitor.visit(*this);
}

void BooleanLiteral::accept(Visitor& visitor) const {
    visitor.visit(*this);
}

void Variable::accept(Visitor& visitor) const {
    visitor.visit(*this);
}

void OpBin::accept(Visitor& visitor) const {
    visitor.visit(*this);
}

void ComparisonExpression::accept(Visitor& visitor) const {
    visitor.visit(*this);
}

void LogicalExpression::accept(Visitor& visitor) const {
    visitor.visit(*this);
}

void UnaryExpression::accept(Visitor& visitor) const {
    visitor.visit(*this);
}

void AssignmentExpression::accept(Visitor& visitor) const {
    visitor.visit(*this);
}

void IfStatement::accept(Visitor& visitor) const {
    visitor.visit(*this);
}

void WhileStatement::accept(Visitor& visitor) const {
    visitor.visit(*this);
}

void ReturnStatement::accept(Visitor& visitor) const {
    visitor.visit(*this);
}

void FunctionDeclaration::accept(Visitor& visitor) const {
    visitor.visit(*this);
}

void FunctionCall::accept(Visitor& visitor) const {
    visitor.visit(*this);
}
