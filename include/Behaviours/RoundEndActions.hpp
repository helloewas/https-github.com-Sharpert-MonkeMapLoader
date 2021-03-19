#pragma once

#include "custom-types/shared/macros.hpp"
#include "typedefs.h"

namespace MapLoader
{
    class RoundEndActions;
}

DECLARE_CLASS(MapLoader, RoundEndActions, "UnityEngine", "MonoBehaviour", sizeof(Il2CppObject) + sizeof(void*),
    DECLARE_INSTANCE_FIELD(bool, respawnOnRoundEnd);
    DECLARE_METHOD(void, Start);
    DECLARE_METHOD(static void, RoundEnd);

    public:
        std::vector<Il2CppObject*> objectsToEnable;
        std::vector<Il2CppObject*> objectsToDisable;
        static inline RoundEndActions* instance = nullptr;
    REGISTER_FUNCTION(RoundEndActions,
        REGISTER_METHOD(Start);
        REGISTER_METHOD(RoundEnd);
        REGISTER_FIELD(respawnOnRoundEnd);
    )
)