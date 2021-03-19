#pragma once

#include "custom-types/shared/macros.hpp"
#include "Behaviours/GorillaMapTriggerBase.hpp"

DECLARE_CLASS(MapLoader, SurfaceClimbSettings, "UnityEngine", "MonoBehaviour", sizeof(Il2CppObject) + sizeof(void*),
    DECLARE_METHOD(void, Start);
    DECLARE_INSTANCE_FIELD(bool, unClimbable);
    DECLARE_INSTANCE_FIELD(float, slipPercentage);

    REGISTER_FUNCTION(SurfaceClimbSettings,
        REGISTER_METHOD(Start);
        REGISTER_FIELD(unClimbable);
        REGISTER_FIELD(slipPercentage);
    )
)