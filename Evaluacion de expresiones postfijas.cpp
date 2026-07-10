#include <iostream>
#include <stack>
#include <string>
#include <cmath>  // Para la funcion pow()
#include <cctype> // Para la funcion isdigit()

using namespace std;

// Función que evalúa la expresión en postfijo
int evaluarPostfijo(string expresion) {
    stack<int> p; // Pila para guardar los operandos

    // 2. Ciclo para recorrer la cadena
    for (size_t i = 0; i < expresion.length(); i++) {
        char symb = expresion[i];

        // 3. Si es un dígito
        if (isdigit(symb)) {
            // Convertimos el char a entero restando el valor ASCII de '0'
            p.push(symb - '0'); 
        } 
        // 4. Si es un operador
        else if (symb == '+' || symb == '-' || symb == '*' || symb == '/' || symb == '^') {
            // 4.1. Hacer pop al primer elemento y guardarlo en op2
            int op2 = p.top();
            p.pop();

            // 4.2. Hacer pop al siguiente elemento y guardarlo en op1
            int op1 = p.top();
            p.pop();

            int res = 0;

            // 4.3. Evaluar la expresión: op1 operador op2
            switch (symb) {
                case '+': res = op1 + op2; break;
                case '-': res = op1 - op2; break;
                case '*': res = op1 * op2; break;
                case '/': res = op1 / op2; break;
                case '^': res = pow(op1, op2); break;
            }

            // 4.4. Hacer push del resultado a la pila
            p.push(res);
        }
    }

    // 5. Al finalizar la cadena, hacer pop al tope de la pila y guardarlo en resultado
    int resultado = p.top();
    p.pop();

    return resultado;
}

int main() {
    string expresionUsuario;

    cout << "--- EVALUADOR DE EXPRESIONES POSTFIJAS ---" << endl;
    cout << "Por favor, ingrese la expresion postfija a evaluar (sin espacios): ";
    cin >> expresionUsuario;

    cout << "\nEvaluando la expresion..." << endl;
    
    // Llamamos a la funcion y mostramos el resultado
    int resultadoFinal = evaluarPostfijo(expresionUsuario);
    
    cout << ">> Resultado final: " << resultadoFinal << " <<" << endl;
    cout << "------------------------------------------" << endl;

    return 0;
}