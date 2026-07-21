#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <string.h> 
#include <stdlib.h> 
#include <stack> // Agregado para procesar las expresiones infijas/posfijas

using namespace std;

// --- Códigos de operación del Simpletron ---
const int READ = 10;
const int WRITE = 11;
const int LOAD = 20;
const int STORE = 21;
const int ADD = 30;
const int SUBTRACT = 31;
const int DIVIDE = 32;
const int MULTIPLY = 33;
const int BRANCH = 40;
const int BRANCHNEG = 41;
const int BRANCHZERO = 42;
const int HALT = 43;

// --- Estructura de la Tabla de Símbolos ---
struct TableEntry {
    int symbol;     
    char type;      
    int location;   
};

class SimpleCompiler {
private:
    int memory[100];        
    int flags[100];         
    vector<TableEntry> symbolTable; 
    int instructionCounter; 
    int dataCounter;        

    int checkSymbolTable(int symbol, char type) {
        for (const auto& entry : symbolTable) {
            if (entry.symbol == symbol && entry.type == type) {
                return entry.location;
            }
        }
        return -1;
    }

    int insertSymbol(int symbol, char type) {
        int loc = checkSymbolTable(symbol, type);
        if (loc != -1) return loc; 

        TableEntry newEntry;
        newEntry.symbol = symbol;
        newEntry.type = type;

        if (type == 'L') {
            newEntry.location = instructionCounter;
        } else {
            newEntry.location = dataCounter--;
            if (type == 'C') memory[newEntry.location] = symbol; 
        }
        
        symbolTable.push_back(newEntry);
        return newEntry.location;
    }

    bool isNumber(const char* s) {
        if (s == nullptr || s[0] == '\0') return false;
        for (int i = 0; s[i] != '\0'; i++) {
            if (!isdigit(s[i]) && s[i] != '-') return false;
        }
        return true;
    }

    bool isOperator(const string& token) {
        return token == "+" || token == "-" || token == "*" || token == "/";
    }

    int getPrecedence(const string& op) {
        if (op == "*" || op == "/") return 2;
        if (op == "+" || op == "-") return 1;
        return 0;
    }

    // Generación de código para instrucciones LET usando Pilas y Posfija
    void handleLet(char* variableToken, char* exprRest) {
        int destLoc = insertSymbol(variableToken[0], 'V');
        
        // 1. Extraer los tokens de la expresión original
        vector<string> infixTokens;
        char* exprToken = strtok(exprRest, " ");
        while (exprToken != NULL) {
            infixTokens.push_back(exprToken);
            exprToken = strtok(NULL, " ");
        }

        // 2. Convertir de Infija a Posfija (Algoritmo Shunting Yard simplificado)
        vector<string> postfix;
        stack<string> opStack;
        
        for (const string& token : infixTokens) {
            if (!isOperator(token)) {
                postfix.push_back(token); // Es un operando (variable o numero)
            } else {
                while (!opStack.empty() && getPrecedence(opStack.top()) >= getPrecedence(token)) {
                    postfix.push_back(opStack.top());
                    opStack.pop();
                }
                opStack.push(token);
            }
        }
        while (!opStack.empty()) {
            postfix.push_back(opStack.top());
            opStack.pop();
        }

        // 3. Evaluar la expresión Posfija (El "Gancho" para generar SML)
        stack<int> evalStack;
        
        for (const string& token : postfix) {
            if (!isOperator(token)) {
                // Es un operando: buscar/insertar en tabla y meter su ubicación en la pila
                int loc = isNumber(token.c_str()) ? 
                          insertSymbol(atoi(token.c_str()), 'C') : 
                          insertSymbol(token[0], 'V');
                evalStack.push(loc);
            } else {
                // Es un operador: sacar los dos operandos de la pila y generar instrucciones
                int rightLoc = evalStack.top(); evalStack.pop();
                int leftLoc = evalStack.top(); evalStack.pop();
                
                memory[instructionCounter++] = LOAD * 100 + leftLoc;
                
                if (token == "+") memory[instructionCounter++] = ADD * 100 + rightLoc;
                else if (token == "-") memory[instructionCounter++] = SUBTRACT * 100 + rightLoc;
                else if (token == "*") memory[instructionCounter++] = MULTIPLY * 100 + rightLoc;
                else if (token == "/") memory[instructionCounter++] = DIVIDE * 100 + rightLoc;
                
                // Guardar el resultado en una ubicación temporal y meterla a la pila
                int tempLoc = dataCounter--; 
                memory[instructionCounter++] = STORE * 100 + tempLoc;
                evalStack.push(tempLoc);
            }
        }

        // 4. El último valor en la pila es el resultado final. Asignarlo a la variable de la izquierda
        if (!evalStack.empty()) {
            int finalResultLoc = evalStack.top();
            evalStack.pop();
            memory[instructionCounter++] = LOAD * 100 + finalResultLoc;
            memory[instructionCounter++] = STORE * 100 + destLoc;
        }
    }

public:
    SimpleCompiler() {
        for (int i = 0; i < 100; i++) {
            memory[i] = 0;
            flags[i] = -1; 
        }
        instructionCounter = 0;
        dataCounter = 99;
    }

    void pass1(const string& filename) {
        ifstream inFile(filename);
        if (!inFile) {
            cerr << "Error al abrir el archivo de codigo Simple." << endl;
            return;
        }

        string lineStr;
        while (getline(inFile, lineStr)) {
            if (lineStr.empty()) continue;

            char line[256];
            strcpy(line, lineStr.c_str());

            char* token = strtok(line, " ");
            if (token == NULL) continue;
            int lineNumber = atoi(token);
            insertSymbol(lineNumber, 'L');

            token = strtok(NULL, " ");
            if (token == NULL) continue;
            string command(token);

            if (command == "rem") {
                continue; 
            } 
            else if (command == "input") {
                token = strtok(NULL, " ");
                if (token) {
                    int loc = insertSymbol(token[0], 'V');
                    memory[instructionCounter++] = READ * 100 + loc;
                }
            } 
            else if (command == "print") {
                token = strtok(NULL, " ");
                if (token) {
                    int loc = insertSymbol(token[0], 'V');
                    memory[instructionCounter++] = WRITE * 100 + loc;
                }
            } 
            else if (command == "goto") {
                token = strtok(NULL, " ");
                if (token) {
                    int targetLine = atoi(token);
                    int loc = checkSymbolTable(targetLine, 'L');
                    if (loc != -1) {
                        memory[instructionCounter++] = BRANCH * 100 + loc;
                    } else {
                        flags[instructionCounter] = targetLine;
                        memory[instructionCounter++] = BRANCH * 100 + 0;
                    }
                }
            } 
            else if (command == "if") {
                char* op1 = strtok(NULL, " ");
                char* comp = strtok(NULL, " ");
                char* op2 = strtok(NULL, " ");
                strtok(NULL, " "); 
                char* targetStr = strtok(NULL, " ");

                if (op1 && comp && op2 && targetStr) {
                    int targetLine = atoi(targetStr);
                    int loc1 = isNumber(op1) ? insertSymbol(atoi(op1), 'C') : insertSymbol(op1[0], 'V');
                    int loc2 = isNumber(op2) ? insertSymbol(atoi(op2), 'C') : insertSymbol(op2[0], 'V');

                    memory[instructionCounter++] = LOAD * 100 + loc1;
                    memory[instructionCounter++] = SUBTRACT * 100 + loc2;

                    int targetLoc = checkSymbolTable(targetLine, 'L');
                    int branchInstruction = (string(comp) == "==") ? BRANCHZERO : BRANCHNEG; 

                    if (targetLoc != -1) {
                        memory[instructionCounter++] = branchInstruction * 100 + targetLoc;
                    } else {
                        flags[instructionCounter] = targetLine;
                        memory[instructionCounter++] = branchInstruction * 100 + 0;
                    }
                }
            } 
            else if (command == "let") {
                char* varToken = strtok(NULL, " ");
                strtok(NULL, " "); 
                char* exprRest = strtok(NULL, ""); 
                if (varToken && exprRest) {
                    handleLet(varToken, exprRest);
                }
            } 
            else if (command == "end") {
                memory[instructionCounter++] = HALT * 100;
            }
        }
        inFile.close();
    }

    void pass2() {
        for (int i = 0; i < 100; i++) {
            if (flags[i] != -1) {
                int unresolvedLine = flags[i];
                int actualLocation = checkSymbolTable(unresolvedLine, 'L');
                
                if (actualLocation != -1) {
                    memory[i] += actualLocation; 
                } else {
                    cerr << "Error de compilacion: Linea no existente " << unresolvedLine << endl;
                }
            }
        }
    }

    void outputSML(const string& filename) {
        ofstream outFile(filename);
        for (int i = 0; i < instructionCounter; i++) {
            outFile << setfill('0') << setw(2) << i << " " 
                    << showpos << setfill('0') << setw(4) << memory[i] << noshowpos << endl;
        }
        outFile.close();
        cout << "Compilacion exitosa. Archivo generado para Simpletron: " << filename << endl;
    }
};

int main() {
    SimpleCompiler compiler;
    
    // Nombres de archivos (asegurate de tener tu archivo .simple listo en la misma carpeta)
    string inputFile = "programa1.simple";
    string outputFile = "programa1.sml";

    cout << "--- Compilador de Lenguaje Simple ---" << endl;
    compiler.pass1(inputFile);
    compiler.pass2();
    compiler.outputSML(outputFile);

    return 0;
}