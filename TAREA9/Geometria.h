#ifndef GEOMETRIA_H
#define GEOMETRIA_H

#include <vector>
#include <cmath>
using namespace std;

// ----------------------
// Definicion de Point
// ----------------------
struct Point {
    double x, y;
    Point() : x(0), y(0) {}
    Point(double _x, double _y) : x(_x), y(_y) {}
};

// ----------------------
// Definicion de Polygon
// ----------------------
struct Polygon {
    vector<Point> pts;
};

// ----------------------
// Funcion de distancia
// ----------------------
inline double dist(const Point& a, const Point& b) {
    double dx = a.x - b.x, dy = a.y - b.y;
    return sqrt(dx * dx + dy * dy);
}

#endif
