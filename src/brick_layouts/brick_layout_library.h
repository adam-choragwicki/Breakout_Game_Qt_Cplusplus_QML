#pragma once

#include <QColor>

struct BrickLayoutEntry
{
    qreal xRel; // 0.0–1.0 relative X
    qreal yRel; // 0.0–1.0 relative Y
    qreal wRel; // width relative to screen width
    qreal hRel; // height relative to screen height
    QColor color;
};

class BrickLayoutLibrary
{
public:
    static std::vector <BrickLayoutEntry> defaultPattern();
    static std::vector <BrickLayoutEntry> classicPattern();
    static std::vector <BrickLayoutEntry> pyramidPattern();
    static std::vector <BrickLayoutEntry> checkerboardPattern();
    static std::vector<BrickLayoutEntry> smilePattern();
    static std::vector<BrickLayoutEntry> manualPattern();
};
