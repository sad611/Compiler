## Como Usar

### 1. Compilar o Compilador
```bash
g++ -std=c++17 -o compilador *.cpp
```

### 2. Compilar um Programa
```bash
./compiler meu_programa.ci
```

### 3. Executar o Assembly Gerado
```bash
as -64 program.s -o program.o
ld program.o -o program
./program
```
