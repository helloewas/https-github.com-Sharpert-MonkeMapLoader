#pragma once

#include "custom-types/shared/macros.hpp"
#include "typedefs.h"

namespace MapLoader
{
    class RoundEndActions;
}

DECLARE_CLASS(MapLoader, RoundEndActions, "UnityEngine", "MonoBehaviour", sizeof(Il2CppObject) + sizeof(void*),
    DECLARE_INSTANCE_FIELD(bool, respawnOnRoundEnd);
    DECLARE_METHOD(void, Awake);
    DECLARE_METHOD(static void, RoundEnd);
    DECLARE_INSTANCE_FIELD(List<Il2CppObject*>*, objectsToEnable);
    DECLARE_INSTANCE_FIELD(List<Il2CppObject*>*, objectsToDisable);

    REGISTER_FUNCTION(RoundEndActions,
        REGISTER_METHOD(Awake);
        REGISTER_METHOD(RoundEnd);
        REGISTER_FIELD(respawnOnRoundEnd);
        REGISTER_FIELD(objectsToEnable);
        REGISTER_FIELD(objectsToDisable);
    )
)