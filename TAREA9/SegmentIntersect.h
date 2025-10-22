#ifndef SEGMENTINTERSECT_H
#define SEGMENTINTERSECT_H
#include "Geometria.h"

inline float cross(const Point& a, const Point& b, const Point& c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

inline bool intersect(const Point& A, const Point& B, const Point& C, const Point& D) {
    return (cross(A, B, C) * cross(A, B, D) < 0 &&
        cross(C, D, A) * cross(C, D, B) < 0);
}
#endif
