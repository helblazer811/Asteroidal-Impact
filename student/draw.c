#include "draw.h"
#include "gba.h"
#include "logic.h"

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

#include <stdio.h>

// TA-TODO: Include any header files for images generated by nin10kit.
// Example for the provided garbage image:
// #include "images/garbage.h"

// TA-TODO: Add any draw/undraw functions for sub-elements of your app here.
// For example, for a snake game, you could have a drawSnake function
// or a drawFood function
//
// e.g.:
// static void drawSnake(Snake* snake);
// static void drawFood(Food* food);
static void drawLives(PlayerShip* ship);
static void drawPlayerShip(PlayerShip* ship);
static void drawFriendlyProjectile(FriendlyProjectile* projectile);
static void drawEnemyProjectile(EnemyProjectile* projectile);
static void drawAsteroid(Asteroid* asteroid);
static void drawRandomAsteroid(int r, int c);
static void drawEnemyShip(EnemyShip* ship);
// This function will be used to draw everything about the app
// including the background and whatnot.
void fullDrawAppState(AppState *state) {
    // TA-TODO: IMPLEMENT.
    //draw player ship
    UNUSED(drawEnemyProjectile);
    UNUSED(drawAsteroid);
    UNUSED(drawRandomAsteroid);
    UNUSED(drawEnemyShip);

    drawPlayerShip(state->ship);

    for (int i = 0; i < 5; i++) {
    	if (state->friendlyProjectiles[i] != NULL)
    		drawFriendlyProjectile(state->friendlyProjectiles[i]);
    }

    drawLives(state->ship);

    
    char buffer[50]; 

    // Counting the character and storing  
    // in buffer using snprintf 
    int j = snprintf(buffer, 40, "%d", state->friendlyProjectilesIndex); 
    UNUSED(j);
    drawString(100,100,buffer,WHITE);
	

    UNUSED(state);
}

// This function will be used to undraw (i.e. erase) things that might
// move in a frame. E.g. in a Snake game, erase the Snake, the food & the score.
void undrawAppState(AppState *state) {
    // TA-TODO: IMPLEMENT.
    //draw black box for now
    fillScreenDMA(BLACK);
    UNUSED(state);
}

// This function will be used to draw things that might have moved in a frame.
// For example, in a Snake game, draw the snake, the food, the score.
void drawAppState(AppState *state) {
    // TA-TODO: IMPLEMENT.
    UNUSED(state);
}


static void drawPlayerShip(PlayerShip* ship) {
	drawImageDMA(ship->location->c,
				ship->location->r,
				GALAGA_SHIP_SPRITE_WIDTH,
				GALAGA_SHIP_SPRITE_HEIGHT,
				galaga_ship_sprite);
}

static void drawFriendlyProjectile(FriendlyProjectile* projectile) {
	drawImageDMA(projectile->location->c,
				 projectile->location->r,
				 FRIENDLY_LASER_WIDTH,
				 FRIENDLY_LASER_HEIGHT,
				 friendly_laser);
}

static void drawEnemyProjectile(EnemyProjectile* projectile) {
	drawImageDMA(projectile->location->c,
				 projectile->location->r,
				 ENEMY_LASER_WIDTH,
				 ENEMY_LASER_HEIGHT,
				 enemy_laser);
}

static void drawAsteroid(Asteroid* asteroid) {
	drawImageDMA(asteroid->location->c,
				 asteroid->location->r,
				 asteroid->size->c,
				 asteroid->size->r,
				 asteroid->image);
}

const u16* asteroidImages []= {asteroid_0,asteroid_1,asteroid_2,asteroid_3,asteroid_4,funk_1,funk_2,funk_3,funk_4};
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

static void drawRandomAsteroid(int r, int c) {
	int num = 9;

	int random_integer = randint(0,num);

	drawImageDMA(c,
				 r,
				 asteroidSizesWidth[random_integer],
				 asteroidSizesHeight[random_integer],
				 asteroidImages[random_integer]);
}

static void drawEnemyShip(EnemyShip* ship) {

	drawImageDMA(ship->location->c,
				ship->location->r,
				GALAGA_ENEMY_WIDTH,
				GALAGA_ENEMY_HEIGHT,
				galaga_enemy);
}

static void drawLives(PlayerShip* ship) {
	int rLoc = 5;
	int cLoc = WIDTH - HEART_SPRITE_WIDTH - 5;
	for (int i = 0; i < ship->lives; i++) {
		drawImageDMA(cLoc, rLoc, HEART_SPRITE_WIDTH, HEART_SPRITE_HEIGHT, heart_sprite);
		cLoc -= (HEART_SPRITE_WIDTH + 5);
	}
}