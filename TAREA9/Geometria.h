#ifndef GEOMETRIA_H          // Evita inclusion multiple del archivo
#define GEOMETRIA_H
#include <vector>            // Permite usar el contenedor vector
#include <cmath>             // Permite usar funciones matematicas como sqrt
using namespace std;         
// ----------------------------------------------------
// ESTRUCTURA POINT: representa un punto en el plano 2D
// ----------------------------------------------------
struct Point {
    double x, y;             // Coordenadas x e y del punto
    // Constructor por defecto, inicializa el punto en el origen (0,0)
    Point() : x(0), y(0) {}
    // Constructor con parametros para definir las coordenadas
    Point(double _x, double _y) : x(_x), y(_y) {}
};
// ----------------------------------------------------
// ESTRUCTURA POLYGON: representa un poligono en 2D
// ----------------------------------------------------
struct Polygon {
    vector<Point> pts;       // Vector que almacena los vertices del poligono
};
// ----------------------------------------------------
// FUNCION DIST: calcula la distancia entre dos puntos
// ----------------------------------------------------
inline double dist(const Point& a, const Point& b) {
    double dx = a.x - b.x;   // Diferencia en el eje X
    double dy = a.y - b.y;   // Diferencia en el eje Y
    return sqrt(dx * dx + dy * dy);  // Aplica formula de distancia euclidiana
}
#endif // GEOMETRIA_H
