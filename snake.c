#include "raylib.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define TAILLE_CASE 20
#define NB_CASES_X 40
#define NB_CASES_Y 30
#define LONGUEUR_MAX 1000

// Fonction d’accueil
int AfficherAccueil()
{
    float angleSoleil = 0.0f;
    int niveauSelectionne = 0;

    while (!WindowShouldClose() && niveauSelectionne == 0) {
        BeginDrawing();
        ClearBackground(SKYBLUE);

        Vector2 centreSoleil = { 700.0f, 100.0f };
        DrawCircleV(centreSoleil, 40.0f, YELLOW);
        Vector2 pointRayon = {
            700.0f + 60.0f * cosf(angleSoleil),
            100.0f + 60.0f * sinf(angleSoleil)
        };
        DrawLineEx(centreSoleil, pointRayon, 4.0f, ORANGE);

        DrawRectangle(0, 400, 800, 200, BLUE);
        DrawRectangle(100 + 20 * sinf(angleSoleil), 450, 30, 60, DARKGREEN);
        DrawRectangle(300 + 20 * cosf(angleSoleil), 440, 30, 60, DARKPURPLE);

        DrawText("Bienvenue sur Snake Plage Edition", 140, 50, 30, DARKBLUE);
        DrawText("Choisis ton niveau :", 260, 150, 25, BLACK);
        DrawText("[1] Débutant  -  [2] Moyen  -  [3] Pro", 200, 200, 25, RED);

        EndDrawing();

        angleSoleil += 0.05f;

        if (IsKeyPressed(KEY_ONE)) niveauSelectionne = 1;
        if (IsKeyPressed(KEY_TWO)) niveauSelectionne = 2;
        if (IsKeyPressed(KEY_THREE)) niveauSelectionne = 3;
    }

    return niveauSelectionne;
}

// Fonction principale du jeu
void JouerSnake(int niveau)
{
    Vector2 serpent[LONGUEUR_MAX];
    int longueur = 3; // taille initiale du serpent

    Vector2 direction = { TAILLE_CASE, 0 }; // vers la droite
    Vector2 pomme = { 0, 0 };
    int score = 0;

    SetTargetFPS(5 + niveau * 5);
    srand(time(NULL));

    // Initialisation du serpent
    for (int i = 0; i < longueur; i++) {
        serpent[i].x = 400 - i * TAILLE_CASE;
        serpent[i].y = 300;
    }

    // Position initiale de la pomme
    pomme.x = (rand() % NB_CASES_X) * TAILLE_CASE;
    pomme.y = (rand() % NB_CASES_Y) * TAILLE_CASE;

    while (!WindowShouldClose()) {
        // Contrôle
        if (IsKeyPressed(KEY_RIGHT) && direction.x == 0) direction = (Vector2){ TAILLE_CASE, 0 };
        if (IsKeyPressed(KEY_LEFT)  && direction.x == 0) direction = (Vector2){ -TAILLE_CASE, 0 };
        if (IsKeyPressed(KEY_UP)    && direction.y == 0) direction = (Vector2){ 0, -TAILLE_CASE };
        if (IsKeyPressed(KEY_DOWN)  && direction.y == 0) direction = (Vector2){ 0, TAILLE_CASE };

        // Décalage du corps
        for (int i = longueur - 1; i > 0; i--) {
            serpent[i] = serpent[i - 1];
        }

        // Avancer la tête
        serpent[0].x += direction.x;
        serpent[0].y += direction.y;

        // Collision mur
        if (serpent[0].x < 0 || serpent[0].x >= NB_CASES_X * TAILLE_CASE ||
            serpent[0].y < 0 || serpent[0].y >= NB_CASES_Y * TAILLE_CASE) {
            break;
        }

        // Collision avec soi-même
        for (int i = 1; i < longueur; i++) {
            if (serpent[0].x == serpent[i].x && serpent[0].y == serpent[i].y) {
                break;
            }
        }

        // Collision avec pomme
        if ((int)serpent[0].x == (int)pomme.x && (int)serpent[0].y == (int)pomme.y) {
            longueur++;
            score++;

            // Nouvelle position aléatoire de la pomme
            pomme.x = (rand() % NB_CASES_X) * TAILLE_CASE;
            pomme.y = (rand() % NB_CASES_Y) * TAILLE_CASE;
        }

        // Dessin
        BeginDrawing();
        ClearBackground(DARKGRAY);

        // Pomme
        DrawRectangleV(pomme, (Vector2){ TAILLE_CASE, TAILLE_CASE }, RED);

        // Serpent
        for (int i = 0; i < longueur; i++) {
            DrawRectangleV(serpent[i], (Vector2){ TAILLE_CASE, TAILLE_CASE }, GREEN);
        }

        DrawText(TextFormat("Score: %d", score), 10, 10, 20, WHITE);

        EndDrawing();
    }
}

int main(void)
{
    InitWindow(NB_CASES_X * TAILLE_CASE, NB_CASES_Y * TAILLE_CASE, "Snake - Plage Edition");

    int niveau = AfficherAccueil(); // écran d’accueil
    JouerSnake(niveau);             // lance le jeu

    CloseWindow();
    return 0;
}
