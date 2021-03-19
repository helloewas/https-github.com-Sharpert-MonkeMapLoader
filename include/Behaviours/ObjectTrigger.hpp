#pragma once

#include "custom-types/shared/macros.hpp"
#include "Behaviours/GorillaMapTriggerBase.hpp"

DECLARE_CLASS_CODEGEN(MapLoader, ObjectTrigger, MapLoader::GorillaMapTriggerBase,
    DECLARE_METHOD(void, Start);
    DECLARE_METHOD(void, Trigger, Il2CppObject* collider);
    DECLARE_INSTANCE_FIELD(Il2CppObject*, objectToTrigger);
    DECLARE_INSTANCE_FIELD(bool, disableObject);
    DECLARE_INSTANCE_FIELD(bool, onlyTriggerOnce);
    DECLARE_INSTANCE_FIELD(bool, triggered);
    REGISTER_FUNCTION(ObjectTrigger,
        REGISTER_METHOD(Start);
        REGISTER_METHOD(Trigger);
        REGISTER_FIELD(objectToTrigger);
        REGISTER_FIELD(disableObject);
        REGISTER_FIELD(onlyTriggerOnce);
        REGISTER_FIELD(triggered);
    )
)