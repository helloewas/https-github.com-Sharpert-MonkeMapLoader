#pragma once

#include "custom-types/shared/macros.hpp"
#include "Behaviours/GorillaMapTriggerBase.hpp"
#include <vector>

namespace MapLoader {
    enum class TeleporterType {
        Normal,
        Treehouse,
        Map
    };
}

DECLARE_CLASS_CODEGEN(MapLoader, Teleporter, MapLoader::GorillaMapTriggerBase,
    DECLARE_METHOD(void, Awake);
    DECLARE_METHOD(void, Trigger, Il2CppObject* collider);
    DECLARE_METHOD(void, TeleportPlayer);
    DECLARE_INSTANCE_FIELD(bool, tagOnTeleport);
    DECLARE_INSTANCE_FIELD(bool, isTeleporting);
    DECLARE_INSTANCE_FIELD(bool, joinGameOnTeleport);
    
    public:
        std::vector<Il2CppObject*> teleportPoints;
        TeleporterType teleporterType = TeleporterType::Normal;

    REGISTER_FUNCTION(Teleporter,
        REGISTER_METHOD(Awake);
        REGISTER_METHOD(Trigger);
        REGISTER_METHOD(TeleportPlayer);
        REGISTER_FIELD(isTeleporting);
        REGISTER_FIELD(joinGameOnTeleport);
    )
)