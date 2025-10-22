#pragma once
#ifndef FUERZA_BRUTA_H
#define FUERZA_BRUTA_H
#include <iostream>
#include <string>
using namespace std;
// funcion que busca un patron dentro de un texto usando el metodo de fuerza bruta
void buscarFuerzaBruta(const string& texto, const string& patron) {
    cout << "[Fuerza Bruta]" << endl;
    bool encontrado = false; // bandera para indicar si se encontro el patron
    // verifica si alguno de los strings esta vacio
    if (patron.empty() || texto.empty()) {
        cout << "texto o patron vacio." << endl;
        return;
    }
    // recorre el texto desde la posicion 0 hasta la ultima posible coincidencia
    for (size_t i = 0; i <= texto.size() - patron.size(); ++i) {
        size_t j = 0;
        // compara caracter por caracter del patron con el texto
        while (j < patron.size() && texto[i + j] == patron[j]) {
            j++;
        }
        // si todos los caracteres coincidieron, se encontro el patron
        if (j == patron.size()) {
            cout << "patron encontrado en posicion: " << i << endl;
            encontrado = true;
        }
    }
    // si no se encontro ninguna coincidencia
    if (!encontrado)
        cout << "patron no encontrado." << endl;
}
#endif