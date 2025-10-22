#pragma once
#include <iostream>
#include <string>
using namespace std;
// ============================================================
// Funcion: modInverse
// ------------------------------------------------------------
// Calcula el inverso multiplicativo de 'a' modulo 'm'.
// En el cifrado Afin, esto se usa para descifrar el texto.
// Parametros:
//   - a: clave multiplicativa (debe ser coprima con 26).
//   - m: modulo (por defecto 26, cantidad de letras del alfabeto).
// ============================================================
inline int modInverse(int a, int m = 26) {
    for (int x = 1; x < m; x++)
        if ((a * x) % m == 1) return x;
    return 1; // valor por defecto si no existe inverso (no deberia pasar)
}
// ============================================================
// Funcion: cifrarAfin
// ------------------------------------------------------------
// Aplica el cifrado Afin sobre el texto recibido por referencia.
// Formula: C = (a * (P - base) + b) mod 26 + base
// Parametros:
//   - texto: referencia al texto a cifrar.
//   - a: clave multiplicativa (debe ser coprima con 26).
//   - b: clave aditiva (puede ser cualquier numero).
// ============================================================
void cifrarAfin(string& texto, int a, int b) {
    for (char& c : texto) {
        if (isalpha(c)) { // solo letras
            char base = isupper(c) ? 'A' : 'a';
            c = (a * (c - base) + b) % 26 + base;
        }
    }
    cout << "Texto cifrado (Afin): " << texto << endl;
}
// ============================================================
// Funcion: descifrarAfin
// Revierte el cifrado Afin, devolviendo el texto original.
// Formula: P = a_inv * (C - base - b) mod 26 + base
// Parametros:
//   - texto: referencia al texto cifrado.
//   - a: clave multiplicativa usada al cifrar.
//   - b: clave aditiva usada al cifrar.
// ============================================================
void descifrarAfin(string& texto, int a, int b) {
    int a_inv = modInverse(a); // calcula el inverso modular de 'a'
    for (char& c : texto) {
        if (isalpha(c)) {
            char base = isupper(c) ? 'A' : 'a';
            c = (a_inv * ((c - base - b + 26)) % 26) + base;
        }
    }
    cout << "Texto descifrado (Afin): " << texto << endl;
}