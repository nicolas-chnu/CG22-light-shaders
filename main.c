#include "raylib.h"

int main(void)
{
    const int screenWidth = 900;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Shaders");

    Camera camera = { 
        .position = (Vector3){ 16.0f, 16.0f, 16.0f },
        .target = (Vector3){ 0.0f, 0.0f, 0.0f },
        .up = (Vector3){ 0.0f, 1.0f, 0.0f },
        .fovy = 45.0f,
        .projection = CAMERA_PERSPECTIVE
    };
    
    SetTargetFPS(60); 

    while (!WindowShouldClose())   
    {
        UpdateCamera(&camera, CAMERA_ORBITAL);

        BeginDrawing();
            ClearBackground(RAYWHITE);

            BeginMode3D(camera);
                
            EndMode3D();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}