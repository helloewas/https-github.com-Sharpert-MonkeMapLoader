#pragma once

#include "custom-types/shared/macros.hpp"
#include "typedefs.h"
#include "Models/MapInfo.hpp"

namespace MapLoader
{
    class PreviewOrb;
}

DECLARE_CLASS(MapLoader, PreviewOrb, "UnityEngine", "MonoBehaviour", sizeof(Il2CppObject) + sizeof(void*) * 2,
    DECLARE_METHOD(void, Start);
    
    public:
        static inline PreviewOrb* instance = nullptr;
        static void ChangeOrb(Color color, Il2CppObject* texture);
        static void ChangeOrb(MapInfo& info);
    REGISTER_FUNCTION(PreviewOrb,
        REGISTER_METHOD(Start);
    )
)