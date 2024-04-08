#include "raylib.h"
#include "rlgl.h"
#include "shader.h"

// #define SWITCH_LIGHTS       // uncomment to enable light switching 
#define TILE_LENGTH 4.0f

Vector3 PosFromTiles(float x, float y, float z);
void UpdateLightEnabled(int *enabled);
void UpdateLightPos(Vector3 *pos);

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

    // light
    MyLight light = {0};

    // shaders
    Shader cube1Shader = LoadShader("shaders/mat.vs", "shaders/mat.fs");
    Shader cube2Shader = LoadShader("shaders/mat.vs", "shaders/mat.fs");
    Shader planeShader = LoadShader("shaders/mat.vs", "shaders/mat.fs");
    Shader lightSourceShader = LoadShader("shaders/light.vs", "shaders/light.fs");

    // models
    Model cube1 = LoadModelFromMesh(GenMeshCube(TILE_LENGTH, TILE_LENGTH, TILE_LENGTH));
    cube1.materials[0].shader = cube1Shader;

    Model cube2 = LoadModelFromMesh(GenMeshCube(TILE_LENGTH, TILE_LENGTH, TILE_LENGTH));
    cube2.materials[0].shader = cube2Shader;

    Model plane = LoadModelFromMesh(GenMeshPlane(TILE_LENGTH * 40, TILE_LENGTH * 40, 1, 1));
    plane.materials[0].shader = planeShader;

    Model sphere = LoadModelFromMesh(GenMeshSphere(TILE_LENGTH / 4, 32, 32));
    sphere.materials[0].shader = lightSourceShader;

    // materials
    MyMaterial bronze = CreateBronze();
    MyMaterial greenRubber = CreateGreenRubber();
    MyMaterial pearl = CreatePearl();

    SetTargetFPS(60); 

    while (!WindowShouldClose())   
    {
        UpdateCamera(&camera, CAMERA_ORBITAL);

        UpdateLightPos(&light.position);
        UpdateLightEnabled(&light.enabled);
        UpdateLightColor(&light);

        UpdateMaterialShader(planeShader, greenRubber, light, camera.position);
        UpdateMaterialShader(cube1Shader, bronze, light, camera.position);
        UpdateMaterialShader(cube2Shader, pearl, light, camera.position);

        BeginDrawing();
            ClearBackground(RAYWHITE);
            BeginMode3D(camera);
                
                DrawModel(plane, PosFromTiles(0, 0, 0), 1, WHITE);
                DrawModel(cube1, PosFromTiles(0, 0.5, 0), 1, WHITE);
                DrawModel(cube2, PosFromTiles(2, 0.5, 2), 1, WHITE);

                DrawModel(sphere, light.position, 1, light.enabled == 1 ? WHITE : BLACK);
            EndMode3D();
        EndDrawing();
    }

    UnloadModel(plane);
    UnloadModel(cube1);
    UnloadModel(cube2);
    UnloadModel(sphere);

    UnloadShader(planeShader);
    UnloadShader(cube1Shader);
    UnloadShader(cube2Shader);
    UnloadShader(lightSourceShader);

    CloseWindow();
    return 0;
}

Vector3 PosFromTiles(float x, float y, float z) {
    return (Vector3){x * TILE_LENGTH, y * TILE_LENGTH, z * TILE_LENGTH};
}

void UpdateLightEnabled(int* enabled) {
    if (IsKeyPressed(KEY_SPACE)) {
        (*enabled) = ((*enabled) == 0) ? 1 : 0;
    }
}

void UpdateLightPos(Vector3 *pos) {
    float step = 0.1;

    if (IsKeyDown(KEY_I)) {
        (*pos).x += step;
    } else if (IsKeyDown(KEY_J)) {
        (*pos).z -= step;
    } else if (IsKeyDown(KEY_K)) {
        (*pos).x -= step;
    } else if (IsKeyDown(KEY_L)) {
        (*pos).z += step;
    } else if (IsKeyDown(KEY_U)) {
        (*pos).y += step;
    } else if (IsKeyDown(KEY_O)) {
        (*pos).y -= step;
    }
}