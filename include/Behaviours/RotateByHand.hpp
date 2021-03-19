#pragma once

#include "custom-types/shared/macros.hpp"
#include "typedefs.h"

DECLARE_CLASS(MapLoader, RotateByHand, "UnityEngine", "MonoBehaviour", sizeof(Il2CppObject) + sizeof(void*),
    DECLARE_METHOD(void, Awake);
    DECLARE_METHOD(void, OnTriggerEnter, Il2CppObject* collider);
    DECLARE_METHOD(void, OnTriggerStay, Il2CppObject* collider);
    DECLARE_METHOD(void, OnTriggerExit, Il2CppObject* collider);
    DECLARE_METHOD(bool, CheckIfValid, Il2CppObject* collider);
    DECLARE_METHOD(Vector3, GetVector, Il2CppObject* collider);

    DECLARE_INSTANCE_FIELD(Vector3, midPoint);
    DECLARE_INSTANCE_FIELD(float, angle);
    DECLARE_INSTANCE_FIELD(Vector3, startVector);
    DECLARE_INSTANCE_FIELD(Il2CppObject*, rigidBody);
    DECLARE_INSTANCE_FIELD(float, deltaAngle);
    DECLARE_INSTANCE_FIELD(float, previousAngle);

    REGISTER_FUNCTION(RotateByHand,
        REGISTER_METHOD(Awake);
        REGISTER_METHOD(OnTriggerEnter);
        REGISTER_METHOD(OnTriggerStay);
        REGISTER_METHOD(OnTriggerExit);
        REGISTER_METHOD(CheckIfValid);
        REGISTER_METHOD(GetVector);

        REGISTER_FIELD(midPoint);
        REGISTER_FIELD(angle);
        REGISTER_FIELD(startVector);
        REGISTER_FIELD(rigidBody);
        REGISTER_FIELD(deltaAngle);
        REGISTER_FIELD(previousAngle);
    )
)