#pragma once
#include "enemy.h"

class ImperialInterceptor : public EnemyShip {

    public:
        ImperialInterceptor(int xpos, int ypos);
        void update(const ofPoint& playerPos) override;
        void draw() override;
        void shoot() override;
        float speed = 0.5;
        int health = 2;
 


    
};

