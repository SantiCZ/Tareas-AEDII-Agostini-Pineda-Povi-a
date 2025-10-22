#pragma once                 
#ifndef HILL_H               
#define HILL_H               
#include <iostream>          
#include <vector>           
#include <string>            
#include <cctype>            
using namespace std;         
// ============================================================
//                FUNCIONES AUXILIARES HILL
// ============================================================
// Calcula el modulo 26 asegurando que el resultado sea positivo
int mod26(int x) {
    int r = x % 26;// Calcula el resto de la division por 26
    return (r < 0) ? r + 26 : r;  // Si es negativo, suma 26
}
// Lee una matriz cuadrada de tamano n desde la entrada del usuario
vector<vector<int>> leerMatrizHill(int n) {
    vector<vector<int>> M(n, vector<int>(n));  // Crea una matriz n x n
    cout << "Ingrese los elementos de la matriz clave (" << n << "x" << n << "):" << endl;
    for (int i = 0; i < n; ++i)// Recorre filas
        for (int j = 0; j < n; ++j)// Recorre columnas
            cin >> M[i][j];// Lee cada elemento
    cin.ignore();// Limpia el buffer de entrada
    return M;// Devuelve la matriz leida
}
// Convierte un texto en un vector de numeros segun el alfabeto (A=0, B=1, y asi segun lo que se necesite)
vector<int> textoANumeros(const string& texto) {
    vector<int> v;// Vector donde se guardaran los valores numericos
    for (char c : texto)// Recorre cada caracter del texto
        if (isalpha(c))// Verifica que sea una letra
            v.push_back(toupper(c) - 'A');// Convierte la letra a mayuscula y luego a numero
    return v;// Devuelve el vector numerico
}
// Convierte un vector de numeros nuevamente a texto
string numerosATexto(const vector<int>& v) {
    string s;// Cadena donde se guardara el resultado
    for (int n : v)// Recorre cada numero del vector
        s += char('A' + mod26(n));// Convierte el numero a letra (0->A, etc.)
    return s;// Devuelve la cadena resultante
}
// ============================================================
//                CIFRAR HILL
// ============================================================
// Funcion que cifra un texto con la matriz Hill dada
void cifrarHill(const string& texto, const vector<vector<int>>& M) {
    int n = M.size();// Tamano de la matriz clave (2x2, 3x3, etc.)
    vector<int> nums = textoANumeros(texto);// Convierte el texto a numeros
    while (nums.size() % n != 0)// Si el texto no es multiplo de n
        nums.push_back('X' - 'A');// Rellena con 'X'
    vector<int> cifrado;// Vector para almacenar el texto cifrado
    for (size_t i = 0; i < nums.size(); i += n) { // Recorre bloques de tamano n
        for (int r = 0; r < n; ++r) {// Recorre las filas de la matriz
            int sum = 0;// Acumulador para el resultado de cada bloque
            for (int c = 0; c < n; ++c)// Recorre las columnas de la matriz
                sum += M[r][c] * nums[i + c];  // Multiplica y suma (producto matricial)
            cifrado.push_back(mod26(sum)); // Aplica modulo 26 al resultado
        }
    }
    cout << "Texto cifrado (Hill): " << numerosATexto(cifrado) << endl;  // Muestra el texto cifrado
}
// ============================================================
//                DESCIFRAR HILL (2x2 SIMPLIFICADO)
// ============================================================
// Calcula el determinante de una matriz 2x2
int det2x2(const vector<vector<int>>& M) {
    return mod26(M[0][0] * M[1][1] - M[0][1] * M[1][0]);  // Determinante mod 26
}
// Calcula el inverso multiplicativo de un numero mod 26
int inversoMod26(int a) {
    for (int i = 1; i < 26; ++i)// Recorre posibles inversos
        if (mod26(a * i) == 1)// Si cumple la condicion de inverso
            return i;// Retorna el inverso
    return -1;// Si no hay inverso, retorna -1
}
// Calcula la matriz inversa de una matriz 2x2 mod 26
vector<vector<int>> inversa2x2(const vector<vector<int>>& M) {
    int det = det2x2(M);// Calcula el determinante
    int invDet = inversoMod26(det);// Calcula su inverso modular
    if (invDet == -1) {// Si no existe inverso
        cout << "No existe inverso modular para el determinante, matriz invalida." << endl;
        return { {0,0},{0,0} };                 // Retorna matriz nula
    }
    vector<vector<int>> inv(2, vector<int>(2));// Crea la matriz inversa
    inv[0][0] = mod26(invDet * M[1][1]);// Intercambia diagonal principal
    inv[0][1] = mod26(-invDet * M[0][1]);// Cambia signo y multiplica por inverso
    inv[1][0] = mod26(-invDet * M[1][0]);// Cambia signo y multiplica por inverso
    inv[1][1] = mod26(invDet * M[0][0]);// Intercambia diagonal secundaria
    return inv;// Retorna la matriz inversa
}
// Descifra un texto cifrado con Hill usando matriz 2x2
void descifrarHill(const string& texto, const vector<vector<int>>& M) {
    if (M.size() != 2) {// Verifica que la matriz sea 2x2
        cout << "Descifrado solo disponible para matriz 2x2." << endl;
        return;// Sale si no lo es
    }
    vector<vector<int>> inv = inversa2x2(M);  // Calcula la matriz inversa
    vector<int> nums = textoANumeros(texto);  // Convierte texto a numeros
    vector<int> descifrado;                   // Vector para resultado descifrado
    for (size_t i = 0; i < nums.size(); i += 2) { // Recorre de a bloques de 2 letras
        for (int r = 0; r < 2; ++r) {// Recorre filas de la matriz inversa
            int sum = 0;// Inicializa acumulador
            for (int c = 0; c < 2; ++c)// Recorre columnas
                sum += inv[r][c] * nums[i + c]; // Multiplica y suma (producto matricial)
            descifrado.push_back(mod26(sum)); // Aplica mod 26 al resultado
        }
    }
    cout << "Texto descifrado (Hill): " << numerosATexto(descifrado) << endl; 
}
#endif 