#include <iostream>
#include <string>
#include <stack>

using namespace std;

// La función de validación se queda exactamente igual
bool verificarAgrupamiento(const string& expresion) {
    stack<char> pila;
    int profundidad = 0;

    for (size_t i = 0; i < expresion.length(); i++) {
        char c = expresion[i];

        if (c == '(' || c == '[' || c == '{') {
            pila.push(c);
            profundidad++;
        } 
        else if (c == ')' || c == ']' || c == '}') {
            if (pila.empty()) {
                cout << "Error: Se encontro '" << c << "' sin un simbolo de apertura previo." << endl;
                return false;
            }

            char tope = pila.top();
            
            if ((c == ')' && tope == '(') ||
                (c == ']' && tope == '[') ||
                (c == '}' && tope == '{')) {
                pila.pop();
                profundidad--;
            } else {
                cout << "Error: Se esperaba cerrar '" << tope << "' pero se encontro '" << c << "'." << endl;
                return false;
            }
        }
    }

    if (!pila.empty()) {
        cout << "Error: La expresion termino pero quedaron simbolos sin cerrar." << endl;
        return false;
    }

    return true;
}

// Este MAIN ahora sí es interactivo
int main() {
    string expresionUsuario;

    cout << "--- Verificador de Profundidad de Anidamiento ---" << endl;
    cout << "Por favor, ingrese la expresion matematica a evaluar: " << endl;
    
    // Usamos getline para que permita capturar espacios en blanco
    getline(cin, expresionUsuario); 

    cout << "\nAnalizando expresion..." << endl;
    
    if (verificarAgrupamiento(expresionUsuario)) {
        cout << ">> Resultado: VALIDA (Simbolos correctamente distribuidos) <<" << endl;
    } else {
        cout << ">> Resultado: INVALIDA <<" << endl;
    }

    cout << "-------------------------------------------------" << endl;
    return 0;
}