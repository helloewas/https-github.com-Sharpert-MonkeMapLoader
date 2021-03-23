#pragma once
#include "beatsaber-hook/shared/utils/il2cpp-type-check.hpp"
#include "beatsaber-hook/shared/utils/typedefs.h"

#ifndef SIMPLE_UNITY_TYPEDEFS
#define SIMPLE_UNITY_TYPEDEFS

// UnityEngine.Color
typedef struct Color {
    float r;
    float g;
    float b;
    float a;
} Color;

// UnityEngine.Vector2
typedef struct Vector2 {
    float x;
    float y;
} Vector2;

// UnityEngine.Vector3
typedef struct Vector3 {
    float x;
    float y;
    float z;
} Vector3;

// UnityEngine.Vector4
typedef struct Vector4 {
    float x;
    float y;
    float z;
    float w;
} Vector4;

// UnityEngine.Quaternion
typedef struct Quaternion {
    float x;
    float y;
    float z;
    float w;
} Quaternion;

// UnityEngine.Rect
typedef struct Rect {
    float m_XMin;
    float m_YMin;
    float m_Width;
    float m_Height;
} Rect;

// UnityEngine.Scene
typedef struct Scene {
    int m_Handle;
} Scene;

DEFINE_IL2CPP_ARG_TYPE(Color, "UnityEngine", "Color");
DEFINE_IL2CPP_ARG_TYPE(Vector2, "UnityEngine", "Vector2");
DEFINE_IL2CPP_ARG_TYPE(Vector3, "UnityEngine", "Vector3");
DEFINE_IL2CPP_ARG_TYPE(Vector4, "UnityEngine", "Vector4");
DEFINE_IL2CPP_ARG_TYPE(Quaternion, "UnityEngine", "Quaternion");
DEFINE_IL2CPP_ARG_TYPE(Rect, "UnityEngine", "Rect");
DEFINE_IL2CPP_ARG_TYPE(Scene, "UnityEngine.SceneManagement", "Scene");
DEFINE_IL2CPP_ARG_TYPE_GENERIC_CLASS(List, "System.Collections.Generic", "List`1");
#endif

enum class LoadSceneMode {
    Single,
    Additive
};

enum class LocalPhysicsMode {
    None,
    Physics2D,
    Physics3D
};

typedef struct LoadSceneParameters {
    LoadSceneMode loadSceneMode;
    LocalPhysicsMode localPhysicsMode;
} LoadSCeneParameters;

typedef struct Bounds {
    Vector3 center;
    Vector3 extents;
} Bounds;   

typedef struct RaycastHit {
    Vector3 point;
    Vector3 normal;
    unsigned int faceID;
    float distance;
    Vector2 UV;
    int colliderID;
} RaycastHit;

typedef struct DeliveryMode {
    int value;
    static constexpr const int Unreliable = 0;
    static constexpr const int Reliable = 1;
    static constexpr const int UnreliableUnsequenced = 2;
    static constexpr const int ReliableUnsequenced = 3;
} DeliveryMode;

typedef struct SendOptions {
    int DeliveryMode;
    bool Encrypt;
    uint8_t Channel;
} SendOptions;

DEFINE_IL2CPP_ARG_TYPE(LoadSceneParameters, "UnityEngine.SceneManagement", "LoadSceneParameters");
DEFINE_IL2CPP_ARG_TYPE(Bounds, "UnityEngine", "Bounds");
DEFINE_IL2CPP_ARG_TYPE(RaycastHit, "UnityEngine", "RaycastHit");
DEFINE_IL2CPP_ARG_TYPE(DeliveryMode, "ExitGames.Client.Photon", "DeliveryMode");
DEFINE_IL2CPP_ARG_TYPE(SendOptions, "ExitGames.Client.Photon", "SendOptions");
