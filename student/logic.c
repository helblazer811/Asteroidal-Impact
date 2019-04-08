#include "logic.h"
#include "gba.h"
#include "images/galaga_ship_sprite.h"
#include "images/friendly_laser.h"
#include <stdlib.h>

void initializeAppState(AppState* appState) {
    // TA-TODO: Initialize everything that's part of this AppState struct here.
    // Suppose the struct contains random values, make sure everything gets
    // the value it should have when the app begins.
    appState->gameOver = 0;
    appState->score = 0;
    appState->ship = PlayerShipNew(HEIGHT - GALAGA_SHIP_SPRITE_HEIGHT*2, WIDTH/2 - GALAGA_SHIP_SPRITE_WIDTH/2, 0, 0, 3);
    appState->numAsteroids = 0;
    appState->asteroids = malloc(20 * sizeof(Asteroid*));
    appState->numEnemyShips = 0;
    appState->enemyShips = malloc(20 * sizeof(EnemyShip*));
    appState->numFriendlyProjectiles = 0;
    appState->friendlyProjectiles = malloc(5 * sizeof(FriendlyProjectile*));
    appState->numEnemyProjectiles = 0;
    appState->enemyProjectiles = malloc(20 * sizeof(EnemyProjectile*));
    appState->counter = 0;
}


Point* PointNew(int r, int c) {
    Point * ret = malloc(sizeof(Point));

    ret->r = r;
    ret->c = c;

    return ret;
}

PlayerShip* PlayerShipNew(int r, int c, int vr, int vc, int lives){
    PlayerShip* ret = malloc(sizeof(PlayerShip));

    ret->location = PointNew(r, c);
    ret->velocity = PointNew(vr, vc);
    ret->lives = lives;
    ret->shotCooldown = 60;

    return ret;
}

Asteroid* AsteroidNew(int r, int c, int vr, int vc, u16* image, int width, int height, int lives) {
    Asteroid* ret = malloc(sizeof(Asteroid));

    ret->location = PointNew(r, c);
    ret->velocity = PointNew(vr, vc);
    ret->size = PointNew(height, width);
    ret->image = image;
    ret->lives = lives;

    return ret;
}

EnemyShip* EnemyShipNew(int r, int c, int vr, int vc,  int lives){
    EnemyShip* ret = malloc(sizeof(EnemyShip));

    ret->location = PointNew(r, c);
    ret->velocity = PointNew(vr, vc);
    ret->lives = lives;

    return ret;
}

FriendlyProjectile* FriendlyProjectileNew(int r, int c, int vr, int vc){
    FriendlyProjectile* ret = malloc(sizeof(FriendlyProjectile));

    ret->location = PointNew(r, c);
    ret->velocity = PointNew(vr, vc);

    return ret;
}

EnemyProjectile* EnemyProjectileNew(int r, int c, int vr, int vc){
    EnemyProjectile* ret = malloc(sizeof(EnemyProjectile));

    ret->location = PointNew(r, c);
    ret->velocity = PointNew(vr, vc);

    return ret;
}

void freePoint(Point* point) {
    free(point);
}

void freeFriendlyProjectile(FriendlyProjectile* friendlyProjectile) {
    free(friendlyProjectile->location);
    free(friendlyProjectile->velocity);
    free(friendlyProjectile);
}


void setPlayerVelocities(PlayerShip* ship, u32 keysPressedNow) {
    //process the buttons
    //use the arrow keys to set the players velocities
    if (KEY_DOWN(BUTTON_LEFT,keysPressedNow)) {
        ship->velocity->c = -1*VELOCITY;
    }else if (KEY_DOWN(BUTTON_RIGHT,keysPressedNow)) {
        ship->velocity->c = VELOCITY;
    } else {
        ship->velocity->c = 0;
    }

    if (KEY_DOWN(BUTTON_RIGHT,keysPressedNow) 
        && KEY_DOWN(BUTTON_LEFT,keysPressedNow)) {
        ship->velocity->c = 0;
    }

    if (KEY_DOWN(BUTTON_UP,keysPressedNow)) {
        ship->velocity->r = -1*VELOCITY;
    } else if (KEY_DOWN(BUTTON_DOWN,keysPressedNow)) {
        ship->velocity->r = VELOCITY;
    } else {
        ship->velocity->r = 0;
    }

    if (KEY_DOWN(BUTTON_UP,keysPressedNow) 
        && KEY_DOWN(BUTTON_DOWN,keysPressedNow)) {
        ship->velocity->r = 0;
    }

    //location constraints

}

void setPlayerPosition(PlayerShip* ship) {
    ship->location->r += ship->velocity->r;
    ship->location->c += ship->velocity->c;
}

void addFriendlyProjectile(AppState* currentAppState, u32 keysPressedNow) {
    if (KEY_DOWN(BUTTON_A, keysPressedNow) && currentAppState->ship->shotCooldown == 0) {
        currentAppState->numFriendlyProjectiles = (currentAppState->numFriendlyProjectiles + 1) % 5;
        currentAppState->ship->shotCooldown = 30;
        if (currentAppState->friendlyProjectiles[currentAppState->numFriendlyProjectiles] != NULL) {
            freeFriendlyProjectile(currentAppState->friendlyProjectiles[currentAppState->numFriendlyProjectiles]);
        }
        currentAppState->friendlyProjectiles[currentAppState->numFriendlyProjectiles % 5] = 
            FriendlyProjectileNew(currentAppState->ship->location->r - FRIENDLY_LASER_HEIGHT,
                                  currentAppState->ship->location->c + GALAGA_SHIP_SPRITE_WIDTH / 2 - FRIENDLY_LASER_WIDTH/2,
                                  -2,
                                   0);
        
    }
}

void setFriendlyProjectilePositions(FriendlyProjectile** friendlyProjectiles, int numFriendlyProjectiles) {
    for (int i = 0; i< numFriendlyProjectiles; i++) {
        friendlyProjectiles[i]->location->r += friendlyProjectiles[i]->velocity->r;
        friendlyProjectiles[i]->location->c += friendlyProjectiles[i]->velocity->c;
    }
}

// TA-TODO: Add any process functions for sub-elements of your app here.
// For example, for a snake game, you could have a processSnake function
// or a createRandomFood function or a processFoods function.
//
// e.g.:
// static Snake processSnake(Snake* currentSnake);
// static void generateRandomFoods(AppState* currentAppState, AppState* nextAppState);

// This function processes your current app state and returns the new (i.e. next)
// state of your application.
AppState processAppState(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow) {
    /* TA-TODO: Do all of your app processing here. This function gets called
     * every frame.
     *
     * To check for key presses, use the KEY_JUST_PRESSED macro for cases where
     * you want to detect each key press once, or the KEY_DOWN macro for checking
     * if a button is still down.
     *
     * To count time, suppose that the GameBoy runs at a fixed FPS (60fps) and
     * that VBlank is processed once per frame. Use the vBlankCounter variable
     * and the modulus % operator to do things once every (n) frames. Note that
     * you want to process button every frame regardless (otherwise you will
     * miss inputs.)
     *
     * Do not do any drawing here.
     *
     * TA-TODO: VERY IMPORTANT! READ THIS PART.
     * You need to perform all calculations on the currentAppState passed to you,
     * and perform all state updates on the nextAppState state which we define below
     * and return at the end of the function. YOU SHOULD NOT MODIFY THE CURRENTSTATE.
     * Modifying the currentAppState will mean the undraw function will not be able
     * to undraw it later.
     */

    AppState nextAppState = *currentAppState;

    //player movement
    setPlayerVelocities(nextAppState.ship, keysPressedNow);
    setPlayerPosition(nextAppState.ship);

    //projectile generation/removal
    addFriendlyProjectile(&nextAppState, keysPressedNow);
    //projectile movement. this handles out of bounds as well
    setFriendlyProjectilePositions(nextAppState.friendlyProjectiles, nextAppState.numFriendlyProjectiles);
    //set player shot countdown
    nextAppState.ship->shotCooldown -= 1;
    if (nextAppState.ship->shotCooldown < 0) 
        nextAppState.ship->shotCooldown = 0;

    //every 5 seconds randomly generate an asteroid
    //with a certain velocity


    UNUSED(keysPressedBefore);
    UNUSED(keysPressedNow);

    return nextAppState;
}
