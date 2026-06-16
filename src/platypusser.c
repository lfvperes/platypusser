#include <stdlib.h>
#include "raylib.h"

typedef struct {
    Vector2 position;
    Vector2 velocity;
    int width;
    int height;
    Color color;
} object;

#define DELAY 0.2
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define PLAYER_STEP 50
#define PLAYER_SIZE 50
#define CAR_SIZE 50
#define CAR_SPEED 3
#define MAX_COLORS_COUNT 21
#define LANE_1 300
#define LANE_2 400
#define LANE_3 500
#define STREET_UPPER_BOUNDARY 300
#define STREET_LOWER_BOUNDARY (WINDOW_HEIGHT - PLAYER_SIZE)

Color colors[MAX_COLORS_COUNT] = {
    DARKGRAY, MAROON, ORANGE, DARKGREEN, DARKBLUE, DARKPURPLE, DARKBROWN,
    GRAY, RED, GOLD, LIME, BLUE, VIOLET, BROWN, LIGHTGRAY, PINK, YELLOW,
    GREEN, SKYBLUE, PURPLE, BEIGE };

int isGameOver = 0;
char* screenText = "PRESS ANY ARROW KEY TO START";
float hopTimer = 0.0f;

int collision(object a, object b) {
    return (a.position.x < b.position.x + b.width) && (a.position.x + a.width > b.position.x) && (a.position.y < b.position.y + b.height) && (a.position.y + a.height > b.position.y);
}

int main() {
    // initialization
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Platypus Game");
    SetTargetFPS(60);
    int score = 0;

    object* cars = NULL;
    object player = {{WINDOW_WIDTH / 2, WINDOW_HEIGHT - PLAYER_SIZE}, {0, 0}, PLAYER_SIZE, PLAYER_SIZE, GREEN};
    
    // game loop
    while (!WindowShouldClose()) {
        // updating game variables
        hopTimer -= GetFrameTime();
        
        if (!isGameOver) {
            screenText = "";
            // only allows movement when hop timer is zero
            if (hopTimer <= 0.0f) {
                // only moves orthogonally, resets the other velocity component to zero
                if (IsKeyDown(KEY_RIGHT)) {
                    player.velocity.x = PLAYER_STEP;
                    player.velocity.y = 0;
                } else if (IsKeyDown(KEY_LEFT)) {
                    player.velocity.x = -PLAYER_STEP;
                    player.velocity.y = 0;
                } else {
                    player.velocity.x = 0;
                }
                if (IsKeyDown(KEY_DOWN)) {
                    player.velocity.y = PLAYER_STEP;
                    player.velocity.x = 0;
                } else if (IsKeyDown(KEY_UP)) {
                    player.velocity.y = -PLAYER_STEP;
                    player.velocity.x = 0;
                } else {
                    player.velocity.y = 0;
                }

                player.position.x += player.velocity.x;
                player.position.y += player.velocity.y;
                hopTimer = DELAY;  // resets timer after each hop
            }

            // check wall collision
            if (player.position.x < 0) player.position.x = 0;
            if (player.position.y < 0) player.position.y = 0;
            if (player.position.x > WINDOW_WIDTH - player.width) player.position.x = WINDOW_WIDTH - player.width;
            if (player.position.y > WINDOW_HEIGHT - player.height) player.position.y = WINDOW_HEIGHT - player.height;

            // check for collisions with cars
            for (int i = 0; i < score; i++) {
                if (collision(player, cars[i])) {
                    isGameOver = 1;
                    screenText = "GAME OVER";
                    break;
                }
            }

            // Add new cars and update their positions
            if (score < 10 && GetRandomValue(0, 100) < 5) { // 5% chance to add a new car
                cars = realloc(cars, (score + 1) * sizeof(object));
                int lane = GetRandomValue(1, 3);
                switch (lane) {
                    case 1:
                        cars[score].position = (Vector2){0, LANE_1};
                        break;
                    case 2:
                        cars[score].position = (Vector2){0, LANE_2};
                        break;
                    case 3:
                        cars[score].position = (Vector2){0, LANE_3};
                        break;
                }
                cars[score].velocity = (Vector2){CAR_SPEED, 0};
                cars[score].width = CAR_SIZE;
                cars[score].height = CAR_SIZE;
                cars[score].color = colors[GetRandomValue(0, MAX_COLORS_COUNT - 1)];
                score++;
            }
        }

        // draw everything
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText(screenText, 10, 10, 20, DARKGRAY);
        // draw player
        DrawRectangleV(player.position, (Vector2){player.width, player.height}, player.color);
        // draw street
        DrawRectangle(0, STREET_UPPER_BOUNDARY, WINDOW_WIDTH, STREET_LOWER_BOUNDARY - STREET_UPPER_BOUNDARY, DARKGRAY);
        for (int i = 0; i < score; i++) {
            DrawRectangleV(cars[i].position, (Vector2){cars[i].width, cars[i].height}, cars[i].color);
            cars[i].position.x += cars[i].velocity.x; // move car
            if (cars[i].position.x > WINDOW_WIDTH) { // reset car position if it goes off screen
                cars[i].position.x = 0;
            }
        }
        // Draw cars here
        EndDrawing();
    }

    // finalizacao
    // free(cars);
    CloseWindow();
    return 0;
}