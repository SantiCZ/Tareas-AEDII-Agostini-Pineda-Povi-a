#ifndef RANGEQUERY_H
#define RANGEQUERY_H
#include "Geometria.h"
#include "PointInPolygon.h"
inline vector<Point> rangeQuery(const vector<Point>& pts, const Polygon& poly) {
    vector<Point> result;
    for (auto& p : pts)
        if (pointInPolygon(p, poly)) result.push_back(p);
    return result;
}
#endif
