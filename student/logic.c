#include "logic.h"
#include "gba.h"
#include "images/galaga_ship_sprite.h"
#include "images/friendly_laser.h"
#include <stdlib.h>

#include "images/galaga_ship_sprite.h"
#include "images/galaga_enemy.h"

#include "images/friendly_laser.h"
#include "images/enemy_laser.h"
//asteroids
#include "images/asteroid_0.h"
#include "images/asteroid_1.h"
#include "images/asteroid_2.h"
#include "images/asteroid_3.h"
#include "images/asteroid_4.h"
#include "images/funk_1.h"
#include "images/funk_2.h"
#include "images/funk_3.h"
#include "images/funk_4.h"

#include "images/heart_sprite.h"

void initializeAppState(AppState* appState) {
    // TA-TODO: Initialize everything that's part of this AppState struct here.
    // Suppose the struct contains random values, make sure everything gets
    // the value it should have when the app begins.
    appState->gameOver = 0;
    appState->score = 0;
    appState->ship = PlayerShipNew(HEIGHT - GALAGA_SHIP_SPRITE_HEIGHT*2, WIDTH/2 - GALAGA_SHIP_SPRITE_WIDTH/2, 0, 0, 3);
    appState->asteroidIndex = 0;
    appState->asteroids = malloc(10 * sizeof(Asteroid*));
    appState->enemyShip = EnemyShipNew(20, 30, 0, 1, 3);
    appState->friendlyProjectiles = malloc(5 * sizeof(FriendlyProjectile*));
    appState->friendlyProjectilesIndex = 0;
    appState->enemyProjectilesIndex = 0;
    appState->enemyProjectiles = malloc(5 * sizeof(EnemyProjectile*));
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
    freePoint(friendlyProjectile->location);
    freePoint(friendlyProjectile->velocity);
    free(friendlyProjectile);
}

void freeAsteroid(Asteroid* asteroid) {
    freePoint(asteroid->location);
    freePoint(asteroid->velocity);
    freePoint(asteroid->size);
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
        int i = currentAppState->friendlyProjectilesIndex;
        currentAppState->ship->shotCooldown = 30;
        if (currentAppState->friendlyProjectiles[i] != NULL) {
            freeFriendlyProjectile(currentAppState->friendlyProjectiles[i]);
        }
        currentAppState->friendlyProjectiles[i] = 
            FriendlyProjectileNew(currentAppState->ship->location->r - FRIENDLY_LASER_HEIGHT,
                                  currentAppState->ship->location->c + GALAGA_SHIP_SPRITE_WIDTH / 2 - FRIENDLY_LASER_WIDTH/2,
                                  -2,
                                   0);
        currentAppState->friendlyProjectilesIndex = (i + 1) % 5;
        
    }
}

void setFriendlyProjectilePositions(FriendlyProjectile** friendlyProjectiles) {
    for (int i = 0; i < 5; i++) {
        if (friendlyProjectiles[i] != NULL) {
            friendlyProjectiles[i]->location->r += friendlyProjectiles[i]->velocity->r;
            friendlyProjectiles[i]->location->c += friendlyProjectiles[i]->velocity->c;
        }
    }
}

void setAsteroidPositions(Asteroid ** asteroids) {
    for (int i = 0; i < 10; i++) {
        if (asteroids[i] != NULL) {
            asteroids[i]->location->r += asteroids[i]->velocity->r;
            asteroids[i]->location->c += asteroids[i]->velocity->c;
        }
    }
}

int isOutOfBounds(Point* location) {
    return (location->r < HEIGHT) &&
            (location->r > 0) &&
            (location->c > 0) &&
            (location->c < WIDTH);
}

#define ASTEROID_NUM (9)

u16* asteroidImages []= {
    asteroid_0,
    asteroid_1,
    asteroid_2,
    asteroid_3,
    asteroid_4,
    funk_1,
    funk_2,
    funk_3,
    funk_4
};

const int asteroidSizesWidth []= {
    ASTEROID_0_WIDTH,
    ASTEROID_1_WIDTH,
    ASTEROID_2_WIDTH,
    ASTEROID_3_WIDTH,
    ASTEROID_4_WIDTH,
    FUNK_1_WIDTH,
    FUNK_2_WIDTH,
    FUNK_3_WIDTH,
    FUNK_4_WIDTH
};
const int asteroidSizesHeight[] = {
    ASTEROID_0_HEIGHT,
    ASTEROID_1_HEIGHT,
    ASTEROID_2_HEIGHT,
    ASTEROID_3_HEIGHT,
    ASTEROID_4_HEIGHT,
    FUNK_1_HEIGHT,
    FUNK_2_HEIGHT,
    FUNK_3_HEIGHT,
    FUNK_4_HEIGHT
};

void addRandomAsteroid(AppState* appState) {
    if (appState->counter == 0) {
        //generate random asteroid
        int asteroid_num = randint(0, ASTEROID_NUM);
        //position starts negative 
        int r = randint(-100, WIDTH + 100);
        //genrates a random int in the range of 100 outside of the map on both sides
        while (r < WIDTH && r > -1*asteroidSizesHeight[asteroid_num]) 
            r = randint(-100, WIDTH + 100); 

        int c = randint(-100, HEIGHT + 100);
        while (c < HEIGHT && c > -1*asteroidSizesWidth[asteroid_num])
            c = randint(-100, HEIGHT + 100);

        //generate velocities
        int vr = randint(-2,2); 
        if (vr == 0) 
            vr++;
        int vc = randint(-2,2);
        if (vc == 0)
            vc++;

        int lives = 1;

        //choose random asteroid
        Asteroid* asteroid = AsteroidNew(r,
                                        c,
                                        vr,
                                        vc,
                                        asteroidImages[asteroid_num],
                                        asteroidSizesWidth[asteroid_num],
                                        asteroidSizesHeight[asteroid_num],
                                        lives);

        //add it to the app state
        if (appState->asteroids[appState->asteroidIndex] != NULL) {
            freeAsteroid(appState->asteroids[appState->asteroidIndex]);
        }
        appState->asteroids[appState->asteroidIndex] = asteroid;
        appState->asteroidIndex = (appState->asteroidIndex + 1) % 10;
    }
}


void setEnemyVelocities(AppState* appState) {
    //set enemy to move back and forth
    appState->enemyShip->velocity->r = 0;
    if (appState->enemyShip->location->c > WIDTH - 50)
        appState->enemyShip->velocity->c = -1;
    if (appState->enemyShip->location->c < 50)
        appState->enemyShip->velocity->c = 1;


}

void setEnemyPosition(EnemyShip* ship) {
    ship->location->r += ship->velocity->r;
    ship->location->c += ship->velocity->c;
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
    setFriendlyProjectilePositions(nextAppState.friendlyProjectiles);
    //set player shot countdown
    nextAppState.ship->shotCooldown -= 1;
    if (nextAppState.ship->shotCooldown < 0) 
        nextAppState.ship->shotCooldown = 0;

    //every 5 seconds randomly generate an asteroid
    //with a certain velocity
    addRandomAsteroid(&nextAppState);
    setAsteroidPositions(nextAppState.asteroids);

    setEnemyVelocities(&nextAppState);
    setEnemyPosition(nextAppState.enemyShip);

    UNUSED(keysPressedBefore);
    UNUSED(keysPressedNow);

    return nextAppState;
}
