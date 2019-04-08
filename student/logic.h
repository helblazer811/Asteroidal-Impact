#ifndef LOGIC_H
#define LOGIC_H

#include "gba.h"


typedef struct {
    int r;
    int c;
} Point;

typedef struct {
    Point* location;
    Point* velocity;
    Point* size;
    u16* image;
    int lives;
} Asteroid;

typedef struct {
    Point* location;
    Point* velocity;
    Point* size;
    u16* image;
    int lives;
    int shotCooldown;
} PlayerShip; 

typedef struct {
    Point* location;
    Point* velocity;
    Point* size;
    u16* image;
    int lives;
} EnemyShip;

typedef struct {
    Point* location;
    Point* velocity;
    Point* size;
    u16* image;
} FriendlyProjectile;

typedef struct {
    Point* location;
    Point* velocity;
    Point* size;
    u16* image;
} EnemyProjectile;


typedef struct {
    // Store whether or not the game is over in this member:
    int gameOver;

    /*
    * TA-TODO: Add any logical elements you need to keep track of in your app.
    *
    * For example, for a Snake game, those could be:
    *
    * Snake snake;
    * Food foods[10];
    * int points;
    *
    */
    int score;
    int counter;
    PlayerShip* ship;//single object
    int numAsteroids;
    Asteroid** asteroids;//array
    int numEnemyShips;
    EnemyShip** enemyShips;//array
    int numFriendlyProjectiles;
    FriendlyProjectile** friendlyProjectiles;//array
    int numEnemyProjectiles;
    EnemyProjectile** enemyProjectiles;//array

} AppState;

/*
* TA-TODO: Add any additional structs that you need for your app.
*
* For example, for a Snake game, one could be:
*
* typedef struct {
*   int heading;
*   int length;
*   int x;
*   int y;
* } Snake;
*
*/

// This function can initialize an unused AppState struct.
void initializeAppState(AppState *appState);

// This function will be used to process app frames.
AppState processAppState(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow);

Point* PointNew(int r, int c);
PlayerShip* PlayerShipNew(int r, int c, int vr, int vc, int lives);
Asteroid* AsteroidNew(int r, int c, int vr, int vc, u16* image, int width, int height, int lives);
EnemyShip* EnemyShipNew(int r, int c, int vr, int vc,  int lives);
FriendlyProjectile* FriendlyProjectileNew(int r, int c, int vr, int vc);
EnemyProjectile* EnemyProjectileNew(int r, int c, int vr, int vc);

void setPlayerVelocities(PlayerShip* ship, u32 keysPressedNow);
void setPlayerPosition(PlayerShip* ship);

void setFriendlyProjectilePositions(FriendlyProjectile** friendlyProjectiles, int numFriendlyProjectiles);
void addFriendlyProjectile(AppState* currentAppState, u32 keysPressedNow);

void freePoint(Point* point);
void freeFriendlyProjectile(FriendlyProjectile* friendlyProjectile);


#define VELOCITY 1

// If you have anything else you need accessible from outside the logic.c
// file, you can add them here. You likely won't.

#endif
