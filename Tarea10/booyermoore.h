#pragma once
#ifndef BOYER_MOORE_H
#define BOYER_MOORE_H
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
#define ALPHABET_SIZE 256  // cantidad de posibles caracteres (ascii)
// funcion para preprocesar el patron y llenar la tabla de mal caracter
void preprocesarBadChar(const string& patron, vector<int>& badChar) {
    // inicializa todas las posiciones con -1
    for (int i = 0; i < ALPHABET_SIZE; i++)
        badChar[i] = -1;
    // almacena la ultima posicion donde aparece cada caracter del patron
    for (int i = 0; i < patron.size(); i++)
        badChar[(unsigned char)patron[i]] = i;
}
// busca el patron dentro del texto usando boyer-moore
void buscarBoyerMoore(const string& texto, const string& patron) {
    cout << "[Boyer-Moore]" << endl;
    // verifica si alguno de los strings esta vacio
    if (patron.empty() || texto.empty()) {
        cout << "texto o patron vacio." << endl;
        return;
    }
    int n = texto.size();  // longitud del texto
    int m = patron.size(); // longitud del patron
    vector<int> badChar(ALPHABET_SIZE); // tabla de desplazamientos
    preprocesarBadChar(patron, badChar); // se genera la tabla con el patron
    int s = 0; // desplazamiento actual sobre el texto
    bool encontrado = false; // bandera para indicar si se encontro el patron
    // se recorre el texto hasta que no haya espacio suficiente para el patron
    while (s <= (n - m)) {
        int j = m - 1; // indice del ultimo caracter del patron
        // compara los caracteres desde el final hacia el principio
        while (j >= 0 && patron[j] == texto[s + j])
            j--;
        // si j es menor a 0, significa que todos los caracteres coincidieron
        if (j < 0) {
            cout << "patron encontrado en posicion: " << s << endl;
            encontrado = true;
            // se ajusta el desplazamiento segun la regla de mal caracter
            s += (s + m < n) ? m - badChar[(unsigned char)texto[s + m]] : 1;
        }
        else {
            // se calcula el desplazamiento usando la tabla de mal caracter
            s += max(1, j - badChar[(unsigned char)texto[s + j]]);
        }
    }
    // si no se encontro ninguna coincidencia
    if (!encontrado)
        cout << "patron no encontrado." << endl;
}
#endif