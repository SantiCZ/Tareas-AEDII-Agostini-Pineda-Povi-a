#ifndef VORONOI_H // Evita que el archivo se incluya mas de una vez
#define VORONOI_H
#include <SFML/Graphics.hpp>// Libreria para graficos
#include "Geometria.h"// Incluye la estructura Point y la funcion dist
using namespace sf; // Usa el espacio de nombres de SFML
// ------------------------------------------------------------
// FUNCION drawVoronoi
// Dibuja un diagrama de Voronoi en la ventana dada, basado en las semillas
// Cada pixel se colorea segun la semilla mas cercana
// ------------------------------------------------------------
inline void drawVoronoi(RenderWindow& win, const vector<Point>& seeds) {
    Image img;// Crea una imagen en memoria
    img.create(800, 600, Color::White);// Define tamano y color de fondo
    // Recorre todos los pixeles de la imagen
    for (int x = 0; x < 800; x++) {
        for (int y = 0; y < 600; y++) {
            Point p{ (float)x, (float)y }; // Crea un punto en la posicion actual
            float best = 1e9;// Distancia minima inicial muy grande
            int idx = 0;// Indice del punto mas cercano
            // Busca la semilla mas cercana
            for (size_t i = 0; i < seeds.size(); i++) {
                float d = dist(p, seeds[i]);// Calcula distancia al punto i
                if (d < best) {// Si es menor, actualiza el mejor
                    best = d;
                    idx = i;
                }
            }
            // Paleta de colores basica para las regiones
            Color colors[5] = {
                Color::Yellow,
                Color::Cyan,
                Color::Green,
                Color::Magenta,
                Color::Blue
            };
            // Asigna el color segun el indice de la semilla
            img.setPixel(x, y, colors[idx % 5]);
        }
    }
    Texture tex;// Crea una textura
    tex.loadFromImage(img);// Carga la imagen generada
    Sprite sp(tex);// Crea un sprite a partir de la textura
    win.draw(sp);// Dibuja el sprite en la ventana
}
#endif // VORONOI_H
