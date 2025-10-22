#ifndef NEARESTNEIGHBOR_H     // Evita incluir el archivo varias veces
#define NEARESTNEIGHBOR_H
#include "Geometria.h"        // Incluye las definiciones de Point y dist
// ------------------------------------------------------------
// FUNCION NEARESTNEIGHBOR
// Busca el punto mas cercano al punto q dentro del vector pts
// ------------------------------------------------------------
inline Point nearestNeighbor(const Point& q, const vector<Point>& pts) {
    Point best = pts[0];          // Toma el primer punto como el mas cercano inicial
    float dmin = dist(q, best);   // Calcula la distancia inicial minima

    // Recorre todos los puntos en el vector
    for (auto& p : pts) {
        float d = dist(q, p);// Calcula la distancia entre q y el punto actual
        if (d < dmin) { // Si encuentra una distancia menor
            dmin = d;// Actualiza la distancia minima
            best = p; // Guarda el punto mas cercano
        }
    }
    return best; // Retorna el punto mas cercano encontrado
}
#endif // NEARESTNEIGHBOR_H
