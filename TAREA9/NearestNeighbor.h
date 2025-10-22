#ifndef NEARESTNEIGHBOR_H
#define NEARESTNEIGHBOR_H
#include "Geometria.h"
inline Point nearestNeighbor(const Point& q, const vector<Point>& pts) {
    Point best = pts[0];
    float dmin = dist(q, best);
    for (auto& p : pts) {
        float d = dist(q, p);
        if (d < dmin) {
            dmin = d;
            best = p;
        }
    }
    return best;
}
#endif
