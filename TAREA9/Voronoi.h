#ifndef VORONOI_H
#define VORONOI_H
#include <SFML/Graphics.hpp>
#include "Geometria.h"
using namespace sf;

inline void drawVoronoi(RenderWindow& win, const vector<Point>& seeds) {
    Image img;
    img.create(800, 600, Color::White);

    for (int x = 0; x < 800; x++) {
        for (int y = 0; y < 600; y++) {
            Point p{ (float)x,(float)y };
            float best = 1e9; int idx = 0;
            for (size_t i = 0; i < seeds.size(); i++) {
                float d = dist(p, seeds[i]);
                if (d < best) { best = d; idx = i; }
            }
            Color colors[5] = { Color::Yellow,Color::Cyan,Color::Green,Color::Magenta,Color::Blue };
            img.setPixel(x, y, colors[idx % 5]);
        }
    }

    Texture tex;
    tex.loadFromImage(img);
    Sprite sp(tex);
    win.draw(sp);
}
#endif
