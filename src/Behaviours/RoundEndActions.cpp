#include "Behaviours/RoundEndActions.hpp"
#include "Behaviours/MapLoader.hpp"

DEFINE_CLASS(MapLoader::RoundEndActions);

namespace MapLoader
{
    void RoundEndActions::Awake()
    {
        static std::vector<const Il2CppClass*> gameObjectKlass = {il2cpp_utils::GetClassFromName("UnityEngine", "GameObject")};
        static Il2CppClass* listKlass = il2cpp_utils::GetClassFromName("System.Collections.Generic", "List`1");
        static Il2CppClass* gameObjectListKlass = il2cpp_utils::MakeGeneric(listKlass, gameObjectKlass);
        objectsToEnable = *il2cpp_utils::New<List<Il2CppObject*>*>(gameObjectListKlass);
        objectsToDisable = *il2cpp_utils::New<List<Il2CppObject*>*>(gameObjectListKlass);
        il2cpp_utils::RunMethod(objectsToEnable, "Clear");
        il2cpp_utils::RunMethod(objectsToDisable, "Clear");
    }

    void RoundEndActions::RoundEnd()
    {
        static std::vector<Il2CppClass*> actionsKlass = {classof(RoundEndActions*)};
        Array<RoundEndActions*>* endActions = *il2cpp_utils::RunGenericMethod<Array<RoundEndActions*>*>("UnityEngine", "Resources", "FindObjectsOfTypeAll", actionsKlass);
        if (!endActions) return;

        for (int i = 0; i < endActions->Length(); i++)
        {
            RoundEndActions* action = endActions->values[i];
            getLogger().info("Found action %d", i);
            for (int j = 0; j < action->objectsToDisable->size; j++)
            {
                il2cpp_utils::RunMethod(action->objectsToDisable->items->values[j], "SetActive", true);
                il2cpp_utils::RunMethod(action->objectsToDisable->items->values[j], "SetActive", false);
            }

            for (int j = 0; j < action->objectsToEnable->size; j++)
            {
                il2cpp_utils::RunMethod(action->objectsToEnable->items->values[j], "SetActive", false);
                il2cpp_utils::RunMethod(action->objectsToEnable->items->values[j], "SetActive", true);
            }

            if (action->respawnOnRoundEnd)
            {
                Loader::ForceRespawn();
            }
        }
    }
}