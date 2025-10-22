#ifndef POINTINPOLYGON_H       // Evita que el archivo se incluya mas de una vez
#define POINTINPOLYGON_H
#include "Geometria.h"         // Incluye las definiciones de Point y Polygon
// ------------------------------------------------------------
// FUNCION pointInPolygon
// Determina si un punto p se encuentra dentro de un poligono
// Usa el algoritmo del rayo (ray casting algorithm)
// ------------------------------------------------------------
inline bool pointInPolygon(const Point& p, const Polygon& poly) {
    bool inside = false;// Variable booleana que indica si el punto esta dentro o fuera
    int n = poly.pts.size(); // Numero de vertices del poligono
    // Recorre todos los lados del poligono
    for (int i = 0, j = n - 1; i < n; j = i++) {
        // Verifica si el punto cruza un lado del poligono
        if (((poly.pts[i].y > p.y) != (poly.pts[j].y > p.y)) && // Comprueba si el rayo horizontal cruza el lado
            (p.x < (poly.pts[j].x - poly.pts[i].x) * // Calcula la interseccion del rayo con el lado
                (p.y - poly.pts[i].y) / (poly.pts[j].y - poly.pts[i].y) + poly.pts[i].x))
            inside = !inside;// Cambia el estado si hay interseccion
    }
    return inside;// Devuelve true si el punto esta dentro, false si esta fuera
}
#endif // POINTINPOLYGON_H
