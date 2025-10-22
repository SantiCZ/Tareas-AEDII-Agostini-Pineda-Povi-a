#include <iostream>             
#include <SFML/Graphics.hpp>      //de momento no implementada, pero configurada para despues
#include <fstream>              
#include <string>               
#include <sstream>             
#include "cesar.h"              
#include "menu_SFML.h"
#include "huffman.h"          
#include "vigenere.h"           
#include "booyermoore.h"        
#include "KMP.h"               
#include "fuerzabruta.h"        
#include "LZW.h"                 
#include "utilidades.h"          
#include "afin_cifrar.h"         
#include "playfair.h"          
#include "hill.h"                
using namespace std;            
// ============================================================
// FUNCION PARA AGREGAR PRODUCTOS AL ARCHIVO DE TEXTO
// ============================================================
void agregarProducto() {
    ofstream archivo("productos_minimarket.txt", ios::app); // Abre archivo en modo agregar
    if (!archivo) {// Si falla al abrir
        cout << "Error al abrir el archivo productos_minimarket.txt" << endl;
        return;// Sale de la funcion
    }
    string codigo, nombre;// Variables para codigo y nombre
    float precio;// Variable para precio
    int cantidad;// Variable para cantidad disponible
    cout << "Ingrese codigo del producto: ";// Solicita codigo
    getline(cin, codigo);// Lee codigo completo
    cout << "Ingrese nombre del producto: ";// Solicita nombre
    getline(cin, nombre);// Lee nombre completo
    cout << "Ingrese precio del producto: ";// Solicita precio
    cin >> precio;// Lee precio
    cout << "Ingrese cantidad disponible: ";// Solicita cantidad
    cin >> cantidad;// Lee cantidad
    cin.ignore();// Limpia buffer de entrada
    archivo << codigo << " | " << nombre << " | " << precio << " | " << cantidad << endl; // Escribe al archivo
    archivo.close();// Cierra el archivo
    cout << "Producto agregado correctamente al archivo productos_minimarket.txt" << endl; // Mensaje confirmacion
}
// ============================================================
// FUNCIONES DE COMPRESION/DESCOMPRESION LZW CON ARCHIVOS
// ============================================================
void comprimirLZWArchivo(const string& entrada, const string& salida) {
    ifstream in(entrada);// Abre archivo de entrada
    if (!in) {// Si falla al abrir
        cout << "Error al abrir el archivo de entrada: " << entrada << endl;
        return;// Sale de la funcion
    }
    stringstream buffer;// Crea buffer de string
    buffer << in.rdbuf();// Copia contenido del archivo al buffer
    string texto = buffer.str();// Convierte buffer a string
    in.close();// Cierra archivo de entrada
    // Compresion LZW
    unordered_map<string, int> dic;// Diccionario LZW
    for (int i = 0; i < 256; i++) dic[string(1, i)] = i; // Inicializa diccionario con caracteres ASCII
    string p;// Cadena temporal
    vector<int> resultado;// Vector para resultados
    int codigo = 256;// Proximo codigo disponible
    for (char c : texto) {// Recorre cada caracter del texto
        string pc = p + c;// Crea nueva combinacion
        if (dic.count(pc))// Si ya esta en diccionario
            p = pc;// Actualiza p
        else {// Si no esta
            resultado.push_back(dic[p]);// Guarda codigo
            dic[pc] = codigo++;// Agrega nuevo codigo al diccionario
            p = string(1, c);// Reinicia p con caracter actual
        }
    }
    if (!p.empty()) resultado.push_back(dic[p]);// Agrega ultimo codigo si queda
    ofstream out(salida, ios::binary);// Abre archivo de salida binario
    for (int n : resultado)// Escribe cada codigo
        out.write(reinterpret_cast<const char*>(&n), sizeof(int));
    out.close();// Cierra archivo
    cout << "Archivo comprimido correctamente con LZW: " << salida << endl; // Mensaje confirmacion
}
//descomprimir archivos con LZW 
void descomprimirLZWArchivo(const string& entrada, const string& salida) {
    ifstream in(entrada, ios::binary);// Abre archivo binario de entrada
    if (!in) {// Si falla al abrir
        cout << "Error al abrir el archivo comprimido: " << entrada << endl;
        return;
    }
    vector<int> codigos;// Vector para leer codigos
    int n;// Variable temporal
    while (in.read(reinterpret_cast<char*>(&n), sizeof(int))) // Lee cada codigo
        codigos.push_back(n);
    in.close();// Cierra archivo
    unordered_map<int, string> dic;// Diccionario inverso
    for (int i = 0; i < 256; i++) dic[i] = string(1, i); // Inicializa con caracteres ASCII
    int codigo = 256;// Proximo codigo
    string p = dic[codigos[0]];// Primer valor
    string resultado = p;// Inicializa resultado
    for (size_t i = 1; i < codigos.size(); i++) {// Recorre codigos restantes
        string entradaStr;// Cadena temporal
        int k = codigos[i];// Codigo actual
        if (dic.count(k)) entradaStr = dic[k];// Si esta en diccionario
        else entradaStr = p + p[0];// Si no, reconstruye
        resultado += entradaStr;// Agrega a resultado
        dic[codigo++] = p + entradaStr[0];// Agrega nueva entrada al diccionario
        p = entradaStr;// Actualiza p
    }
    ofstream out(salida);// Abre archivo de salida
    out << resultado;// Escribe resultado
    out.close();// Cierra archivo
    cout << "Archivo descomprimido correctamente con LZW: " << salida << endl; // Mensaje confirmacion
}
// ============================================================
// MENU PRINCIPAL (EMPLEADO)
// ============================================================
void mostrarMenuEmpleado() {
    cout << "=============================================" << endl;
    cout << "\tSISTEMA MINIMARKET (EMPLEADO)" << endl;
    cout << "=============================================" << endl;
    cout << "1. Buscar producto (Fuerza Bruta)" << endl;
    cout << "2. Buscar producto (KMP)" << endl;
    cout << "3. Buscar producto (Boyer-Moore)" << endl;
    cout << "4. Cifrar texto (Cesar)" << endl;
    cout << "5. Descifrar texto (Cesar)" << endl;
    cout << "6. Cifrar texto (Afin)" << endl;
    cout << "7. Descifrar texto (Afin)" << endl;
    cout << "8. Cifrar texto (Vigenere)" << endl;
    cout << "9. Descifrar texto (Vigenere)" << endl;
    cout << "10. Agregar producto al archivo" << endl;
    cout << "11. Comprimir archivo (Huffman)" << endl;
    cout << "12. Descomprimir archivo (Huffman)" << endl;
    cout << "13. Volver al menu de seleccion de rol" << endl;
    cout << "14. Cifrar texto (Playfair)" << endl;
    cout << "15. Descifrar texto (Playfair)" << endl;
    cout << "16. Cifrar texto (Hill)" << endl;
    cout << "17. Descifrar texto (Hill)" << endl;
    cout << "18. Comprimir archivo (LZW)" << endl;
    cout << "19. Descomprimir archivo (LZW)" << endl;
    cout << "20. Ejecutar menu en SFML" << endl; // menu en SFML
    cout << "0. Salir" << endl;
    cout << "Seleccione una opcion: ";
}
// ============================================================
// MENU CLIENTE
// ============================================================
void mostrarMenuCliente() {
    cout << "==========================================" << endl;
    cout << "\tSISTEMA MINIMARKET (CLIENTE)" << endl;
    cout << "==========================================" << endl;
    cout << "1. Comprimir archivo (Huffman)" << endl;
    cout << "2. Descomprimir archivo (Huffman)" << endl;
    cout << "3. Volver al menu de seleccion de rol" << endl;
    cout << "4. Comprimir archivo (LZW)" << endl;
    cout << "5. Descomprimir archivo (LZW)" << endl;
    cout << "6. Ejecutar menu en SFML" << endl;
    cout << "0. Salir" << endl;
    cout << "Seleccione una opcion: ";
}
// ============================================================
// MENU DE INGRESO DE ROL
// ============================================================
int seleccionarRol() {
    int rol;// Variable para guardar el rol
    cout << "====================================" << endl;
    cout << "\tBIENVENIDO AL SISTEMA" << endl;
    cout << "====================================" << endl;
    cout << "Seleccione su rol:" << endl;
    cout << "1. Empleado" << endl;
    cout << "2. Cliente" << endl;
    cout << "0. Salir" << endl;
    cout << "Opcion: ";
    cin >> rol;// Lee rol
    cin.ignore();// Limpia buffer
    return rol;// Devuelve rol seleccionado
}
// ============================================================
// FUNCION PRINCIPAL
// ============================================================
int main() {
    int rol, opcion;// Variables de rol y opcion
    bool salir = false;// Bandera de salida
    while (!salir) {// Bucle principal
        rol = seleccionarRol();// Selecciona rol
        if (rol == 0) {// Si el usuario desea salir
            cout << "Saliendo del sistema..." << endl;
            break;                                    
        }
        // ========================== EMPLEADO ==========================
        if (rol == 1) {
            do {
                mostrarMenuEmpleado();// Muestra menu empleado
                cin >> opcion;// Lee opcion
                cin.ignore();// Limpia buffer
                switch (opcion) {
                case 0:
                    salir = true;// Marca salida
                    break;
                case 13:// Volver al menu de rol
                    cout << "Volviendo al menu de seleccion de rol..." << endl;
                    opcion = 0;
                    break;
                case 10:// Agregar producto
                    agregarProducto();
                    break;
                case 11: {// Comprimir Huffman
                    string entrada, salida;
                    cout << "Ingrese archivo de entrada: ";
                    getline(cin, entrada);
                    cout << "Ingrese archivo de salida: ";
                    getline(cin, salida);
                    comprimirHuffmanArchivo(entrada, salida);
                    break;
                }
                case 18: {// Comprimir LZW
                    string entrada, salida;
                    cout << "Ingrese archivo de entrada: ";
                    getline(cin, entrada);
                    cout << "Ingrese archivo de salida: ";
                    getline(cin, salida);
                    comprimirLZWArchivo(entrada, salida);
                    break;
                }
                case 19: {// Descomprimir LZW
                    string entrada, salida;
                    cout << "Ingrese archivo comprimido (.lzw): ";
                    getline(cin, entrada);
                    cout << "Ingrese archivo de salida: ";
                    getline(cin, salida);
                    descomprimirLZWArchivo(entrada, salida);
                    break;
                }
                case 20: { // Menu grafico SFML para Empleado
                    cout << "Abriendo menu grafico (Empleado)..." << endl;
                    MenuSFML menuEmpleado(800, 600, "Menu Empleado - SFML");
                    menuEmpleado.run();
                    break;
                }
                default:
                    cout << "Opcion invalida." << endl;// Mensaje si opcion no valida
                }
                cout << endl;
            } while (opcion != 0 && opcion != 13 && !salir);
        }
        // ========================== CLIENTE ==========================
        else if (rol == 2) {
            do {
                mostrarMenuCliente();// Muestra menu cliente
                cin >> opcion;// Lee opcion
                cin.ignore();// Limpia buffer
                switch (opcion) {
                case 0:
                    salir = true;// Marca salida
                    break;
                case 3:// Volver al menu de rol
                    opcion = 0;
                    break;
                case 4: {// Comprimir LZW
                    string entrada, salida;
                    cout << "Ingrese archivo de entrada: ";
                    getline(cin, entrada);
                    cout << "Ingrese archivo de salida: ";
                    getline(cin, salida);
                    comprimirLZWArchivo(entrada, salida);
                    break;
                }
                case 5: {// Descomprimir LZW
                    string entrada, salida;
                    cout << "Ingrese archivo comprimido (.lzw): ";
                    getline(cin, entrada);
                    cout << "Ingrese archivo de salida: ";
                    getline(cin, salida);
                    descomprimirLZWArchivo(entrada, salida);
                    break;
                }
                case 6: { // Menu grafico SFML para Cliente
                    cout << "Abriendo menu grafico (Cliente)..." << endl;
                    MenuSFML menuCliente(800, 600, "Menu Cliente - SFML");
                    menuCliente.run();
                    break;
                }
                default:
                    cout << "Opcion invalida." << endl;// Mensaje si opcion no valida
                }
                cout << endl;
            } while (opcion != 0 && opcion != 3 && !salir);
        }
    }
    return 0;      
}