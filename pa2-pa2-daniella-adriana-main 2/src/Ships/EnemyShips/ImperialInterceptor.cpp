#include "ImperialInterceptor.h"


ImperialInterceptor::ImperialInterceptor(int xpos, int ypos) : EnemyShip(xpos, ypos, 3.5, 20, 50) {
    amIBoss = false;
    enemyShipSprite.load("CompressedImages/newEnemy-min.png");
    enemyShipSprite.rotate90(2);
    enemyHitBox = new HitBox(pos, enemyShipSprite.getWidth() * 0.1, enemyShipSprite.getHeight() * 0.1);
}

void ImperialInterceptor::update(const ofPoint& playerPos) {
    // Calculate the angle to the player
        ofVec2f toPlayer = playerPos - pos;
        float angleToPlayer = ofRadToDeg(-atan2(toPlayer.x, toPlayer.y));

    // Update ship's orientation
        shipOrientation = angleToPlayer;
        

    // Move towards the player
        pos += toPlayer.getNormalized() * speed;
        enemyHitBox->box.setPosition(pos.x - 15, pos.y - 10);

    this->shotTimer++;
    // Check if the ship should shoot
        if (shotTimer % 50 == 0) {
            shoot();
            shotTimer = 0; //resets the timer
        }
}

void ImperialInterceptor::draw() {
    ofPushMatrix();
    ofTranslate(pos.x, pos.y);
    ofRotateDeg(shipOrientation);
    enemyShipSprite.draw(-32, -32, 60, 60);
    ofPopMatrix();
    
    if(showHitboxes){
        enemyHitBox->draw();
    }
}

void ImperialInterceptor::shoot() {
    // Create a new projectile and add it to the list of bullets 
    Projectiles p = Projectiles(pos, shipOrientation + 95);
    p.setColors(ofColor::red, ofColor::orange);
    enemyBullets.push_back(p);
    SoundManager::playSong("bulletSound", false);

    Projectiles p2 = Projectiles(pos, shipOrientation + 90); //phase 2 new bullet
    p2.setColors(ofColor::red, ofColor::orange);
    enemyBullets.push_back(p2);
    SoundManager::playSong("bulletSound", false);


}

