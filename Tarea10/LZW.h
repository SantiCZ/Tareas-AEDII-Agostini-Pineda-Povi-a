#pragma once
#ifndef LZW_H
#define LZW_H
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;
// funcion que comprime un texto usando el algoritmo lzw
void comprimirLZW(const string& input) {
    unordered_map<string, int> dic; // diccionario para almacenar secuencias y sus codigos
    // inicializa el diccionario con todos los caracteres ascii basicos
    for (int i = 0; i < 256; i++)
        dic[string(1, i)] = i;
    string p;// secuencia actual
    vector<int> resultado;// vector donde se guardaran los codigos comprimidos
    int codigo = 256;// siguiente codigo disponible para nuevas combinaciones
    // recorre cada caracter del texto de entrada
    for (char c : input) {
        string pc = p + c; // combina la secuencia actual con el nuevo caracter
        // si la combinacion ya existe en el diccionario, se actualiza p
        if (dic.count(pc))
            p = pc;
        else {
            // si no existe, se agrega el codigo de p al resultado
            resultado.push_back(dic[p]);
            // se inserta la nueva combinacion en el diccionario
            dic[pc] = codigo++;
            // se reinicia p con el caracter actual
            p = string(1, c);
        }
    }
    // agrega el ultimo codigo si hay una secuencia pendiente
    if (!p.empty())
        resultado.push_back(dic[p]);
    // muestra los resultados por consola
    cout << "Texto original: " << input << endl;
    cout << "Secuencia comprimida (LZW): ";
    for (int n : resultado)
        cout << n << " ";
    cout << endl;
    // muestra el tamaño comparativo entre el texto original y el comprimido
    cout << "Tamaño original: " << input.size() * 8 << " bits" << endl;
    cout << "Tamaño comprimido aprox: " << resultado.size() * 12 << " bits" << endl;
}
// funcion que descomprime una secuencia de codigos generada por el algoritmo LZW
string descomprimirLZW(const vector<int>& codigos) {
    unordered_map<int, string> dic; // diccionario que mapea codigos a cadenas
    // inicializa el diccionario con los 256 caracteres ASCII
    for (int i = 0; i < 256; i++) dic[i] = string(1, i);
    int codigo = 256;// siguiente codigo disponible para nuevas entradas
    string p = dic[codigos[0]]; // primer codigo, ya existe en el diccionario
    string resultado = p;// resultado final, inicia con la primera entrada
    // recorre los codigos restantes
    for (size_t i = 1; i < codigos.size(); i++) {
        string entrada;
        int k = codigos[i]; // codigo actual
        // si el codigo existe en el diccionario, se obtiene la cadena correspondiente
        if (dic.count(k)) entrada = dic[k];
        // si no existe (caso especial), se construye como p + primer caracter de p
        else entrada = p + p[0];
        // agrega la cadena decodificada al resultado final
        resultado += entrada;
        // agrega una nueva entrada al diccionario: cadena anterior + primer caracter de la actual
        dic[codigo++] = p + entrada[0];
        // actualiza p para la siguiente iteracion
        p = entrada;
    }
    // devuelve el texto descomprimido completo
    return resultado;
}
#endif