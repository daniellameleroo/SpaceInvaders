#include "SpiralBoss.h"
// Include any necessary headers

SpiralBoss::SpiralBoss(int xpos, int ypos, string name) : Boss(xpos, ypos, 0.5, 250, name) {
    enemyShipSprite.load("CompressedImages/secondShip.png");
    pos.x = ofGetWidth() / 2;
    pos.y = ofGetWidth() / 2; 
    enemyHitBox = new HitBox(pos.x - 40, pos.y - 40, 90, 90);
    shootingPoint = ofPoint(pos.x - 10, pos.y - 10);
}   

    

void SpiralBoss::update(const ofPoint& playerPos) {
    shotTimer++;
    if(shotTimer % 30 == 0){
        shoot();
    }
}

void SpiralBoss::draw() {
    ofPushMatrix();
    ofTranslate(pos.x, pos.y);
    enemyShipSprite.draw(-40, -40, 90, 90);
    ofPopMatrix();
    showBossHealth();
}

void SpiralBoss::shoot() {
    const int numBullets = 20; 
    const float initialRadius = 10.0; 
    const float radiusIncrement = 5.0;
    float currentRadius = initialRadius;

    for(int i=0; i < numBullets; i++){
        float angle = ofMap(i, 0, numBullets, 0, 360);
        float radians = ofDegToRad(angle);

        Projectiles bullet(ofPoint(shootingPoint.x, shootingPoint.y), angle);
        bullet.setColors(ofColor::blue, ofColor::purple);
        enemyBullets.push_back(bullet);
        SoundManager::playSong("bulletSound",false);

    }

}