#include "raylib.h"
#include "rlgl.h"

// #define SWITCH_LIGHTS       // uncomment to enable light switching 
#include "shader.h"

#define TILE_LENGTH 4.0f

Vector3 PosFromTiles(float x, float y, float z);
void UpdateLightEnabled(int *enabled);
void UpdateLightPos(Vector3 *pos);
void UpdateFogDensity(float *density);

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

    // fog
    float fogDensity = 0.04f;

    // light
    MyLight light = {0};

    // shaders
    Shader cube1Shader = LoadShader("shaders/mat.vs", "shaders/mat.fs");
    Shader cube2Shader = LoadShader("shaders/mat.vs", "shaders/mat.fs");
    Shader planeShader = LoadShader("shaders/mat.vs", "shaders/mat.fs");

    // models
    Model cube1 = LoadModelFromMesh(GenMeshCube(TILE_LENGTH, TILE_LENGTH, TILE_LENGTH));
    cube1.materials[0].shader = cube1Shader;

    Model cube2 = LoadModelFromMesh(GenMeshCube(TILE_LENGTH, TILE_LENGTH, TILE_LENGTH));
    cube2.materials[0].shader = cube2Shader;

    Model plane = LoadModelFromMesh(GenMeshPlane(TILE_LENGTH * 40, TILE_LENGTH * 40, 1, 1));
    plane.materials[0].shader = planeShader;

    Model sphere = LoadModelFromMesh(GenMeshSphere(TILE_LENGTH / 4, 32, 32));

    // materials
    MyMaterial bronze = CreateBronze();
    MyMaterial greenRubber = CreateGreenRubber();
    MyMaterial pearl = CreatePearl();

    SetTargetFPS(60); 

    while (!WindowShouldClose())   
    {
        UpdateCamera(&camera, CAMERA_ORBITAL);

        UpdateFogDensity(&fogDensity);

        UpdateLightPos(&light.position);
        UpdateLightEnabled(&light.enabled);
        UpdateLightColor(&light);

        UpdateMaterialShader(planeShader, greenRubber, light, camera.position, fogDensity);
        UpdateMaterialShader(cube1Shader, bronze, light, camera.position, fogDensity);
        UpdateMaterialShader(cube2Shader, pearl, light, camera.position, fogDensity);

        BeginDrawing();
            ClearBackground(RAYWHITE);
        
            BeginMode3D(camera);
                DrawModel(plane, PosFromTiles(0, 0, 0), 1, WHITE);

                BeginBlendMode(BLEND_ADDITIVE);    
                    DrawModel(sphere, light.position, 1, light.enabled == 1 ? WHITE : BLACK);
                    
                    DrawModel(cube1, PosFromTiles(0, 0.5, 0), 1, WHITE);
                    DrawModel(cube2, PosFromTiles(2, 0.5, 2), 1, WHITE);

                    
                EndBlendMode();
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

    if (IsKeyDown(KEY_W)) {
        (*pos).x += step;
    } else if (IsKeyDown(KEY_A)) {
        (*pos).z -= step;
    } else if (IsKeyDown(KEY_S)) {
        (*pos).x -= step;
    } else if (IsKeyDown(KEY_D)) {
        (*pos).z += step;
    } else if (IsKeyDown(KEY_E)) {
        (*pos).y += step;
    } else if (IsKeyDown(KEY_Q)) {
        (*pos).y -= step;
    }
}

void UpdateFogDensity(float* density) {
    if (IsKeyDown(KEY_UP))
        {
            *density += 0.001f;
            
            if (*density > 1.0f) {
                *density = 1.0f;
            }
        }

        if (IsKeyDown(KEY_DOWN))
        {
            *density -= 0.001f;

            if (*density < 0.0f) {
                *density = 0.0f;
            }
        }
}