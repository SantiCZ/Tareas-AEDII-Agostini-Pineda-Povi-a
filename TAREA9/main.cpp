#include <SFML/Graphics.hpp>     // Libreria SFML para graficos y manejo de eventos
#include <iostream>              // Entrada y salida estandar
#include "Geometria.h"           // Estructuras basicas de punto y poligono
#include "PointInPolygon.h"      // Funcion para verificar si un punto esta dentro de un poligono
#include "SegmentIntersect.h"    // Permite verificar interseccion de segmentos
#include "RangeQuery.h"          // Consulta de rango (no usada directamente aqui)
#include "NearestNeighbor.h"     // Busqueda del deposito mas cercano
#include "Voronoi.h"             // Genera diagrama de Voronoi segun depositos
using namespace std;
using namespace sf;
int main() {
    // ----------------------------------------------------------
    // Crear ventana principal SFML
    // ----------------------------------------------------------
    RenderWindow win(VideoMode(800, 600), "Demo Modular Logistica - Dinamico");
    // ----------------------------------------------------------
    // Vectores para almacenar los puntos
    // ----------------------------------------------------------
    vector<Point> deposits;   // Puntos de deposito
    vector<Point> clientes;   // Puntos de clientes
    Polygon barrio;// Poligono que representa el barrio o zona
    bool drawingPolygon = false; // Estado para saber si se esta dibujando el poligono
    // ----------------------------------------------------------
    // Mostrar controles en consola
    // ----------------------------------------------------------
    cout << "Controles:" << endl;
    cout << " - Click izquierdo: agregar deposito" << endl;
    cout << " - Click derecho: agregar cliente" << endl;
    cout << " - Tecla P: comenzar/terminar poligono (barrio)" << endl;
    cout << " - Tecla R: resetear todo" << endl;
    cout << " - Cerrar ventana para salir" << endl;
    // ----------------------------------------------------------
    // Bucle principal de la aplicacion
    // ----------------------------------------------------------
    while (win.isOpen()) {
        Event e;  // Objeto para manejar eventos
        // ------------------------------------------------------
        // Procesamiento de eventos (mouse, teclado, cierre, etc.)
        // ------------------------------------------------------
        while (win.pollEvent(e)) {
            // Cerrar la ventana
            if (e.type == Event::Closed) win.close();
            // --------------------------------------------------
            // Eventos de click del mouse
            // --------------------------------------------------
            if (e.type == Event::MouseButtonPressed) {
                Point p{ (float)e.mouseButton.x, (float)e.mouseButton.y };  // Coordenadas del click
                if (e.mouseButton.button == Mouse::Left && !drawingPolygon) {
                    // Agregar deposito si no se esta dibujando el poligono
                    deposits.push_back(p);
                }
                else if (e.mouseButton.button == Mouse::Right) {
                    // Agregar cliente
                    clientes.push_back(p);
                }
            }
            // --------------------------------------------------
            // Eventos de teclado
            // --------------------------------------------------
            if (e.type == Event::KeyPressed) {
                // Tecla P: activar/desactivar modo de dibujo de poligono
                if (e.key.code == Keyboard::P) {
                    if (!drawingPolygon) {
                        barrio.pts.clear();
                        drawingPolygon = true;
                        cout << "Dibuja poligono: click izquierdo para vertices, Enter para terminar.\n";
                    }
                    else {
                        drawingPolygon = false;
                        cout << "Poligono finalizado.\n";
                    }
                }
                // Tecla Enter: cerrar el poligono actual
                if (e.key.code == Keyboard::Enter && drawingPolygon) {
                    drawingPolygon = false;
                    cout << "Poligono cerrado.\n";
                }
                // Tecla R: resetear todos los datos
                if (e.key.code == Keyboard::R) {
                    deposits.clear();
                    clientes.clear();
                    barrio.pts.clear();
                    cout << "Sistema reseteado.\n";
                }
            }
            // --------------------------------------------------
            // Agregar vertices al poligono mientras se dibuja
            // --------------------------------------------------
            if (e.type == Event::MouseButtonPressed && drawingPolygon) {
                if (e.mouseButton.button == Mouse::Left) {
                    barrio.pts.push_back({ (float)e.mouseButton.x, (float)e.mouseButton.y });
                }
            }
        }
        // ------------------------------------------------------
        // Dibujado de escena
        // ------------------------------------------------------
        win.clear(Color::White);   // Limpia la pantalla en blanco
        // Dibuja diagrama de Voronoi si hay depositos
        if (!deposits.empty())
            drawVoronoi(win, deposits);
        // ------------------------------------------------------
        // Dibuja el poligono del barrio (si existe)
        // ------------------------------------------------------
        if (!barrio.pts.empty()) {
            ConvexShape poly;
            poly.setPointCount(barrio.pts.size());   // Define cantidad de vertices
            for (size_t i = 0; i < barrio.pts.size(); ++i)
                poly.setPoint(i, { barrio.pts[i].x, barrio.pts[i].y });  // Asigna los puntos
            poly.setFillColor(Color(200, 200, 250, 80));  // Color semitransparente
            poly.setOutlineColor(Color::Blue); // Borde azul
            poly.setOutlineThickness(2); // Grosor del borde
            win.draw(poly);
        }
        // ------------------------------------------------------
        // Dibuja los depositos
        // ------------------------------------------------------
        for (auto& d : deposits) {
            CircleShape c(6);
            c.setPosition(d.x - 6, d.y - 6);
            c.setFillColor(Color::Red);   // Color rojo para depositos
            win.draw(c);
        }
        // ------------------------------------------------------
        // Dibuja los clientes
        // ------------------------------------------------------
        for (auto& cl : clientes) {
            CircleShape c(5);
            c.setPosition(cl.x - 5, cl.y - 5);
            bool in = (!barrio.pts.empty() ? pointInPolygon(cl, barrio) : false); // Verifica si esta en el barrio
            c.setFillColor(in ? Color::Green : Color::Black); // Verde si esta dentro, negro si no
            win.draw(c);
            // Dibuja linea al deposito mas cercano
            if (!deposits.empty()) {
                Point nn = nearestNeighbor(cl, deposits);
                Vertex line[] = {
                    Vertex({cl.x, cl.y}, Color::Magenta),
                    Vertex({nn.x, nn.y}, Color::Magenta)
                };
                win.draw(line, 2, Lines);
            }
        }
        // Muestra todo en pantalla
        win.display();
    }
}
