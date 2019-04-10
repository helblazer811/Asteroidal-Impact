
Point* PointNew(int r, int c);
PlayerShip* PlayerShipNew(int r, int c, int vr, int vc, int lives);
Asteroid* AsteroidNew(int r, int c, int vr, int vc, u16* image, int width, int height, int lives);
EnemyShip* EnemyShipNew(int r, int c, int vr, int vc,  int lives);
FriendlyProjectile* FriendlyProjectileNew(int r, int c, int vr, int vc);
EnemyProjectile* EnemyProjectileNew(int r, int c, int vr, int vc);

void setPlayerVelocities(PlayerShip* ship, u32 keysPressedNow);
void setPlayerPosition(PlayerShip* ship);

void setFriendlyProjectilePositions(FriendlyProjectile** friendlyProjectiles);
void addFriendlyProjectile(AppState* currentAppState, u32 keysPressedNow);

void freePoint(Point* point);
void freeFriendlyProjectile(FriendlyProjectile* friendlyProjectile);
void freeEnemyProjectile(EnemyProjectile* enemyProjectile);
void removeOutOfBoundsEnemyProjectiles(EnemyProjectile** enemyProjectiles);

void removeFriendlyProjectile(AppState* appState);
void addRandomAsteroid(AppState* appState);

void setEnemyVelocities(AppState* appState);
void setEnemyPosition(EnemyShip* ship);
void addEnemyProjectile(AppState* state);
void handleFriendlyCollision(AppState* state);
void handleEnemyCollision(AppState* state);
void handleCollision(AppState* state);
int overlaps(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2);
EnemyShip* createEnemyShip(void);


#define ASTEROID_NUM (9)


#define VELOCITY 1