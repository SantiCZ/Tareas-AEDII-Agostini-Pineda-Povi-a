#include <iostream>
#include <vector>
#include <random>
#include <ctime>

// Headers del proyecto
#include "Geometria.h"
#include "SegmentIntersect.h"
#include "PointInPolygon.h"
#include "NearestNeighbor.h"
#include "RangeQuery.h"
#include "delaunay.h"
#include "algoritmos.h"

using namespace std;

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    // Generar puntos aleatorios (landmarks/obstáculos)
    vector<Point> points;
    for (int i = 0; i < 30; i++) {
        double x = (double)rand() / RAND_MAX * 10.0;
        double y = (double)rand() / RAND_MAX * 10.0;
        points.push_back({ x, y });
    }

    // KD-tree + consultas NN y rango
    KDTree* kd = build_kdtree(points, 0);
    Point query = { 5, 5 };
    auto [nn, d2] = nearest(kd, query);
    cout << "Nearest to (5,5): (" << nn.x << ", " << nn.y << ") dist = " << sqrt(d2) << endl;

    auto inRange = range_query(kd, query, 2.0);
    cout << "Points within r=2: " << inRange.size() << endl;

    // Triangulación Delaunay
    auto triangles = bowyer_watson(points);

    // Construir Voronoi a partir de Delaunay
    auto [centers, edges] = build_voronoi(triangles);

    // Probar punto-en-polígono con el primer triángulo
    if (!triangles.empty()) {
        auto tri = triangles[0];
        Point centroid = { (tri[0].x + tri[1].x + tri[2].x) / 3.0,
                          (tri[0].y + tri[1].y + tri[2].y) / 3.0 };
        bool inside = point_in_polygon(centroid, { tri[0], tri[1], tri[2] });
        cout << "Centroid inside first triangle? " << (inside ? "Yes" : "No") << endl;
    }

    // Ejemplo de intersección de segmentos
    Segment s1 = { {0,0}, {5,5} };
    Segment s2 = { {0,5}, {5,0} };
    cout << "Segments intersect? " << (segments_intersect(s1, s2) ? "Yes" : "No") << endl;

    // Visualización/exportación se puede agregar aquí (SVG, etc.)

    return 0;
}
