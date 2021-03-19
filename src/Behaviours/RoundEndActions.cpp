#include "Behaviours/RoundEndActions.hpp"
#include "Behaviours/MapLoader.hpp"

DEFINE_CLASS(MapLoader::RoundEndActions);

namespace MapLoader
{
    void RoundEndActions::Start()
    {
        instance = this;
    }

    void RoundEndActions::RoundEnd()
    {
        static std::vector<Il2CppClass*> actionsKlass = {classof(RoundEndActions*)};

        Array<RoundEndActions*>* endActions = *il2cpp_utils::RunGenericMethod<Array<RoundEndActions*>*>("UnityEngine", "Resources", "FindObjectsOfTypeAll", actionsKlass);
        if (endActions) return;
        for (int i = 0; i < endActions->Length(); i++)
        {
            RoundEndActions* action = endActions->values[i];

            for (auto o : action->objectsToDisable)
            {
                il2cpp_utils::RunMethod(o, "SetActive", true);
                il2cpp_utils::RunMethod(o, "SetActive", false);
            }

            for (auto o : action->objectsToEnable)
            {
                il2cpp_utils::RunMethod(o, "SetActive", false);
                il2cpp_utils::RunMethod(o, "SetActive", true);
            }

            if (action->respawnOnRoundEnd)
            {
                Loader::ForceRespawn();
            }
        }
    }
}