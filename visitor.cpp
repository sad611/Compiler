#include "visitor.h"
#include "ast.h"
#include <iostream>
#include <stdexcept>

void PrintVisitor::visit(const Program& node) {
    for (int i = 0; i < depth; i++) {
        std::cout << "  ";
    }
    std::cout << "Program" << std::endl;
    
    int declNum = 1;
    for (const auto& decl : node.globalDeclarations) {
        for (int i = 0; i < depth + 1; i++) {
            std::cout << "  ";
        }
        std::cout << "|- Global Declaration " << declNum << ":" << std::endl;
        
        PrintVisitor declVisitor(depth + 2);
        decl->accept(declVisitor);
        declNum++;
    }
    
    if (node.mainFunction) {
        for (int i = 0; i < depth + 1; i++) {
            std::cout << "  ";
        }
        std::cout << "|- Main Function:" << std::endl;
        
        PrintVisitor mainVisitor(depth + 2);
        node.mainFunction->accept(mainVisitor);
    }
}

void PrintVisitor::visit(const BlockStatement& node) {
    for (int i = 0; i < depth; i++) {
        std::cout << "  ";
    }
    std::cout << "BlockStatement" << std::endl;
    
    int stmtNum = 1;
    for (const auto& stmt : node.statements) {
        for (int i = 0; i < depth + 1; i++) {
            std::cout << "  ";
        }
        std::cout << "|- Statement " << stmtNum << ":" << std::endl;
        
        PrintVisitor stmtVisitor(depth + 2);
        stmt->accept(stmtVisitor);
        stmtNum++;
    }
}

void PrintVisitor::visit(const MainFunction& node) {
    for (int i = 0; i < depth; i++) {
        std::cout << "  ";
    }
    std::cout << "MainFunction()" << std::endl;
    
    PrintVisitor bodyVisitor(depth + 1);
    node.body->accept(bodyVisitor);
}

void PrintVisitor::visit(const ExpressionStatement& node) {
    for (int i = 0; i < depth; i++) {
        std::cout << "  ";
    }
    std::cout << "ExpressionStatement" << std::endl;
    
    PrintVisitor exprVisitor(depth + 1);
    node.expression->accept(exprVisitor);
}

void PrintVisitor::visit(const VarDeclaration& node) {
    for (int i = 0; i < depth; i++) {
        std::cout << "  ";
    }
    std::cout << "VarDeclaration(\"" << node.identifier << "\")" << std::endl;
    
    if (node.initializer) {
        for (int i = 0; i < depth + 1; i++) {
            std::cout << "  ";
        }
        std::cout << "|- Initializer:" << std::endl;
        
        PrintVisitor initVisitor(depth + 2);
        node.initializer->accept(initVisitor);
    }
}

void PrintVisitor::visit(const Variable& node) {
    for (int i = 0; i < depth; i++) {
        std::cout << "  ";
    }
    std::cout << "Variable(\"" << node.name << "\")" << std::endl;
}

void PrintVisitor::visit(const Const& node) {
    for (int i = 0; i < depth; i++) {
        std::cout << "  ";
    }
    std::cout << "Const(" << node.valor << ")" << std::endl;
}

void PrintVisitor::visit(const BooleanLiteral& node) {
    for (int i = 0; i < depth; i++) {
        std::cout << "  ";
    }
    std::cout << "Boolean(" << (node.value ? "true" : "false") << ")" << std::endl;
}

void PrintVisitor::visit(const OpBin& node) {
    for (int i = 0; i < depth; i++) {
        std::cout << "  ";
    }
    std::cout << "OpBin(" << operadorToString(node.op) << ")" << std::endl;
    
    for (int i = 0; i < depth + 1; i++) {
        std::cout << "  ";
    }
    std::cout << "|- Operando Esquerdo:" << std::endl;
    
    PrintVisitor leftVisitor(depth + 2);
    node.opEsq->accept(leftVisitor);
    
    for (int i = 0; i < depth + 1; i++) {
        std::cout << "  ";
    }
    std::cout << "|- Operando Direito:" << std::endl;
    
    PrintVisitor rightVisitor(depth + 2);
    node.opDir->accept(rightVisitor);
}

void PrintVisitor::visit(const IfStatement& node) {
    for (int i = 0; i < depth; i++) std::cout << "  ";
    std::cout << "IfStatement" << std::endl;
    
    for (int i = 0; i < depth; i++) std::cout << "  ";
    std::cout << "|- Condition:" << std::endl;
    PrintVisitor condVisitor(depth + 1);
    node.condition->accept(condVisitor);
    
    for (int i = 0; i < depth; i++) std::cout << "  ";
    std::cout << "|- Then:" << std::endl;
    PrintVisitor thenVisitor(depth + 1);
    node.thenBranch->accept(thenVisitor);
    
    if (node.elseBranch) {
        for (int i = 0; i < depth; i++) std::cout << "  ";
        std::cout << "|- Else:" << std::endl;
        PrintVisitor elseVisitor(depth + 1);
        node.elseBranch->accept(elseVisitor);
    }
}

void PrintVisitor::visit(const WhileStatement& node) {
    for (int i = 0; i < depth; i++) std::cout << "  ";
    std::cout << "WhileStatement" << std::endl;
    
    for (int i = 0; i < depth; i++) std::cout << "  ";
    std::cout << "|- Condition:" << std::endl;
    PrintVisitor condVisitor(depth + 1);
    node.condition->accept(condVisitor);
    
    for (int i = 0; i < depth; i++) std::cout << "  ";
    std::cout << "|- Body:" << std::endl;
    PrintVisitor bodyVisitor(depth + 1);
    node.body->accept(bodyVisitor);
}

void PrintVisitor::visit(const ComparisonExpression& node) {
    for (int i = 0; i < depth; i++) std::cout << "  ";
    std::cout << "Comparison(" << comparisonOperatorToString(node.op) << ")" << std::endl;
    
    for (int i = 0; i < depth; i++) std::cout << "  ";
    std::cout << "|- Left:" << std::endl;
    PrintVisitor leftVisitor(depth + 1);
    node.left->accept(leftVisitor);
    
    for (int i = 0; i < depth; i++) std::cout << "  ";
    std::cout << "|- Right:" << std::endl;
    PrintVisitor rightVisitor(depth + 1);
    node.right->accept(rightVisitor);
}

void PrintVisitor::visit(const LogicalExpression& node) {
    for (int i = 0; i < depth; i++) std::cout << "  ";
    std::cout << "Logical(" << logicalOperatorToString(node.op) << ")" << std::endl;
    
    for (int i = 0; i < depth; i++) std::cout << "  ";
    std::cout << "|- Left:" << std::endl;
    PrintVisitor leftVisitor(depth + 1);
    node.left->accept(leftVisitor);
    
    for (int i = 0; i < depth; i++) std::cout << "  ";
    std::cout << "|- Right:" << std::endl;
    PrintVisitor rightVisitor(depth + 1);
    node.right->accept(rightVisitor);
}

void PrintVisitor::visit(const UnaryExpression& node) {
    for (int i = 0; i < depth; i++) std::cout << "  ";
    std::cout << "Unary(" << (node.isNot ? "!" : "") << ")" << std::endl;
    
    for (int i = 0; i < depth; i++) std::cout << "  ";
    std::cout << "|- Operand:" << std::endl;
    PrintVisitor operandVisitor(depth + 1);
    node.operand->accept(operandVisitor);
}

void PrintVisitor::visit(const AssignmentExpression& node) {
    for (int i = 0; i < depth; i++) std::cout << "  ";
    std::cout << "Assignment(=)" << std::endl;
    
    for (int i = 0; i < depth; i++) std::cout << "  ";
    std::cout << "|- Variable: " << node.variable << std::endl;
    
    for (int i = 0; i < depth; i++) std::cout << "  ";
    std::cout << "|- Value:" << std::endl;
    PrintVisitor valueVisitor(depth + 1);
    node.value->accept(valueVisitor);
}

void PrintVisitor::visit(const ReturnStatement& node) {
    for (int i = 0; i < depth; ++i) std::cout << "  ";
    std::cout << "|- Return:" << std::endl;
    
    depth++;
    node.expression->accept(*this);
    depth--;
}

void PrintVisitor::visit(const FunctionDeclaration& node) {
    for (int i = 0; i < depth; i++) std::cout << "  ";
    std::cout << "FunctionDeclaration(\"" << node.name << "\")" << std::endl;
    
    for (int i = 0; i < depth + 1; i++) std::cout << "  ";
    std::cout << "|- Parameters:" << std::endl;
    
    for (size_t j = 0; j < node.parameters.size(); j++) {
        for (int i = 0; i < depth + 2; i++) std::cout << "  ";
        std::cout << "|- Parameter " << (j + 1) << ": " << node.parameters[j].name << std::endl;
    }
    
    for (int i = 0; i < depth + 1; i++) std::cout << "  ";
    std::cout << "|- Body:" << std::endl;
    PrintVisitor bodyVisitor(depth + 2);
    node.body->accept(bodyVisitor);
}

void PrintVisitor::visit(const FunctionCall& node) {
    for (int i = 0; i < depth; i++) std::cout << "  ";
    std::cout << "FunctionCall(\"" << node.name << "\")" << std::endl;
    
    for (int i = 0; i < depth + 1; i++) std::cout << "  ";
    std::cout << "|- Arguments:" << std::endl;
    
    for (size_t j = 0; j < node.arguments.size(); j++) {
        for (int i = 0; i < depth + 2; i++) std::cout << "  ";
        std::cout << "|- Argument " << (j + 1) << ":" << std::endl;
        PrintVisitor argVisitor(depth + 3);
        node.arguments[j]->accept(argVisitor);
    }
}

void CodeGenerationVisitor::visit(const Program& node) {
    collectingVariables = true;
    for (const auto& decl : node.globalDeclarations) {
        if (auto varDecl = dynamic_cast<const VarDeclaration*>(decl.get())) {
            declaredVariables.push_back(varDecl->identifier);
        } else if (auto funcDecl = dynamic_cast<const FunctionDeclaration*>(decl.get())) {
            funcDecl->accept(*this);
        }
    }

    if (!declaredVariables.empty()) {
        generateBSSSection();
        std::cout << std::endl;
    }
    collectingVariables = false;
    generateTextSection(node);
}

void CodeGenerationVisitor::generateBSSSection() {
    std::cout << ".section .bss" << std::endl;
    for (const auto& varName : declaredVariables) {
        std::cout << ".lcomm " << varName << ", 8" << std::endl;
    }
}

void CodeGenerationVisitor::generateTextSection(const Program& node) {
    collectingVariables = false;
    std::cout << ".section .text" << std::endl;
    std::cout << ".globl _start" << std::endl;
    std::cout << std::endl;
    
    for (const auto& decl : node.globalDeclarations) {
        if (auto funcDecl = dynamic_cast<const FunctionDeclaration*>(decl.get())) {
            funcDecl->accept(*this);
            std::cout << std::endl;
        }
    }
    
    std::cout << "_start:" << std::endl;
    
    for (const auto& decl : node.globalDeclarations) {
        if (auto varDecl = dynamic_cast<const VarDeclaration*>(decl.get())) {
            decl->accept(*this);
        }
    }
    
    if (node.mainFunction) {
        node.mainFunction->accept(*this);
    }
}

void CodeGenerationVisitor::visit(const BlockStatement& node) {
    for (const auto& stmt : node.statements) {
        stmt->accept(*this);
    }
}

void CodeGenerationVisitor::visit(const MainFunction& node) {
    node.body->accept(*this);
}

void CodeGenerationVisitor::visit(const ExpressionStatement& node) {
    isAssignmentExpression = false;
    node.expression->accept(*this);

    if (!isAssignmentExpression) {
        std::cout << "  call imprime_num" << std::endl;
    }
}

void CodeGenerationVisitor::visit(const VarDeclaration& node) {
    if (collectingVariables) return;
    
    if (insideFunction) {
        if (node.initializer) {
            node.initializer->accept(*this);
        } else {
            std::cout << "  mov $0, %rax" << std::endl;
        }
        
        int offset = localOffsets[node.identifier];
        std::cout << "  mov %rax, " << offset << "(%rbp)" << std::endl;
    } else {
        if (node.initializer) {
            node.initializer->accept(*this);
            std::cout << "  mov %rax, " << node.identifier << std::endl;
        } else {
            std::cout << "  mov $0, %rax" << std::endl;
            std::cout << "  mov %rax, " << node.identifier << std::endl;
        }
    }
}

void CodeGenerationVisitor::visit(const Variable& node) {
    if (insideFunction) {
        if (parameterOffsets.find(node.name) != parameterOffsets.end()) {
            int offset = parameterOffsets[node.name];
            std::cout << "  mov " << offset << "(%rbp), %rax" << std::endl;
            return;
        }
        
        if (localOffsets.find(node.name) != localOffsets.end()) {
            int offset = localOffsets[node.name];
            std::cout << "  mov " << offset << "(%rbp), %rax" << std::endl;
            return;
        }
    }
    
    std::cout << "  mov " << node.name << ", %rax" << std::endl;
}

void CodeGenerationVisitor::visit(const Const& node) {
    std::cout << "  mov $" << node.valor << ", %rax" << std::endl;
}

void CodeGenerationVisitor::visit(const BooleanLiteral& node) {
    std::cout << "  mov $" << (node.value ? 1 : 0) << ", %rax" << std::endl;
}

void CodeGenerationVisitor::visit(const OpBin& node) {
    node.opDir->accept(*this);
    std::cout << "  push %rax" << std::endl;
    
    node.opEsq->accept(*this);
    std::cout << "  pop %rbx" << std::endl;
    
    switch (node.op) {
        case Operador::SOMA:
            std::cout << "  add %rbx, %rax" << std::endl;
            break;
        case Operador::SUB:
            std::cout << "  sub %rbx, %rax" << std::endl;
            break;
        case Operador::MULT:
            std::cout << "  imul %rbx, %rax" << std::endl;
            break;
        case Operador::DIV:
            std::cout << "  cqo" << std::endl;
            std::cout << "  idiv %rbx" << std::endl;
            break;
        default:
            throw std::runtime_error("Operador desconhecido na geracao de codigo.");
    }
}

void CodeGenerationVisitor::visit(const IfStatement& node) {
    if (collectingVariables) return;
    
    std::string falseLabel = generateLabel("Lfalso");
    std::string endLabel = generateLabel("Lfim");
    
    node.condition->accept(*this);
    
    std::cout << "    cmp $0, %rax" << std::endl;
    std::cout << "    jz " << falseLabel << std::endl;
    
    node.thenBranch->accept(*this);
    
    if (node.elseBranch) {
        std::cout << "    jmp " << endLabel << std::endl;
        std::cout << falseLabel << ":" << std::endl;
        node.elseBranch->accept(*this);
        std::cout << endLabel << ":" << std::endl;
    } else {
        std::cout << falseLabel << ":" << std::endl;
    }
}

void CodeGenerationVisitor::visit(const WhileStatement& node) {
    if (collectingVariables) return;
    
    std::string loopLabel = generateLabel("Linicio");
    std::string endLabel = generateLabel("Lfim");
    
    std::cout << loopLabel << ":" << std::endl;
    
    node.condition->accept(*this);
    
    std::cout << "    cmp $0, %rax" << std::endl;
    std::cout << "    jz " << endLabel << std::endl;
    
    node.body->accept(*this);
    
    std::cout << "    jmp " << loopLabel << std::endl;
    
    std::cout << endLabel << ":" << std::endl;
}

void CodeGenerationVisitor::visit(const ComparisonExpression& node) {
    if (collectingVariables) return;
    
    node.left->accept(*this);
    std::cout << "    pushq %rax" << std::endl;
    
    node.right->accept(*this);
    std::cout << "    popq %rbx" << std::endl;
    
    std::cout << "    cmp %rax, %rbx" << std::endl;
    
    switch (node.op) {
        case ComparisonOperator::EQUAL:
            std::cout << "    sete %al" << std::endl;
            break;
        case ComparisonOperator::NOT_EQUAL:
            std::cout << "    setne %al" << std::endl;
            break;
        case ComparisonOperator::LESS:
            std::cout << "    setl %al" << std::endl;
            break;
        case ComparisonOperator::GREATER:
            std::cout << "    setg %al" << std::endl;
            break;
        case ComparisonOperator::LESS_EQUAL:
            std::cout << "    setle %al" << std::endl;
            break;
        case ComparisonOperator::GREATER_EQUAL:
            std::cout << "    setge %al" << std::endl;
            break;
    }
    
    std::cout << "    movzbl %al, %eax" << std::endl;
}

void CodeGenerationVisitor::visit(const LogicalExpression& node) {
    if (collectingVariables) return;
    
    std::string shortCircuitLabel = generateLabel("Lcircuit");
    std::string endLabel = generateLabel("Lend");
    
    node.left->accept(*this);
    
    if (node.op == LogicalOperator::OR) {
        std::cout << "    cmp $0, %rax" << std::endl;
        std::cout << "    jnz " << shortCircuitLabel << std::endl;
    } else {
        std::cout << "    cmp $0, %rax" << std::endl;
        std::cout << "    jz " << shortCircuitLabel << std::endl;
    }

    node.right->accept(*this);
    std::cout << "    jmp " << endLabel << std::endl;
    
    std::cout << shortCircuitLabel << ":" << std::endl;
    
    std::cout << endLabel << ":" << std::endl;
}

void CodeGenerationVisitor::visit(const UnaryExpression& node) {
    if (collectingVariables) return;
    
    node.operand->accept(*this);
    
    if (node.isNot) {
        std::cout << "    cmp $0, %rax" << std::endl;
        std::cout << "    sete %al" << std::endl;
        std::cout << "    movzbl %al, %eax" << std::endl;
    }
}

void CodeGenerationVisitor::visit(const AssignmentExpression& node) {
    if (collectingVariables) return;
    
    isAssignmentExpression = true;

    node.value->accept(*this);
    
    if (insideFunction) {
        if (parameterOffsets.find(node.variable) != parameterOffsets.end()) {
            int offset = parameterOffsets[node.variable];
            std::cout << "    mov %rax, " << offset << "(%rbp)" << std::endl;
            return;
        }

        if (localOffsets.find(node.variable) != localOffsets.end()) {
            int offset = localOffsets[node.variable];
            std::cout << "    mov %rax, " << offset << "(%rbp)" << std::endl;
            return;
        }
    }

    std::cout << "    mov %rax, " << node.variable << std::endl;
}

void CodeGenerationVisitor::visit(const ReturnStatement& node) {
    if (collectingVariables) return;
    
    node.expression->accept(*this);
    
    if (insideFunction) {
        if (currentFunctionStackSize > 0) {
            std::cout << "  add $" << currentFunctionStackSize << ", %rsp" << std::endl;
        }
        std::cout << "  pop %rbp" << std::endl;
        std::cout << "  ret" << std::endl;
        hasReturn = true;
    } else {
        std::cout << "  call imprime_num" << std::endl;
        std::cout << "call sair" << std::endl;
    }
}

void CodeGenerationVisitor::visit(const FunctionDeclaration& node) {
    if (collectingVariables) {
        localVariables.clear();
        parameterOffsets.clear();
        localOffsets.clear();
        currentFunctionStackSize = 0;

        for (const auto& stmt : node.body->statements) {
            if (auto varDecl = dynamic_cast<const VarDeclaration*>(stmt.get())) {
                localVariables.push_back(varDecl->identifier);
            }
        }

        int paramOffset = 16;
        for (const auto& param : node.parameters) {
            parameterOffsets[param.name] = paramOffset;
            paramOffset += 8;
        }
        
        int localOffset = -8;
        for (const auto& localVar : localVariables) {
            localOffsets[localVar] = localOffset;
            localOffset -= 8;
            currentFunctionStackSize += 8;
        }
        return;
    }
    
    auto savedLocalVariables = localVariables;
    auto savedParameterOffsets = parameterOffsets;
    auto savedLocalOffsets = localOffsets;
    auto savedFunctionStackSize = currentFunctionStackSize;

    localVariables.clear();
    parameterOffsets.clear();
    localOffsets.clear();
    currentFunctionStackSize = 0;

    for (const auto& stmt : node.body->statements) {
        if (auto varDecl = dynamic_cast<const VarDeclaration*>(stmt.get())) {
            localVariables.push_back(varDecl->identifier);
        }
    }
    
    int paramOffset = 16;
    for (const auto& param : node.parameters) {
        parameterOffsets[param.name] = paramOffset;
        paramOffset += 8;
    }
    
    int localOffset = -8;
    for (const auto& localVar : localVariables) {
        localOffsets[localVar] = localOffset;
        localOffset -= 8;
        currentFunctionStackSize += 8;
    }

    std::cout << node.name << ":" << std::endl;
    
    std::cout << "  push %rbp" << std::endl;
    std::cout << "  mov %rsp, %rbp" << std::endl;
    if (currentFunctionStackSize > 0) {
        std::cout << "  sub $" << currentFunctionStackSize << ", %rsp" << std::endl;
    }
    
    insideFunction = true;
    hasReturn = false;

    node.body->accept(*this);
    
    insideFunction = false;

    if (!hasReturn) {
        std::cout << "  mov $0, %rax" << std::endl;
        if (currentFunctionStackSize > 0) {
            std::cout << "  add $" << currentFunctionStackSize << ", %rsp" << std::endl;
        }
        std::cout << "  pop %rbp" << std::endl;
        std::cout << "  ret" << std::endl;
    }
    
    localVariables = savedLocalVariables;
    parameterOffsets = savedParameterOffsets;
    localOffsets = savedLocalOffsets;
    currentFunctionStackSize = savedFunctionStackSize;
}

void CodeGenerationVisitor::visit(const FunctionCall& node) {
    if (collectingVariables) return;

    for (int i = node.arguments.size() - 1; i >= 0; i--) {
        node.arguments[i]->accept(*this);
        std::cout << "  push %rax" << std::endl;
    }
    
    std::cout << "  call " << node.name << std::endl;
    
    if (!node.arguments.empty()) {
        std::cout << "  add $" << (node.arguments.size() * 8) << ", %rsp" << std::endl;
    }
}