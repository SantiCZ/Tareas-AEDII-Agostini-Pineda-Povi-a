#ifndef RANGEQUERY_H        // Evita inclusiones multiples del mismo archivo
#define RANGEQUERY_H
#include "Geometria.h"      // Incluye la definicion de Point y Polygon
#include "PointInPolygon.h" // Incluye la funcion que verifica si un punto esta dentro de un poligono
// ------------------------------------------------------------
// FUNCION rangeQuery
// Devuelve todos los puntos que se encuentran dentro de un poligono
// ------------------------------------------------------------
inline vector<Point> rangeQuery(const vector<Point>& pts, const Polygon& poly) {
    vector<Point> result;// Vector para almacenar los puntos dentro del poligono
    // Recorre todos los puntos de entrada
    for (auto& p : pts)
        // Si el punto esta dentro del poligono, se agrega al resultado
        if (pointInPolygon(p, poly))
            result.push_back(p);
    return result;// Devuelve el vector con los puntos contenidos
}
#endif // RANGEQUERY_H
