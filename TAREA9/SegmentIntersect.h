#ifndef SEGMENTINTERSECT_H       // Evita que el archivo se incluya mas de una vez
#define SEGMENTINTERSECT_H
#include "Geometria.h"            // Incluye la definicion de Point
// FUNCION cross
// Calcula el producto cruzado de los vectores AB y AC
// Devuelve un valor positivo, negativo o cero segun la orientacion
// ------------------------------------------------------------
inline float cross(const Point& a, const Point& b, const Point& c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    // Si el resultado es positivo -> C esta a la izquierda de AB
    // Si es negativo -> C esta a la derecha de AB
    // Si es cero -> Son colineales
}
// ------------------------------------------------------------
// FUNCION intersect
// Determina si los segmentos AB y CD se intersectan
// ------------------------------------------------------------
inline bool intersect(const Point& A, const Point& B, const Point& C, const Point& D) {
    // Los segmentos se cruzan si los puntos C y D estan en lados opuestos de AB
    // y los puntos A y B estan en lados opuestos de CD
    return (cross(A, B, C) * cross(A, B, D) < 0 &&
            cross(C, D, A) * cross(C, D, B) < 0);
}
#endif // SEGMENTINTERSECT_H
