#ifndef HUFFMAN_H
#define HUFFMAN_H
#include <stack>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <string>
#include <fstream>
using namespace std;
// ============================================================
// ESTRUCTURA BASICA DE UN NODO DEL ARBOL DE HUFFMAN
// ============================================================
struct Nodo {
    char ch;// caracter almacenado
    int freq;// frecuencia del caracter
    Nodo* izq;// hijo izquierdo
    Nodo* der;// hijo derecho
    Nodo(char c, int f) : ch(c), freq(f), izq(nullptr), der(nullptr) {}
};
// ============================================================
// COMPARADOR PARA LA COLA DE PRIORIDAD (ORDENA POR FRECUENCIA)
// ============================================================
struct comparar {
    bool operator()(Nodo* a, Nodo* b) {
        return a->freq > b->freq;
    }
};
// ============================================================
// FUNCION RECURSIVA PARA GENERAR LOS CODIGOS BINARIOS DE CADA CARACTER
// ============================================================
inline void generarCodigos(Nodo* raiz, string str, unordered_map<char, string>& codigo) {
    if (!raiz) return;
    // Si es una hoja, asigna el codigo al caracter
    if (!raiz->izq && !raiz->der)
        codigo[raiz->ch] = str;
    // Recorre izquierda y derecha agregando 0 o 1
    generarCodigos(raiz->izq, str + "0", codigo);
    generarCodigos(raiz->der, str + "1", codigo);
}
// ============================================================
//  FUNCION SIMPLE: COMPRIME TEXTO DIRECTAMENTE DESDE CONSOLA
// ============================================================
inline void comprimirHuffman(const string& texto) {
    // Contar frecuencia de cada caracter
    unordered_map<char, int> freq;
    for (char c : texto)
        freq[c]++;
    // Crear cola de prioridad (min-heap)
    priority_queue<Nodo*, vector<Nodo*>, comparar> pq;
    for (auto& p : freq)
        pq.push(new Nodo(p.first, p.second));
    // Construir arbol de Huffman
    while (pq.size() > 1) {
        Nodo* izq = pq.top(); pq.pop();
        Nodo* der = pq.top(); pq.pop();
        Nodo* nuevo = new Nodo('\0', izq->freq + der->freq);
        nuevo->izq = izq;
        nuevo->der = der;
        pq.push(nuevo);
    }
    // Raiz del arbol final
    Nodo* raiz = pq.top();
    // Generar codigos para cada caracter
    unordered_map<char, string> codigo;
    generarCodigos(raiz, "", codigo);
    // Generar texto comprimido
    string comprimido = "";
    for (char c : texto)
        comprimido += codigo[c];
    // Mostrar resultados
    cout << "Texto original: " << texto << endl;
    cout << "Codigos Huffman:\n";
    for (auto& p : codigo)
        cout << p.first << ": " << p.second << endl;
    cout << "Comprimido: " << comprimido << endl;
    cout << "Tamano original: " << texto.size() * 8 << " bits" << endl;
    cout << "Tamano comprimido: " << comprimido.size() << " bits" << endl;
}
// ============================================================
//  VERSION COMPLETA: LEE TEXTO DESDE ARCHIVO Y GUARDA EL RESULTADO
// ============================================================
inline void comprimirHuffmanArchivo(const string& inputFile, const string& outputFile) {
    // Abrir archivo de entrada
    ifstream entrada(inputFile);
    if (!entrada.is_open()) {
        cout << "Error al abrir archivo de entrada: " << inputFile << endl;
        return;
    }
    // Leer todo el contenido del archivo
    string texto((istreambuf_iterator<char>(entrada)), istreambuf_iterator<char>());
    entrada.close();
    // Calcular frecuencias de cada caracter
    unordered_map<char, int> freq;
    for (char c : texto)
        freq[c]++;
    // Crear cola de prioridad con los nodos
    priority_queue<Nodo*, vector<Nodo*>, comparar> pq;
    for (auto& p : freq)
        pq.push(new Nodo(p.first, p.second));
    // Construir arbol de Huffman
    while (pq.size() > 1) {
        Nodo* izq = pq.top(); pq.pop();
        Nodo* der = pq.top(); pq.pop();
        Nodo* nuevo = new Nodo('\0', izq->freq + der->freq);
        nuevo->izq = izq;
        nuevo->der = der;
        pq.push(nuevo);
    }
    // Generar codigos Huffman
    Nodo* raiz = pq.top();
    unordered_map<char, string> codigo;
    generarCodigos(raiz, "", codigo);
    // Crear texto comprimido
    string comprimido = "";
    for (char c : texto)
        comprimido += codigo[c];
    // Guardar en archivo de salida
    ofstream salida(outputFile);
    salida << comprimido;
    salida.close();
    cout << "Archivo comprimido guardado en: " << outputFile << endl;
}
// ============================================================
//  FUNCION PARA DESCOMPRIMIR TEXTO USANDO EL ARBOL EN MEMORIA
// ============================================================
inline string descomprimirHuffman(const string& comprimido, Nodo* raiz) {
    string resultado;
    Nodo* actual = raiz;
    // Recorrer los bits del texto comprimido
    for (char bit : comprimido) {
        if (bit == '0') actual = actual->izq;
        else actual = actual->der;
        // Si llegamos a una hoja, agregamos el caracter
        if (!actual->izq && !actual->der) {
            resultado += actual->ch;
            actual = raiz;
        }
    }
    return resultado;
}
#endif