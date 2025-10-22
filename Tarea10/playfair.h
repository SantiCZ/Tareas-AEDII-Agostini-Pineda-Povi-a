#pragma once
#ifndef PLAYFAIR_H
#define PLAYFAIR_H
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
using namespace std;
// ============================================================
//        FUNCIONES AUXILIARES PARA CIFRADO PLAYFAIR
// ============================================================
// Genera la matriz 5x5 usada por el algoritmo Playfair
// A partir de la clave, elimina letras repetidas y reemplaza J por I
vector<vector<char>> generarMatrizPlayfair(const string& clave) {
    string filtrada;
    // Procesa la clave: convierte a mayusculas, reemplaza J por I, y elimina duplicados
    for (char c : clave) {
        if (isalpha(c)) {
            char upper = toupper(c == 'J' ? 'I' : c);
            if (filtrada.find(upper) == string::npos)
                filtrada += upper; // agrega si no existe aun
        }
    }
    // Completa la matriz con el resto del alfabeto (sin J)
    string alfabeto = "ABCDEFGHIKLMNOPQRSTUVWXYZ";
    for (char c : alfabeto) {
        if (filtrada.find(c) == string::npos)
            filtrada += c;
    }
    // Llena una matriz 5x5 con las letras procesadas
    vector<vector<char>> matriz(5, vector<char>(5));
    int k = 0;
    for (int i = 0; i < 5; ++i)
        for (int j = 0; j < 5; ++j)
            matriz[i][j] = filtrada[k++];
    return matriz;
}
// Devuelve la posicion (fila, columna) de una letra en la matriz
// Si el caracter es 'J', se reemplaza por 'I'.
pair<int, int> posicion(const vector<vector<char>>& matriz, char c) {
    if (c == 'J') c = 'I';
    for (int i = 0; i < 5; ++i)
        for (int j = 0; j < 5; ++j)
            if (matriz[i][j] == c)
                return { i, j };
    return { -1, -1 }; // si no se encuentra (no debería pasar)
}
// ============================================================
//                   CIFRAR CON PLAYFAIR
// ============================================================
void cifrarPlayfair(const string& texto, const string& clave) {
    // Genera la matriz de cifrado
    auto matriz = generarMatrizPlayfair(clave);
    // Limpia el texto: solo letras, mayúsculas, y J→I
    string limpio;
    for (char c : texto)
        if (isalpha(c))
            limpio += toupper(c == 'J' ? 'I' : c);
    // Prepara el texto: agrega 'X' entre letras iguales y al final si es impar
    string preparado;
    for (size_t i = 0; i < limpio.size(); ++i) {
        preparado += limpio[i];
        if (i + 1 < limpio.size()) {
            if (limpio[i] == limpio[i + 1])
                preparado += 'X';
        }
    }
    if (preparado.size() % 2 != 0)
        preparado += 'X';
    // Aplica las reglas de cifrado Playfair
    string cifrado;
    for (size_t i = 0; i < preparado.size(); i += 2) {
        auto [r1, c1] = posicion(matriz, preparado[i]);
        auto [r2, c2] = posicion(matriz, preparado[i + 1]);
        if (r1 == r2) {
            // Mismo fila → reemplazar por la letra a la derecha
            cifrado += matriz[r1][(c1 + 1) % 5];
            cifrado += matriz[r2][(c2 + 1) % 5];
        }
        else if (c1 == c2) {
            // Misma columna → reemplazar por la letra debajo
            cifrado += matriz[(r1 + 1) % 5][c1];
            cifrado += matriz[(r2 + 1) % 5][c2];
        }
        else {
            // Diferente fila y columna → intercambiar columnas
            cifrado += matriz[r1][c2];
            cifrado += matriz[r2][c1];
        }
    }
    cout << "Texto cifrado (Playfair): " << cifrado << endl;
}
// ============================================================
//                   DESCIFRAR CON PLAYFAIR
// ============================================================
void descifrarPlayfair(const string& texto, const string& clave) {
    // Genera la misma matriz que se usó para cifrar
    auto matriz = generarMatrizPlayfair(clave);
    // Limpia el texto cifrado
    string limpio;
    for (char c : texto)
        if (isalpha(c))
            limpio += toupper(c == 'J' ? 'I' : c);
    string descifrado;
    for (size_t i = 0; i < limpio.size(); i += 2) {
        auto [r1, c1] = posicion(matriz, limpio[i]);
        auto [r2, c2] = posicion(matriz, limpio[i + 1]);
        if (r1 == r2) {
            // Misma fila → moverse a la izquierda
            descifrado += matriz[r1][(c1 + 4) % 5];
            descifrado += matriz[r2][(c2 + 4) % 5];
        }
        else if (c1 == c2) {
            // Misma columna → moverse hacia arriba
            descifrado += matriz[(r1 + 4) % 5][c1];
            descifrado += matriz[(r2 + 4) % 5][c2];
        }
        else {
            // Rectangulo → intercambiar columnas
            descifrado += matriz[r1][c2];
            descifrado += matriz[r2][c1];
        }
    }
    cout << "Texto descifrado (Playfair): " << descifrado << endl;
}
#endif