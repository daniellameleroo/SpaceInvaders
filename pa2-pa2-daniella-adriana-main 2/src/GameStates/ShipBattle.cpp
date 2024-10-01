
#include "ShipBattle.h"
#include "EnemyManager.h"
#include "Boss.h"


// ====================================
// Constructor & Destructor Section
// ====================================

ShipBattle::ShipBattle() {
    this->player = new Player(ofGetWidth() / 2, ofGetHeight() / 2);
    this->playerScore = 0;
    this->killspreeTimer = 0;
    this->shieldBar = 0;

    
    font.load("Fonts/Orbitron.ttf", 20, true);
    indicatorFont.load("Fonts/Orbitron.ttf", 10, true);
    backgroundImage.load("Menu_Images/BattleArea.jpg");
    shieldSprite.load("CompressedImages/ForceShield.png");
    bombSprite.load("CompressedImages/Bomb-min.png");

}

// ====================================
// Core Functionality Section
// ====================================


// Update Method
void ShipBattle::update() {
    // Boss spawn logic
    if (EnemyManager::isBossSpawning()) {
        displayBossWarning = true;
        SoundManager::stopSong("battle");
    } 
    else {
        displayBossWarning = false;
    }
    
    // Update enemies and player
    EnemyManager::updateEnemies(player);
    this->player->processPressedKeys();
    this->player->update();
    wrapCoords(this->player->pos);
    if(shieldActive){
        player->health = 100;
    }
    if (shieldActive) {
        if (shieldTimer % 60 == 0) {
            this->player->shield--;
            if (this->player->shield <= 0) {
                this->player->shield = 0; 
                shieldActive = false; 
            }
        }
        shieldTimer--;
        if (shieldTimer <= 0) {
            shieldActive = false; 
        }
    }
    if(EnemyManager::bossKilled && !bombReady ){
        bombReady = true;
        EnemyManager::changeShip = true;
        
    }

    
    // Update scoring and kill spree timer
    playerScore += EnemyManager::getPointsScored() * scoreMultiplier() * killSpreeMode();
    this->player->setScore(playerScore);
    killspreeTimer = EnemyManager::getKillSpreeTimer();
    shieldBar = EnemyManager::getShieldBar();

    // Update player projectiles
    if (!this->player->bullets.empty()) {
        updateBullets();
    }

    // State switching logic for when the player dies
    if (this->player->health <= 0) 
           if (this->player->health <= 0) {
        if (livesRemaining > 0) {
     // Reset ship's health and decrement lives remaining
            this->player->health = 100;
            livesRemaining--;
        } else {
    
        this->setNextState("GameOverState");
        SoundManager::stopSong("battle");
        if(EnemyManager::getSpawningBossType() != ""){
            SoundManager::stopSong(EnemyManager::getSpawningBossType());
        }
            // Write the current score to a file
            ofstream scoreFile("currentScore.txt");
            if (scoreFile.is_open()) {
                scoreFile << playerScore;
                scoreFile.close();
            }
            this->setFinished(true);
    }
    
}


}


//====== Draw Method ====== 
void ShipBattle::draw() {
    ofSetBackgroundColor(ofColor::black);
    backgroundImage.draw(0, 0, ofGetWidth(), ofGetHeight());
    
    // Draw the score
    ofSetColor(ofColor::white);
    font.drawString("SCORE " + to_string(playerScore), ofGetWidth() / 2 - 60, 50);
    if(player->shield == 10){
        font.drawString("SHIELD IS READY",ofGetWidth() / 2 - 120, 100);
    }
    if(shieldActive == true and player->shield > 0){
        shieldSprite.draw(player->pos.x/2,player->pos.y/2);
    }
    if(EnemyManager::bossKilled){
        font.drawString("BOMB IS READY",ofGetWidth() / 2 - 120, 75);
    }
    // Draw enemies and player
    EnemyManager::drawEnemies();
    if(EnemyManager::changeShip){
        player->draw2();
    }
    else{
        player->draw();
    }
    
    draw_bullets();    
    // Draw boss warning if needed
    if (displayBossWarning) {
        // Semi-transparent red screen tint
        ofSetColor(255, 0, 0, 128);
        ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

        // Warning text
        ofSetColor(255);
        font.drawString("BOSS APPROACHING!", ofGetWidth() / 2 - 100, ofGetHeight() / 2);
    }

    // Draw UI elements
    healthBar(player->health, 100);
    killSpreeTimer(this->killspreeTimer, 150);
    shieldBAR(player->shield,10);
    lifeIndicator();     

    //Draw a mini box for the bomb. Make sure to draw the bomb inside this box.
        ofNoFill();
        ofDrawRectangle(ofGetWidth() - 150, 30, 50, 50);
    if(EnemyManager::bossKilled){
        bombSprite.draw(ofGetWidth()-150,30,50,50);
    }
        
        ofFill();
    
}

// ====================================
// Input Handling Section
// ====================================

void ShipBattle::keyPressed(int key) {
    player->addPressedKey(key);

    //DEBUG KEYS - Press these keys to ease the debugging with the game
    if(key == 'k') {
        EnemyManager::toggleHitBoxes = !EnemyManager::toggleHitBoxes; 
        player->showHitbox = !player->showHitbox;
    }
    if(key == 'o')  player->health = 100;
    if(key == 'p')  playerScore += 10000;
    if(key == 'r') player->shield = 10;
    if(key == 'f') player->shield -= 1;
    if(key == 'q'){
        if(player->shield == 10){
            shieldActive = true;
            shieldTimer = 10 * 60;   
        }
    }
    if(key == 'e'){
        if(EnemyManager::bossKilled){
            bomb();
            int bombDamage = 200; // Adjust as needed
            EnemyManager::damageBosses(bombDamage);
            EnemyManager::bossKilled = false;
            bombUsed = true;
            bombReady = false;

            
        }
    }
    // if(key == 'q') {
    //     if(counter){
    //         EnemyManager::shieldActive = true;
    //     }
    // }
}

void ShipBattle::keyReleased(int key) {
    key = tolower(key);
    this->player->removePressedKey(key);

}

void ShipBattle::mousePressed(int x, int y, int button) {
    // Implementation for mousePressed event in case you want to do something with this
}

// ====================================
// Helper Methods Section
// ====================================

void ShipBattle::wrapCoords(ofPoint &currentPos) {
    // Screen wrapping logic for player ship
    if (currentPos.x < 0.0) currentPos.x = ofGetWidth() - 10;
    if (currentPos.x >= ofGetWidth()) currentPos.x = 10;
    if (currentPos.y < 0.0) currentPos.y = ofGetHeight() - 10;
    if (currentPos.y >= ofGetHeight()) currentPos.y = 10;
}

void ShipBattle::draw_bullets() {
    // Draw all player bullets

    for (auto &bullet : this->player->bullets) {
        bullet.draw();
    }
  
}

void ShipBattle::updateBullets() {
    // Update logic for player bullets
    for (auto &bullet : this->player->bullets) {
    
       
     bullet.update();

    }
}

// ====================================
// UI and Feedback Methods Section
// ====================================
void ShipBattle::healthBar(int currHealth, int maxHealth) {
    indicatorFont.drawString("HEALTH", 10, 30);
    ofNoFill();
    ofDrawRectangle(10, 40, maxHealth *2, 20);
    ofFill();
    ofSetColor(ofColor::green);
    ofDrawRectangle(10, 40, currHealth *2, 20);
    ofSetColor(ofColor::white);
}

void ShipBattle::killSpreeTimer(int currTimer, int maxTimer) {
    indicatorFont.drawString("KILL SPREE", 10, 80);
    ofNoFill();
    ofDrawRectangle(10, 90, maxTimer, 10);
    ofFill();
    ofSetColor(ofColor::red);
    ofDrawRectangle(10, 90, currTimer, 10);
    ofSetColor(ofColor::white);
}

void ShipBattle::shieldBAR(int currS, int maxT){
    indicatorFont.drawString("FORCE SHIELD", 10, 740);
    ofNoFill();
    ofDrawRectangle(10, 750, maxT*10, 20);
    ofFill();
    ofSetColor(ofColor::blue);
    ofDrawRectangle(10, 750, currS*10, 20);
    ofSetColor(ofColor::white);
}
void ShipBattle::lifeIndicator() {
    indicatorFont.drawString("LIFE INDICATOR", 10, 123);
    
    for (int i = 0; i < livesRemaining; ++i) {
        ofNoFill();
        ofDrawCircle(20 + i * 25, 140, 10); 
        ofFill();
        ofSetColor(ofColor::red);
        ofDrawCircle(20 + i *25, 140,10 );
        ofSetColor(ofColor::white);
        
        // ofSetColor(ofColor::azure);
        
    }
}



// ====================================
// Game Mechanics Methods Section
// ====================================

double ShipBattle::killSpreeMode() {
    // Logic for kill spree mode bonus
    return this->killspreeTimer > 0 ? 1.5 : 1.0;
}

double ShipBattle::scoreMultiplier() {
    // Logic for score multiplier based on current score
    if (playerScore >= 350) return 3.5;
    else if (playerScore >= 200) return 2.5;
    else if (playerScore >= 100) return 1.5;
    else return 1.0; // Default multiplier
}

void ShipBattle::decrementShield(){
    if(this->player->shield >= 0){
        player->shield--;
    }
    if(this->player->shield <= 0){
        shieldActive = false;
    }

}

void ShipBattle::bomb(){
    EnemyManager::removeEnemiesOnlyCleanUp();
    EnemyManager::removeEnemiesOnly();
    bombReady = false;
}

void ShipBattle::reset(){
    playerScore = 0;
    killspreeTimer = 0;
    shieldBar = 0;
    livesRemaining = 2;
    EnemyManager::changeShip = false;
    EnemyManager::bossKilled = false;
    battle = new ShipBattle;
    EnemyManager::cleanUp();
    EnemyManager::removeEnemies();
    player = new Player(ofGetWidth()/2, ofGetHeight()/2);
    setFinished(false);
    setNextState("");
}


                                        
