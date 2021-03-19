#pragma once

#include "custom-types/shared/macros.hpp"
#include "typedefs.h"

namespace MapLoader
{
    class Player;
}

DECLARE_CLASS(MapLoader, Player, "UnityEngine", "MonoBehaviour", sizeof(Il2CppObject) + sizeof(void*),
    DECLARE_INSTANCE_FIELD(Il2CppObject*, destination);
    DECLARE_INSTANCE_FIELD(bool, isTeleporting);

    DECLARE_METHOD(void, Start);
    DECLARE_METHOD(static void, TeleportPlayer, Il2CppObject* destination);
    
    public:
        static inline Player* instance = nullptr;
    REGISTER_FUNCTION(Player,
        REGISTER_METHOD(Start);
        REGISTER_METHOD(TeleportPlayer);
        REGISTER_FIELD(isTeleporting);
        REGISTER_FIELD(destination);
    )
)