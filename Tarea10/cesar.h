#pragma once
#include <iostream>
#include <string>
using namespace std;
// ============================================================
// Funcion: cifrarCesar
// Aplica el cifrado Cesar a un texto recibido por referencia.
// Desplaza cada letra del alfabeto segun la clave numerica.
// Parametros:
//   - texto: referencia al texto que se quiere cifrar.
//   - clave: cantidad de posiciones a desplazar cada letra.
// ============================================================
void cifrarCesar(string& texto, int clave) {
    for (char& c : texto) {
        if (isalpha(c)) { // verifica si el caracter es una letra
            char base = isupper(c) ? 'A' : 'a'; // determina si es mayuscula o minuscula
            c = (c - base + clave) % 26 + base; // aplica desplazamiento modular
        }
    }
    cout << "Texto cifrado (Cesar): " << texto << endl;
}
// ============================================================
// Funcion: descifrarCesar
// Invierte el cifrado Cesar, devolviendo el texto original.
// Usa la misma clave que se empleo para cifrar.
// Parametros:
//   - texto: referencia al texto cifrado.
//   - clave: cantidad de posiciones usadas para cifrar.
// ============================================================
void descifrarCesar(string& texto, int clave) {
    for (char& c : texto) {
        if (isalpha(c)) { // solo letras del alfabeto
            char base = isupper(c) ? 'A' : 'a';
            c = (c - base - clave + 26) % 26 + base; // resta la clave y ajusta alfabeto
        }
    }
    cout << "Texto descifrado (Cesar): " << texto << endl;
}