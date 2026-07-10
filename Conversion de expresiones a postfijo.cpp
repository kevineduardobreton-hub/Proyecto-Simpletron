#include <iostream>
#include <stack>
#include <cstring>

using namespace std;

// ==========================================
// PROTOTIPOS DE LAS FUNCIONES (Según especificaciones)
// ==========================================
int prec(char op1, char op2);
void postfijo(char expresion[]);

// ==========================================
// FUNCIÓN PRINCIPAL (MAIN)
// ==========================================
int main() {
    char expresionUsuario[1000];

    cout << "--- CONVERTIDOR DE EXPRESIONES A POSTFIJO ---" << endl;
    cout << "Soporta operadores (+, -, *, /, ^) y agrupadores ((), [], {})" << endl;
    cout << "---------------------------------------------------------" << endl;
    
    cout << "Ingrese la expresion en formato infijo: ";
    cin.getline(expresionUsuario, 1000);

    cout << "Cadena convertida a postfijo: ";
    postfijo(expresionUsuario);
    cout << "\n---------------------------------------------------------" << endl;

    return 0;
}

// ==========================================
// DEFINICIÓN DE FUNCIÓN: prec
// Propósito: Determinar la precedencia de los operadores
// ==========================================
int prec(char op1, char op2) {
    // --- MODIFICACIONES DE LA FASE 2 (Signos de agrupamiento) ---
    
    // Regla: Si op2 es un signo de cierre, se deben remover operadores hasta el de apertura
    if (op2 == ')' || op2 == ']' || op2 == '}') {
        // prec('(', ')') es igual a falso para detener el ciclo
        if (op1 == '(' || op1 == '[' || op1 == '{') {
            return 0; // falso
        }
        return 1; // verdadero para cualquier otro operador en la pila
    }

    // Si op2 es un signo de apertura, no debe remover nada al llegar
    if (op2 == '(' || op2 == '[' || op2 == '{') {
        return 0; // falso
    }

    // Si op1 ya está en la pila y es de apertura, ningún operador entrante lo puede sacar
    if (op1 == '(' || op1 == '[' || op1 == '{') {
        return 0; // falso
    }

    // --- JERARQUÍA NORMAL DE LA FASE 1 ---
    int jer1 = 0;
    int jer2 = 0;

    // Determina jerarquía de op1
    if (op1 == '^') jer1 = 3;
    else if (op1 == '*' || op1 == '/') jer1 = 2;
    else if (op1 == '+' || op1 == '-') jer1 = 1;

    // Determina jerarquía de op2
    if (op2 == '^') jer2 = 3;
    else if (op2 == '*' || op2 == '/') jer2 = 2;
    else if (op2 == '+' || op2 == '-') jer2 = 1;

    // Regresa verdadero (1) si jer1 >= jer2 (asociatividad izquierda a derecha)
    if (jer1 >= jer2) {
        return 1;
    } else {
        return 0;
    }
}

// ==========================================
// DEFINICIÓN DE FUNCIÓN: postfijo
// Propósito: Convertir una expresión infija a postfija
// ==========================================
void postfijo(char expresion[]) {
    // Variables auxiliares requeridas por el documento de diseño
    stack<char> p;     // Pila para guardar los caracteres operador
    char symb;         // Caracter a analizar
    char topSymb;      // Espacio temporal para el tope de la pila
    int i = 0;         // Índice para la cadena expresión
    int j = 0;         // Índice para la cadena postfijo
    
    char resultado[1000] = {0}; // Cadena destino para almacenar el postfijo

    // 1. Nos posicionamos al inicio de la cadena a convertir (i = 0 ya inicializado)
    
    // 2. Mientras no hayamos leido toda la cadena
    while (expresion[i] != '\0') {
        
        // 2.1. Copiar el siguiente caracter en symb
        symb = expresion[i];

        // Omitir espacios en blanco para mantener la salida limpia
        if (symb == ' ') {
            i++;
            continue;
        }

        // 2.2. Si symb es un operando (Letras de la A-Z, a-z o números)
        if ((symb >= 'A' && symb <= 'Z') || (symb >= 'a' && symb <= 'z') || (symb >= '0' && symb <= '9')) {
            // 2.2.1. Agregar symb a la cadena postfijo
            resultado[j] = symb;
            // 2.2.2. Incrementar el indice de postfijo
            j++;
        } 
        // 2.3. Si no (Es un operador o signo de agrupamiento)
        else {
            // 2.3.1. Mientras (not isEmpty()) y prec( stacktop(), symb )
            while (!p.empty() && prec(p.top(), symb)) {
                // 2.3.1.1. topsymb = pop()
                topSymb = p.top();
                p.pop();
                // 2.3.1.2. Agregar topsymb a la cadena postfijo
                resultado[j] = topSymb;
                // 2.3.1.3. Incrementar el indice de postfijo
                j++;
            }

            // --- IF AGREGADO DE LA FASE 2 ---
            // Modificación para eliminar de la expresión postfija cualquier signo de agrupamiento
            if (symb == ')' || symb == ']' || symb == '}') {
                if (!p.empty()) {
                    p.pop(); // Remueve y descarta el signo de apertura correspondiente ('(', '[', '{')
                }
            } else {
                // 2.3.2. push(symb)
                p.push(symb);
            }
        }
        i++;
    }

    // 3. Mientras (not isEmpty())
    while (!p.empty()) {
        // 3.1. topsymb = pop()
        topSymb = p.top();
        p.pop();
        // 3.2. Agregar topsymb a la cadena postfijo
        resultado[j] = topSymb;
        // 3.3. Incrementar el indice de postfijo
        j++;
    }

    // Asegurar el fin de cadena en el resultado
    resultado[j] = '\0';

    // 4. imprimir cadena postfijo.
    cout << resultado;
}