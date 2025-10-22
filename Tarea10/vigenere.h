#pragma once
#include <iostream>
#include <string>
using namespace std;
// ============================================================
// FUNCION: cifrarVigenere
// Cifra un texto utilizando el algoritmo de Vigenere.
// Parametros:
//   - texto: referencia al texto que se va a cifrar.
//   - clave: palabra clave utilizada para generar el desplazamiento.
// ============================================================
void cifrarVigenere(string& texto, const string& clave) {
    // recorre todo el texto caracter por caracter
    for (int i = 0, j = 0; i < texto.size(); i++) {
        char c = texto[i];
        // verifica si el caracter es una letra (ignora espacios y simbolos)
        if (isalpha(c)) {
            // determina si es mayuscula o minuscula para mantener el formato
            char base = isupper(c) ? 'A' : 'a';
            // aplica el desplazamiento según la letra correspondiente de la clave
            // (posicion del texto + posicion de la clave) mod 26
            c = (c - base + (tolower(clave[j % clave.size()]) - 'a')) % 26 + base;
            // avanza el indice de la clave solo si se proceso una letra
            j++;
        }
        // actualiza el texto con el caracter cifrado
        texto[i] = c;
    }
    cout << "Texto cifrado (Vigenere): " << texto << endl;
}
// ============================================================
// FUNCION: descifrarVigenere
// Descifra un texto cifrado con el metodo de Vigenere.
// Parámetros:
//   - texto: referencia al texto cifrado que se desea descifrar.
//   - clave: palabra clave usada originalmente para cifrar.
// ============================================================
void descifrarVigenere(string& texto, const string& clave) {
    // recorre todo el texto caracter por caracter
    for (int i = 0, j = 0; i < texto.size(); i++) {
        char c = texto[i];
        // solo procesa letras
        if (isalpha(c)) {
            char base = isupper(c) ? 'A' : 'a';
            // resta el desplazamiento de la clave para revertir el cifrado
            c = (c - base - (tolower(clave[j % clave.size()]) - 'a') + 26) % 26 + base;
            j++;
        }
        // actualiza el texto con el caracter descifrado
        texto[i] = c;
    }
    cout << "Texto descifrado (Vigenere): " << texto << endl;
}