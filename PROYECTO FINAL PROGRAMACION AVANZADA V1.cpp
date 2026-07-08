#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

// Definicion de los codigos de operacion
const int READ = 10;
const int WRITE = 11;
const int NEWLINE = 12;        // MEJORA 5: Salida de nueva linea
const int READ_STRING = 13;    // MEJORA 6: Entrada de cadenas (ASCII)
const int WRITE_STRING = 14;   // MEJORA 7: Salida de cadenas (ASCII)
const int LOAD = 20;
const int STORE = 21;
const int ADD = 30;
const int SUBTRACT = 31;
const int DIVIDE = 32;
const int MULTIPLY = 33;
const int MODULUS = 34;        // MEJORA 3: Residuo
const int EXPONENT = 35;       // MEJORA 4: Exponenciacion
const int BRANCH = 40;
const int BRANCHNEG = 41;
const int BRANCHZERO = 42;
const int HALT = 43;

class Simpletron {
private:
    int memory[1000]; 
    int accumulator;
    int instructionCounter;
    int instructionRegister;
    int operationCode;
    int operand;

    // Funcion para imprimir el volcado de memoria ajustado
    void dump() {
        cout << "\nRegistros:\n";
        cout << left << setw(23) << "acumulador:" << right << showpos << setfill('0') << internal << setw(6) << accumulator << noshowpos << setfill(' ') << endl;
        cout << left << setw(23) << "instructionCounter:" << right << setfill('0') << setw(3) << instructionCounter << setfill(' ') << endl;
        cout << left << setw(23) << "instructionRegister:" << right << showpos << setfill('0') << internal << setw(6) << instructionRegister << noshowpos << setfill(' ') << endl;
        cout << left << setw(23) << "operationcode:" << right << setfill('0') << setw(2) << operationCode << setfill(' ') << endl;
        cout << left << setw(23) << "operand:" << right << setfill('0') << setw(3) << operand << setfill(' ') << endl;

        cout << "\nMEMORIA:\n";
        cout << "    ";
        for (int i = 0; i < 10; i++) {
            cout << right << setw(6) << i << " ";
        }
        cout << endl;

        for (int i = 0; i < 1000; i += 10) { 
            cout << setfill('0') << setw(3) << i << " " << setfill(' ');
            for (int j = 0; j < 10; j++) {
                cout << showpos << setfill('0') << internal << setw(6) << memory[i + j] << " " << noshowpos << setfill(' ');
            }
            cout << endl;
        }
    }

public:
    // Constructor para inicializar todo en 0
    Simpletron() {
        for (int i = 0; i < 1000; i++) { 
            memory[i] = 0;
        }
        accumulator = 0;
        instructionCounter = 0;
        instructionRegister = 0;
        operationCode = 0;
        operand = 0;
    }

    // Cargar el programa en memoria
    void loadProgram() {
        ifstream file("programa.simp");
        
        if (file.is_open()) {
            cout << "*** Cargando programa desde archivo 'programa.simp' ***\n";
            int instruction;
            int index = 0;
            
            while (file >> instruction && index < 1000) { 
                if (instruction == -99999 || instruction == 99999) { 
                    break;
                }
                
                if (instruction < -99999 || instruction > 99999) {
                    cout << "Error: Instruccion invalida (" << instruction << ") ignorada en el archivo.\n";
                    continue; 
                }

                memory[index] = instruction;
                index++;
            }
            file.close();
            cout << "*** Se termino de cargar el programa desde el archivo ***\n\n";
        } else {
            cout << "*** Archivo 'programa.simp' no encontrado. Iniciando modo manual... ***\n";
            cout << "*** ¡Bienvenido a Simpletron! ***\n";
            cout << "*** Introduzca su programa una instruccion ***\n";
            cout << "*** (o palabra de datos) a la vez en la linea ***\n";
            cout << "*** de texto de entrada. Yo indicare el numero ***\n";
            cout << "*** de posicion y una interrogacion (?). Usted ***\n";
            cout << "*** tecleara entonces la palabra para esa ***\n";
            cout << "*** posicion. Teclee -99999 para ***\n";
            cout << "*** dejar de introducir su programa. ***\n\n";

            int instruction;
            int index = 0;

            while (index < 1000) { 
                cout << setfill('0') << setw(3) << index << " ? "; 
                cin >> instruction;

                if (instruction == -99999 || instruction == 99999) { 
                    break;
                }

                if (instruction < -99999 || instruction > 99999) {
                    cout << "Error: El numero debe estar entre -99999 y +99999. Intente de nuevo.\n";
                    continue; 
                }

                memory[index] = instruction;
                index++;
            }

            cout << "*** Se termino de cargar el programa ***\n";
        }
    }

    // Ejecutar el programa
    void executeProgram() {
        cout << "*** Comienza la ejecucion del programa ***\n";
        bool isHalted = false;
        bool isFatalError = false;

        while (!isHalted && !isFatalError && instructionCounter < 1000) {
            instructionRegister = memory[instructionCounter];
            
            operationCode = instructionRegister / 1000;
            operand = instructionRegister % 1000;

            long long int temp;

            switch (operationCode) {
                case READ:
                    cout << "? ";
                    cin >> temp;
                    if (temp < -99999 || temp > 99999) { 
                        cout << "Entrada invalida. Debe estar entre -99999 y +99999.\n";
                        return; 
                    }
                    memory[operand] = temp;
                    instructionCounter++;
                    break;

                case WRITE:
                    cout << memory[operand] << endl;
                    instructionCounter++;
                    break;

                case NEWLINE:
                    cout << "\n";
                    instructionCounter++;
                    break;

                case READ_STRING: {
                    cout << "string? ";
                    string str;
                    
                    char ch;
                    while (cin.peek() == ' ' || cin.peek() == '\t') {
                        cin.get(ch);
                    }
                    if (cin.peek() == '\n') {
                        cin.get(ch);
                    }
                    
                    getline(cin, str);
                    
                    if (str.length() > 99) {
                        str = str.substr(0, 99);
                    }
                    
                    if (operand + str.length() >= 1000) {
                        cout << "*** Error Fatal: La longitud de la cadena excede el limite de memoria disponible ***\n";
                        isFatalError = true;
                        break;
                    }
                    
                    memory[operand] = static_cast<int>(str.length()) * 1000;
                    
                    for (size_t i = 0; i < str.length(); i++) {
                        int charIndex = static_cast<int>(i + 1);
                        int asciiVal = static_cast<int>(str[i]);
                        memory[operand + 1 + i] = (charIndex * 1000) + asciiVal;
                    }
                    
                    instructionCounter++;
                    break;
                }

                // MEJORA 7: Operacion Salida de Cadenas
                case WRITE_STRING: {
                    // Validacion controlada de rango del operando base
                    if (operand < 0 || operand >= 1000) {
                        cout << "*** Error: Direccion de memoria base para cadena fuera de rango (" << operand << ") ***\n";
                        isFatalError = true;
                        break;
                    }

                    int rawValue = memory[operand];
                    int length = rawValue / 1000; // Extrae la longitud (dos primeros digitos XX)

                    // Error controlado si los datos de longitud estan corruptos o exceden el mapeo de memoria
                    if (length < 0 || operand + length >= 1000) {
                        cout << "*** Error Controlado: Estructura de cadena corrupta o excede limites en direccion " << operand << " ***\n";
                        instructionCounter++; // Continua con la ejecucion para evitar colapso completo catastrophico
                        break;
                    }

                    // Impresion secuencial traduciendo de ASCII a caracter
                    for (int i = 0; i < length; i++) {
                        int word = memory[operand + 1 + i];
                        int asciiVal = word % 1000; // Obtiene el equivalente decimal ASCII (ultimos 3 digitos YYY)
                        cout << static_cast<char>(asciiVal);
                    }
                    
                    instructionCounter++;
                    break;
                }

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
                    if (temp > 99999 || temp < -99999) { 
                        cout << "*** Desbordamiento del acumulador ***\n";
                        isFatalError = true;
                    } else {
                        accumulator = temp;
                        instructionCounter++;
                    }
                    break;

                case SUBTRACT:
                    temp = accumulator - memory[operand];
                    if (temp > 99999 || temp < -99999) {
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
                    if (temp > 99999 || temp < -99999) {
                        cout << "*** Desbordamiento del acumulador ***\n";
                        isFatalError = true;
                    } else {
                        accumulator = temp;
                        instructionCounter++;
                    }
                    break;

                case MODULUS:
                    if (memory[operand] == 0) {
                        cout << "*** Intento de dividir entre cero (modulo) ***\n";
                        isFatalError = true;
                    } else {
                        accumulator %= memory[operand];
                        instructionCounter++;
                    }
                    break;

                case EXPONENT: {
                    int base = accumulator;
                    int exp = memory[operand];
                    
                    if (exp < 0) {
                        cout << "*** Error: Exponente negativo no soportado en modo entero ***\n";
                        isFatalError = true;
                    } else if (exp == 0) {
                        accumulator = 1;
                        instructionCounter++;
                    } else if (exp == 1) {
                        instructionCounter++;
                    } else {
                        long long int result = 1;
                        bool overflow = false;
                        
                        for (int i = 0; i < exp; i++) {
                            result *= base;
                            if (result > 99999 || result < -99999) {
                                overflow = true;
                                break;
                            }
                        }
                        
                        if (overflow) {
                            cout << "*** Desbordamiento del acumulador en exponenciacion ***\n";
                            isFatalError = true;
                        } else {
                            accumulator = static_cast<int>(result);
                            instructionCounter++;
                        }
                    }
                    break;
                }

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
                    if (operationCode != 0) {
                        cout << "*** Intento de ejecutar un codigo de operacion invalido ***\n";
                        isFatalError = true;
                    } else {
                        instructionCounter++; 
                    }
                    break;
            }
        }

        if (isFatalError) {
            cout << "*** La ejecucion de Simpletron termino anormalmente ***\n";
        }
        
        dump();
    }
};

int main() {
    Simpletron myComputer;
    myComputer.loadProgram();
    myComputer.executeProgram();
    
    return 0;
}