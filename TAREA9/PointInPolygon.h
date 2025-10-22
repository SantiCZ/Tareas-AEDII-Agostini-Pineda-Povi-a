#ifndef POINTINPOLYGON_H
#define POINTINPOLYGON_H
#include "Geometria.h"

inline bool pointInPolygon(const Point& p, const Polygon& poly) {
    bool inside = false;
    int n = poly.pts.size();
    for (int i = 0, j = n - 1; i < n; j = i++) {
        if (((poly.pts[i].y > p.y) != (poly.pts[j].y > p.y)) &&
            (p.x < (poly.pts[j].x - poly.pts[i].x) *
                (p.y - poly.pts[i].y) / (poly.pts[j].y - poly.pts[i].y) + poly.pts[i].x))
            inside = !inside;
    }
    return inside;
}
#endif
