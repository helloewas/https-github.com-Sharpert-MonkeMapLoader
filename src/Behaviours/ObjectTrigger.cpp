#include "Behaviours/ObjectTrigger.hpp"

DEFINE_CLASS(MapLoader::ObjectTrigger);

namespace MapLoader
{
    void ObjectTrigger::Awake()
    {
        static std::vector<const Il2CppClass*> gameObjectKlass = {il2cpp_utils::GetClassFromName("UnityEngine", "GameObject")};
        static Il2CppClass* listKlass = il2cpp_utils::GetClassFromName("System.Collections.Generic", "List`1");
        static Il2CppClass* gameObjectListKlass = il2cpp_utils::MakeGeneric(listKlass, gameObjectKlass);
        objectsToTrigger = *il2cpp_utils::New<List<Il2CppObject*>*>(gameObjectListKlass);
    }

    void ObjectTrigger::OnEnable()
    {
        for (int i = 0; i < objectsToTrigger->size; i++)
        {
            Il2CppObject* objectToTrigger = objectsToTrigger->items->values[i];
            il2cpp_utils::RunMethod(objectToTrigger, "SetActive", disableObject);
        }

        triggered = false;
    }

    void ObjectTrigger::Trigger(Il2CppObject* collider)
    {
        if (triggered && onlyTriggerOnce)
            return;
        
        for (int i = 0; i < objectsToTrigger->size; i++)
        {
            Il2CppObject* objectToTrigger = objectsToTrigger->items->values[i];
            il2cpp_utils::RunMethod(objectToTrigger, "SetActive", disableObject);
            il2cpp_utils::RunMethod(objectToTrigger, "SetActive", !disableObject);
        }
        

        triggered = true;
    }
}