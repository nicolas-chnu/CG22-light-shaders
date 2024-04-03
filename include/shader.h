#ifndef SHADER_H
#define SHADER_H

#ifndef RAYLIB_H
#include "raylib.h"
#endif

typedef struct MyMaterial {
    Vector3 ambient;
    Vector3 diffuse;
    Vector3 specular;
    Vector3 shininess;
} MyMaterial;

typedef struct MyLight {
    bool enabled;
    Vector3 position;

    Vector3 ambient;
    Vector3 diffuse;
    Vector3 specular;
} MyLight;

MyMaterial CreateBronze() {
    return (MyMaterial){
        .ambient = (Vector3){0.2125, 0.1275, 0.054},
        .diffuse = (Vector3){0.714, 0.4284, 0.18144},
        .specular = (Vector3){0.393548, 0.271906, 0.166721},
        .shininess = 0.2
    };
}

void UpdateMaterialShader(Shader sh, MyMaterial mat, MyLight li, Vector3 viewPos) {
    int viewPosLoc = GetShaderLocation(sh, "viewPos");
    SetShaderValue(sh, viewPosLoc, (float[3]){viewPos.x, viewPos.y, viewPos.z}, SHADER_UNIFORM_VEC3);

    // material props
    int matAmbLoc = GetShaderLocation(sh, "material.ambient");
    SetShaderValue(sh, matAmbLoc, (float[3]){mat.ambient.x, mat.ambient.y, mat.ambient.z}, SHADER_UNIFORM_VEC3);

    int matDiffLoc = GetShaderLocation(sh, "material.diffuse");
    SetShaderValue(sh, matDiffLoc, (float[3]){mat.diffuse.x, mat.diffuse.y, mat.diffuse.z}, SHADER_UNIFORM_VEC3);

    int matSpecLoc = GetShaderLocation(sh, "material.specular");
    SetShaderValue(sh, matSpecLoc, (float[3]){mat.specular.x, mat.specular.y, mat.specular.z}, SHADER_UNIFORM_VEC3);

    int matShinLoc = GetShaderLocation(sh, "material.shininess");
    SetShaderValue(sh, matShinLoc, &mat.shininess, SHADER_UNIFORM_FLOAT);

    // light props
    int liEnabLoc = GetShaderLocation(sh, "light.enabled");
    SetShaderValue(sh, liEnabLoc, &li.enabled, SHADER_UNIFORM_INT);

    int liPosLoc = GetShaderLocation(sh, "light.position");
    SetShaderValue(sh, liPosLoc, (float[3]){li.position.x, li.position.y, li.position.z}, SHADER_UNIFORM_VEC3);

    int liAmbLoc = GetShaderLocation(sh, "light.ambient");
    SetShaderValue(sh, liAmbLoc, (float[3]){li.ambient.x, li.ambient.y, li.ambient.z}, SHADER_UNIFORM_VEC3);

    int liDiffLoc = GetShaderLocation(sh, "light.diffuse");
    SetShaderValue(sh, liDiffLoc, (float[3]){li.diffuse.x, li.diffuse.y, li.diffuse.z}, SHADER_UNIFORM_VEC3);

    int liSpecLoc = GetShaderLocation(sh, "light.specular");
    SetShaderValue(sh, liSpecLoc, (float[3]){li.specular.x, li.specular.y, li.specular.z}, SHADER_UNIFORM_VEC3);
}

#endif