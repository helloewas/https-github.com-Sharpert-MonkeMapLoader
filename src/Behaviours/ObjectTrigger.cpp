#include "Behaviours/ObjectTrigger.hpp"

DEFINE_CLASS(MapLoader::ObjectTrigger);

namespace MapLoader
{
    void ObjectTrigger::Start()
    {
        il2cpp_utils::RunMethod(objectToTrigger, "SetActive", disableObject);
        triggered = false;
    }

    void ObjectTrigger::Trigger(Il2CppObject* collider)
    {
        if (triggered && onlyTriggerOnce)
            return;
        
        il2cpp_utils::RunMethod(objectToTrigger, "SetActive", disableObject);
        il2cpp_utils::RunMethod(objectToTrigger, "SetActive", !disableObject);

        triggered = true;
    }
}