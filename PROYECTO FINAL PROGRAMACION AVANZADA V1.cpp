#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

// Definición de los códigos de operación (Lenguaje de Máquina Simpletron - SML)
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

class Simpletron {
private:
    int memory[100];
    int accumulator;
    int instructionCounter;
    int instructionRegister;
    int operationCode;
    int operand;

    // Función para imprimir el volcado de memoria (Memory Dump)
    void dump() {
        cout << "\nRegistros:\n";
        cout << left << setw(23) << "acumulador:" << right << showpos << setfill('0') << internal << setw(5) << accumulator << noshowpos << setfill(' ') << endl;
        cout << left << setw(23) << "instructionCounter:" << right << setfill('0') << setw(2) << instructionCounter << setfill(' ') << endl;
        cout << left << setw(23) << "instructionRegister:" << right << showpos << setfill('0') << internal << setw(5) << instructionRegister << noshowpos << setfill(' ') << endl;
        cout << left << setw(23) << "operationcode:" << right << setfill('0') << setw(2) << operationCode << setfill(' ') << endl;
        cout << left << setw(23) << "operand:" << right << setfill('0') << setw(2) << operand << setfill(' ') << endl;

        cout << "\nMEMORIA:\n";
        cout << "  ";
        for (int i = 0; i < 10; i++) {
            cout << right << setw(6) << i;
        }
        cout << endl;

        for (int i = 0; i < 100; i += 10) {
            cout << setw(2) << i / 10 << " ";
            for (int j = 0; j < 10; j++) {
                cout << showpos << setfill('0') << internal << setw(5) << memory[i + j] << " " << noshowpos << setfill(' ');
            }
            cout << endl;
        }
    }

public:
    // Constructor para inicializar todo en 0
    Simpletron() {
        for (int i = 0; i < 100; i++) {
            memory[i] = 0;
        }
        accumulator = 0;
        instructionCounter = 0;
        instructionRegister = 0;
        operationCode = 0;
        operand = 0;
    }

    // Fase 1: Cargar el programa en memoria
    void loadProgram() {
        cout << "*** ¡Bienvenido a Simpletron! ***\n";
        cout << "*** Introduzca su programa una instruccion ***\n";
        cout << "*** (o palabra de datos) a la vez en la linea ***\n";
        cout << "*** de texto de entrada. Yo indicare el numero ***\n";
        cout << "*** de posicion y una interrogacion (?). Usted ***\n";
        cout << "*** tecleara entonces la palabra para esa ***\n";
        cout << "*** posicion. Teclee -9999 para ***\n";
        cout << "*** dejar de introducir su programa. ***\n\n";

        int instruction;
        int index = 0;

        while (index < 100) {
            cout << setfill('0') << setw(2) << index << " ? ";
            cin >> instruction;

            if (instruction == -9999 || instruction == 9999) { // Aceptamos 9999 o -9999 como centinela
                break;
            }

            // Validar que el valor esté entre -9999 y +9999
            if (instruction < -9999 || instruction > 9999) {
                cout << "Error: El numero debe estar entre -9999 y +9999. Intente de nuevo.\n";
                continue; // Repite la misma posición
            }

            memory[index] = instruction;
            index++;
        }

        cout << "*** Se termino de cargar el programa ***\n";
    }

    // Fase 2: Ejecutar el programa
    void executeProgram() {
        cout << "*** Comienza la ejecucion del programa ***\n";
        bool isHalted = false;
        bool isFatalError = false;

        while (!isHalted && !isFatalError && instructionCounter < 100) {
            // Obtener instrucción
            instructionRegister = memory[instructionCounter];
            
            // Decodificar
            operationCode = instructionRegister / 100;
            operand = instructionRegister % 100;

            // Variables temporales para cálculos y validaciones
            int temp;

            switch (operationCode) {
                case READ:
                    cout << "? ";
                    cin >> temp;
                    if (temp < -9999 || temp > 9999) {
                        cout << "Entrada invalida. Debe estar entre -9999 y +9999.\n";
                        return; // O puedes pedir el número de nuevo, aquí simplemente abortamos
                    }
                    memory[operand] = temp;
                    instructionCounter++;
                    break;

                case WRITE:
                    cout << memory[operand] << endl;
                    instructionCounter++;
                    break;

                case LOAD:
                    accumulator = memory[operand];
                    instructionCounter++;
                    break;

                case STORE:
                    memory[operand] = accumulator;
                    instructionCounter++;
                    break;

                case ADD:
                    temp = accumulator + memory[operand];
                    if (temp > 9999 || temp < -9999) {
                        cout << "*** Desbordamiento del acumulador ***\n";
                        isFatalError = true;
                    } else {
                        accumulator = temp;
                        instructionCounter++;
                    }
                    break;

                case SUBTRACT:
                    temp = accumulator - memory[operand];
                    if (temp > 9999 || temp < -9999) {
                        cout << "*** Desbordamiento del acumulador ***\n";
                        isFatalError = true;
                    } else {
                        accumulator = temp;
                        instructionCounter++;
                    }
                    break;

                case DIVIDE:
                    if (memory[operand] == 0) {
                        cout << "*** Intento de dividir entre cero ***\n";
                        isFatalError = true;
                    } else {
                        accumulator /= memory[operand];
                        instructionCounter++;
                    }
                    break;

                case MULTIPLY:
                    temp = accumulator * memory[operand];
                    if (temp > 9999 || temp < -9999) {
                        cout << "*** Desbordamiento del acumulador ***\n";
                        isFatalError = true;
                    } else {
                        accumulator = temp;
                        instructionCounter++;
                    }
                    break;

                case BRANCH:
                    instructionCounter = operand;
                    break;

                case BRANCHNEG:
                    if (accumulator < 0) {
                        instructionCounter = operand;
                    } else {
                        instructionCounter++;
                    }
                    break;

                case BRANCHZERO:
                    if (accumulator == 0) {
                        instructionCounter = operand;
                    } else {
                        instructionCounter++;
                    }
                    break;

                case HALT:
                    cout << "*** Termino la ejecucion de Simpletron ***\n";
                    isHalted = true;
                    break;

                default:
                    cout << "*** Intento de ejecutar un codigo de operacion invalido ***\n";
                    isFatalError = true;
                    break;
            }
        }

        if (isFatalError) {
            cout << "*** La ejecucion de Simpletron termino anormalmente ***\n";
        }
        
        // Al final, ya sea por HALT o por error fatal, imprimimos el volcado de memoria
        dump();
    }
};

int main() {
    Simpletron myComputer;
    myComputer.loadProgram();
    myComputer.executeProgram();
    
    return 0;
}
