#include "raylib.h"
#include "rlgl.h"
#include "glfw3.h"
#include "stencil.h"

// #define SWITCH_LIGHTS       // uncomment to enable light switching
#include "shader.h"

#define TILE_LENGTH 4.0f

typedef struct ModelDistance
{
    Model model;
    Vector3 position;
} ModelDistance;

Vector3 PosFromTiles(float x, float y, float z);
void UpdateLightEnabled(int *enabled);
void UpdateLightPos(Vector3 *pos);
void UpdateFogDensity(float *density);
void UpdateBlendMode(int *blendMode);
int CompareModels(const void *a, const void *b);

Vector3 cameraPosition;

int main(void)
{
    const int screenWidth = 1400;
    const int screenHeight = 900;

    InitWindow(screenWidth, screenHeight, "Shaders");

    Camera camera = {
        .position = (Vector3){16.0f, 5.0f, 16.0f},
        .target = (Vector3){0.0f, 0.0f, 0.0f},
        .up = (Vector3){0.0f, 1.0f, 0.0f},
        .fovy = 45.0f,
        .projection = CAMERA_PERSPECTIVE};

    // fog
    float fogDensity = 0.04f;
    int blendMode = BLEND_ADDITIVE;

    // light
    MyLight light = {0};

    // shaders
    Shader cube1Shader = LoadShader("src/shaders/shared.vs", "src/shaders/transparent.fs");
    Shader cube2Shader = LoadShader("src/shaders/shared.vs", "src/shaders/transparent.fs");
    Shader planeShader = LoadShader("src/shaders/shared.vs", "src/shaders/plain.fs");

    // models
    Model cube1 = LoadModelFromMesh(GenMeshCube(TILE_LENGTH, TILE_LENGTH, TILE_LENGTH));
    cube1.materials[0].shader = cube1Shader;

    Model cube2 = LoadModelFromMesh(GenMeshCube(TILE_LENGTH, TILE_LENGTH, TILE_LENGTH));
    cube2.materials[0].shader = cube2Shader;

    Model plane = LoadModelFromMesh(GenMeshPlane(TILE_LENGTH * 10, TILE_LENGTH * 10, 1, 1));
    plane.materials[0].shader = planeShader;

    Model sphere = LoadModelFromMesh(GenMeshSphere(TILE_LENGTH / 4, 32, 32));

    // materials
    MyMaterial bronze = CreateBronze();
    MyMaterial blueCristal = CreateBlueCristal();
    MyMaterial pearl = CreatePearl();

    SetTargetFPS(60);
    DisableCursor();

    while (!WindowShouldClose())
    {
        UpdateCamera(&camera, CAMERA_FREE);
        cameraPosition = camera.position;

        UpdateFogDensity(&fogDensity);
        UpdateBlendMode(&blendMode);

        UpdateLightPos(&light.position);
        UpdateLightEnabled(&light.enabled);
        UpdateLightColor(&light);

        UpdateMaterialShader(planeShader, blueCristal, light, camera.position, fogDensity);
        UpdateMaterialShader(cube1Shader, bronze, light, camera.position, fogDensity);
        UpdateMaterialShader(cube2Shader, pearl, light, camera.position, fogDensity);

        ModelDistance models[] = {
            {cube1, PosFromTiles(0, 0.5, 0)},
            {cube2, PosFromTiles(1.3, 0.6, 1.3)}};

        // sorting the models by distance from the camera
        qsort(models, sizeof(models) / sizeof(models[0]), sizeof(ModelDistance), CompareModels);

        BeginDrawing();
            ClearBackground(RAYWHITE);

            BeginMode3D(camera);
                BeginStencil();

                BeginStencilMask();
                    DrawPlane(PosFromTiles(0, 0, 0), (Vector2){TILE_LENGTH * 4, TILE_LENGTH * 4}, (Color) {255, 0, 0, 100});
                EndStencilMask();

                rlPushMatrix();
                    rlScalef(1, -1, 1);
                    DrawCube(PosFromTiles(0, 0.6, 0), TILE_LENGTH, TILE_LENGTH, TILE_LENGTH, BLUE);
                rlPopMatrix();

                EndStencil();

                BeginBlendMode(BLEND_ALPHA);
                    DrawPlane(PosFromTiles(0, 0, 0), (Vector2){TILE_LENGTH * 4, TILE_LENGTH * 4}, (Color) {255, 0, 0, 100});
                EndBlendMode();

                DrawCube(PosFromTiles(0, 0.6, 0), TILE_LENGTH, TILE_LENGTH, TILE_LENGTH, BLUE);
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

    CloseWindow();
    return 0;
}

Vector3 PosFromTiles(float x, float y, float z)
{
    return (Vector3){x * TILE_LENGTH, y * TILE_LENGTH, z * TILE_LENGTH};
}

void UpdateLightEnabled(int *enabled)
{
    if (IsKeyPressed(KEY_F11))
    {
        (*enabled) = ((*enabled) == 0) ? 1 : 0;
    }
}

void UpdateLightPos(Vector3 *pos)
{
    float step = 0.1;

    if (IsKeyDown(KEY_I))
    {
        (*pos).x += step;
    }
    else if (IsKeyDown(KEY_J))
    {
        (*pos).z -= step;
    }
    else if (IsKeyDown(KEY_K))
    {
        (*pos).x -= step;
    }
    else if (IsKeyDown(KEY_L))
    {
        (*pos).z += step;
    }
    else if (IsKeyDown(KEY_O))
    {
        (*pos).y += step;
    }
    else if (IsKeyDown(KEY_U))
    {
        (*pos).y -= step;
    }
}

void UpdateFogDensity(float *density)
{
    if (IsKeyDown(KEY_PAGE_UP))
    {
        *density += 0.001f;

        if (*density > 1.0f)
        {
            *density = 1.0f;
        }
    }

    if (IsKeyDown(KEY_PAGE_DOWN))
    {
        *density -= 0.001f;

        if (*density < 0.0f)
        {
            *density = 0.0f;
        }
    }
}

void UpdateBlendMode(int *blendMode)
{
    if (IsKeyPressed(KEY_F5))
    {
        *blendMode = *blendMode == BLEND_ALPHA ? BLEND_ADDITIVE : BLEND_ALPHA;
    }
}

int CompareModels(const void *a, const void *b)
{
    ModelDistance *modelA = (ModelDistance *)a;
    ModelDistance *modelB = (ModelDistance *)b;

    float distanceA = Vector3Distance(modelA->position, cameraPosition);
    float distanceB = Vector3Distance(modelB->position, cameraPosition);

    return distanceB - distanceA;
}