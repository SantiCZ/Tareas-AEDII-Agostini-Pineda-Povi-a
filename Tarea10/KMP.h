#pragma once
#include <iostream>
#include <fstream>   // para manejo de archivos
#include <string>
#include "utilidades.h" // para la función trim() que elimina espacios
using namespace std;
// ============================================================
// FUNCION: buscarFuerzaBrutaArchivo
// ------------------------------------------------------------
// Busca un producto dentro del archivo "productos_minimarket.txt"
// comparando el codigo de producto ingresado con el listado.
// Usa una busqueda directa (Fuerza Bruta).
// ------------------------------------------------------------
// Parametro:
//   - codigoBuscado: codigo del producto que se quiere buscar.
// ============================================================
void buscarFuerzaBrutaArchivo(const string& codigoBuscado) {
    ifstream file("productos_minimarket.txt"); // abre el archivo de productos
    if (!file.is_open()) {// verifica si se abrio correctamente
        cout << "No se pudo abrir productos_minimarket.txt"<<endl;
        return; // si falla, termina la funcion
    }
    string linea;
    bool encontrado = false; // bandera para saber si el codigo fue hallado
    // lee linea por linea el archivo
    while (getline(file, linea)) {
        // busca la posicion del punto que separa el codigo del nombre del producto
        size_t pos = linea.find('.');
        if (pos != string::npos) {
            // extrae el codigo antes del punto
            string codigo = trim(linea.substr(0, pos));
            // compara el codigo con el que busca el usuario
            if (codigo == codigoBuscado) {
                encontrado = true;
                break; // termina la busqueda al encontrar coincidencia
            }
        }
    }
    file.close(); 
    // muestra resultado al usuario
    if (encontrado)
        cout << "Producto con codigo '" << codigoBuscado << "' SI se encuentra en stock."<<endl;
    else
        cout << "Producto con codigo '" << codigoBuscado << "' NO se encuentra en stock."<<endl;
}
// ============================================================
// FUNCION: buscarKMPArchivo
// Version alternativa de busqueda que reutiliza la misma logica
// de Fuerza Bruta, pero puede reemplazarse por el algoritmo KMP
// si se desea hacer la busqueda optimizada por patrones.
void buscarKMPArchivo(const string& codigoBuscado) {
    buscarFuerzaBrutaArchivo(codigoBuscado); // reutiliza búsqueda básica
}
// ============================================================
// FUNCION: buscarBoyerMooreArchivo
// Version alternativa de busqueda que reutiliza la logica base,
// aunque en un escenario real implementaría el algoritmo
// Boyer-Moore para optimizar busquedas en textos grandes.
void buscarBoyerMooreArchivo(const string& codigoBuscado) {
    buscarFuerzaBrutaArchivo(codigoBuscado); // reutiliza busqueda basica
}