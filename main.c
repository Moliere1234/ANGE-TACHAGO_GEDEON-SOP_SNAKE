#include "raylib.h"
#include <stdlib.h>
#include <time.h>

#define CELL_SIZE 20
#define GRID_WIDTH 50
#define GRID_HEIGHT 35
#define MAX_SNAKE_LENGTH 100
#define FRUIT_COUNT 4

typedef enum { UP, DOWN, LEFT, RIGHT } Direction;

typedef struct {
    int x, y;
} Position;

typedef struct {
    Position body[MAX_SNAKE_LENGTH];
    int length;
    Direction dir;
} Snake;

typedef struct {
    Position position;
    int type;
} Fruit;

Snake snake;
Fruit fruit;
Texture2D fruitTextures[FRUIT_COUNT];
int score = 0;
bool gameOver = false;

void InitGame();
void UpdateGame();
void DrawGame();
void GenerateFruit();
bool CheckCollision(Position a, Position b);
bool CheckSelfCollision();
bool CheckWallCollision();

int main(void) {
    InitWindow(GRID_WIDTH * CELL_SIZE, GRID_HEIGHT * CELL_SIZE, "Snake avec vrais fruits 🍎🍌🍒");
    SetTargetFPS(10);
    InitGame();

    // Charger les images de fruits
    fruitTextures[0] = LoadTexture("apple.png");
    fruitTextures[1] = LoadTexture("banana.png");
    fruitTextures[2] = LoadTexture("cherry.png");
    fruitTextures[3] = LoadTexture("strawberry.png");

    while (!WindowShouldClose()) {
        if (!gameOver) {
            UpdateGame();
        }
        BeginDrawing();
        ClearBackground(BLACK);
        DrawGame();
        EndDrawing();
    }

    // Décharger les textures
    for (int i = 0; i < FRUIT_COUNT; i++) {
        UnloadTexture(fruitTextures[i]);
    }

    CloseWindow();
    return 0;
}

void InitGame() {
    snake.length = 3;
    snake.dir = RIGHT;
    for (int i = 0; i < snake.length; i++) {
        snake.body[i].x = 5 - i;
        snake.body[i].y = 5;
    }
    GenerateFruit();
    score = 0;
    gameOver = false;
}

void UpdateGame() {
    if (IsKeyPressed(KEY_UP) && snake.dir != DOWN) snake.dir = UP;
    if (IsKeyPressed(KEY_DOWN) && snake.dir != UP) snake.dir = DOWN;
    if (IsKeyPressed(KEY_LEFT) && snake.dir != RIGHT) snake.dir = LEFT;
    if (IsKeyPressed(KEY_RIGHT) && snake.dir != LEFT) snake.dir = RIGHT;

    for (int i = snake.length - 1; i > 0; i--) {
        snake.body[i] = snake.body[i - 1];
    }

    switch (snake.dir) {
        case UP:    snake.body[0].y--; break;
        case DOWN:  snake.body[0].y++; break;
        case LEFT:  snake.body[0].x--; break;
        case RIGHT: snake.body[0].x++; break;
    }

    if (CheckCollision(snake.body[0], fruit.position)) {
        if (snake.length < MAX_SNAKE_LENGTH) snake.length++;
        score++;
        GenerateFruit();
    }

    if (CheckWallCollision() || CheckSelfCollision()) {
        gameOver = true;
    }
}

void DrawGame() {
    for (int i = 0; i < snake.length; i++) {
        DrawRectangle(snake.body[i].x * CELL_SIZE,
                      snake.body[i].y * CELL_SIZE,
                      CELL_SIZE, CELL_SIZE,
                      (i == 0) ? DARKGREEN : GREEN);
    }

    // Dessiner le fruit (image)
    DrawTextureEx(fruitTextures[fruit.type],
                  (Vector2){fruit.position.x * CELL_SIZE, fruit.position.y * CELL_SIZE},
                  0.0f, (float)CELL_SIZE / fruitTextures[fruit.type].width, WHITE);

    DrawText(TextFormat("Score: %d", score), 10, 10, 20, WHITE);

    if (gameOver) {
        DrawText("💀 Game Over! Appuie sur R pour recommencer", 100, GetScreenHeight() / 2, 20, RED);
        if (IsKeyPressed(KEY_R)) InitGame();
    }
}

void GenerateFruit() {
    fruit.position.x = rand() % GRID_WIDTH;
    fruit.position.y = rand() % GRID_HEIGHT;
    fruit.type = rand() % FRUIT_COUNT;
}

bool CheckCollision(Position a, Position b) {
    return a.x == b.x && a.y == b.y;
}

bool CheckSelfCollision() {
    for (int i = 1; i < snake.length; i++) {
        if (CheckCollision(snake.body[0], snake.body[i])) return true;
    }
    return false;
}

bool CheckWallCollision() {
    return (snake.body[0].x < 0 || snake.body[0].x >= GRID_WIDTH ||
            snake.body[0].y < 0 || snake.body[0].y >= GRID_HEIGHT);
}

