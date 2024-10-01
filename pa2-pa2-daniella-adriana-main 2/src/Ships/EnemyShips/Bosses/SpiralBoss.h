#pragma once
#include "Boss.h"

class SpiralBoss : public Boss {
private:
    ofPoint shootingPoint;
    ofPoint leftTriangleEdge;
    ofPoint rightTriangleEdge; 

public:
    SpiralBoss(int xpos, int ypos, string name);
    void update(const ofPoint& playerPos) override;
    void draw() override;
    void shoot() override;
};