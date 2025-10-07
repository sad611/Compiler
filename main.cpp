#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "token.h"
#include "lexer.h"
#include "parser.h"
#include "visitor.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Uso: " << argv[0] << " <arquivo.ci>" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo " << argv[1] << std::endl;
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source_code = buffer.str();

    try {
        Lexer lexer(source_code);
        std::vector<Token> tokens = lexer.tokenize();

        Parser parser(tokens);
        std::unique_ptr<Program> ast_root = parser.parse();

        std::cout << "Arvore Sintatica:" << std::endl;
        PrintVisitor printVisitor;
        ast_root->accept(printVisitor);
        std::cout << std::endl;

        std::ofstream output_file("program.s");
        if (output_file.is_open()) {
            output_file << std::endl;
            
            std::streambuf* orig = std::cout.rdbuf();
            std::cout.rdbuf(output_file.rdbuf());
            
            CodeGenerationVisitor codeGenVisitor;
            ast_root->accept(codeGenVisitor);
            
            std::cout << std::endl;
            std::cout << "call sair" << std::endl;
            std::cout << std::endl;
            std::cout << ".include \"runtime.s\"" << std::endl;
            
            std::cout.rdbuf(orig);

            output_file.close();
            std::cout << "Codigo assembly gerado em: program.s" << std::endl;
        } else {
            std::cerr << "Erro: Nao foi possivel criar arquivo program.s" << std::endl;
        }

    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
