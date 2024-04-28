#ifndef SHADER_H
#define SHADER_H

#ifndef RAYLIB_H
#include "raylib.h"
#endif

#ifndef RAYMATH_H
#include "raymath.h"
#endif

typedef struct MyMaterial {
    Vector3 ambient;
    Vector3 diffuse;
    Vector3 specular;
    Vector3 shininess;
} MyMaterial;

typedef struct MyLight {
    int enabled;
    Vector3 position;

    Vector3 ambient;
    Vector3 diffuse;
    Vector3 specular;
} MyLight;

MyMaterial CreateBronze() {
    return (MyMaterial) {
        .ambient = (Vector3){0.2125, 0.1275, 0.054},
        .diffuse = (Vector3){0.714, 0.4284, 0.18144},
        .specular = (Vector3){0.393548, 0.271906, 0.166721},
        .shininess = 72 
    };
}

MyMaterial CreatePearl() {
    return (MyMaterial) {
        .ambient = (Vector3){0.25, 0.20725, 0.20725},
        .diffuse = (Vector3){1, 0.829, 0.829},
        .specular = (Vector3){0.296648, 0.296648, 0.296648},
        .shininess = 0.088   
    };
}

MyMaterial CreateBlueCristal() {
    return (MyMaterial) {
        .ambient = (Vector3){0.0, 0.05, 0.0},
        .diffuse = (Vector3){0.1, 0.1, 0.9},
        .specular = (Vector3){0.05,	0.05, 0.05},
        .shininess = 0.5
    };
}

void UpdateLightColor(MyLight* li) {
    float tk = sinf((float)GetTime());

    #ifdef SWITCH_LIGHTS
    Vector3 lightColor = {tk * 2.0, tk * 0.7, tk * 1.3};
    #else
    Vector3 lightColor = {2.0, 0.7, 1.3};
    #endif
    
    Vector3 diffuseColor = Vector3Multiply(lightColor, (Vector3){.5, .5, .5});
    Vector3 ambientColor = Vector3Multiply(lightColor, (Vector3){.2, .2, .2});

    (*li).ambient = ambientColor;
    (*li).diffuse = diffuseColor;
    (*li).specular = (Vector3){1, 1, 1};
}

void UpdateMaterialShader(Shader sh, MyMaterial mat, MyLight li, Vector3 viewPos, float fogDensity) {
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

    // fog props
    #ifdef SWITCH_LIGHTS
    float tk = sinf((float)GetTime());
    Vector4 fogColor = {tk * 2.0, tk * 0.7, tk * 1.3, 1};
    #else
    Vector4 fogColor = {0.5, 0.5, 0.5, 1};
    #endif

    int fogColorLoc = GetShaderLocation(sh, "fogColor");
    SetShaderValue(sh, fogColorLoc, (float[4]){fogColor.x, fogColor.y, fogColor.z, fogColor.w}, SHADER_UNIFORM_VEC4);

    int fogDensityLoc = GetShaderLocation(sh, "fogDensity");
    SetShaderValue(sh, fogDensityLoc, &fogDensity, SHADER_UNIFORM_FLOAT);
}

#endif