#include "Behaviours/SurfaceClimbSettings.hpp"

DEFINE_CLASS(MapLoader::SurfaceClimbSettings);

namespace MapLoader
{
    void SurfaceClimbSettings::Start()
    {
        static std::vector<Il2CppClass*> klass = {il2cpp_utils::GetClassFromName("GorillaLocomotion", "Surface")};
        Il2CppObject* go = *il2cpp_utils::RunMethod(this, "get_gameObject");
        Il2CppObject* surface = *il2cpp_utils::RunGenericMethod(go, "GetComponent", klass);

        if (!surface)
        {
            surface = *il2cpp_utils::RunGenericMethod(go, "AddComponent", klass);
        }

        if (unClimbable)
            il2cpp_utils::SetFieldValue(surface, "slipPercentage", 1.0f);
        else 
            il2cpp_utils::SetFieldValue(surface, "slipPercentage", slipPercentage);

    }
}