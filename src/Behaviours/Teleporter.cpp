#include "Behaviours/Teleporter.hpp"
#include "Behaviours/TagZone.hpp"
#include "Behaviours/Player.hpp"
#include "Behaviours/MapLoader.hpp"

DEFINE_CLASS(MapLoader::Teleporter);

namespace MapLoader
{
    void Teleporter::Awake()
    {
        teleportPoints.clear();
        joinGameOnTeleport = false;
        isTeleporting = false;
        tagOnTeleport = false;
    }

    void Teleporter::Trigger(Il2CppObject* collider)
    {
        if (isTeleporting)
            return;

        isTeleporting = true;
        TeleportPlayer();
    }

    void Teleporter::TeleportPlayer()
    {
        if (teleporterType == TeleporterType::Map)
        {
            static Il2CppString* spawnPointContainerName = il2cpp_utils::createcsstr("SpawnPointContainer", il2cpp_utils::StringType::Manual);
            Il2CppObject* spawnPointContainer = *il2cpp_utils::RunMethod("UnityEngine", "GameObject", "Find", spawnPointContainerName);
            Il2CppObject* containerTransform = *il2cpp_utils::RunMethod(spawnPointContainer, "get_transform");

            int childCount = *il2cpp_utils::RunMethod<int>(containerTransform, "get_childCount");
            teleportPoints.clear();
            
            for (int i = 0; i < childCount; i++)
            {
                Il2CppObject* child = *il2cpp_utils::RunMethod(containerTransform, "GetChild", i);
                teleportPoints.push_back(child);
            }
        }

        if (teleportPoints.size() == 0)
        {
            if (teleporterType == TeleporterType::Map)
            {
                static Il2CppString* treeHomeTargetObjectName = il2cpp_utils::createcsstr("TreeHomeTargetObject", il2cpp_utils::StringType::Manual);
                Il2CppObject* treeHomeTargetObject = *il2cpp_utils::RunMethod("UnityEngine", "GameObject", "Find", treeHomeTargetObjectName);
                Il2CppObject* treeHomeTransform = *il2cpp_utils::RunMethod(treeHomeTargetObject, "get_transform");
                teleportPoints.push_back(treeHomeTransform);
            }
            else 
            {
                getLogger().error("Teleporter point array was empty!");
                return;
            }
        }

        int index = teleportPoints.size() > 1 ? 0 : rand() % teleportPoints.size();
        Il2CppObject* dest = teleportPoints[index]; 
        
        Player::TeleportPlayer(dest);

        if (tagOnTeleport) TagZone::TagLocalPlayer();
        if (joinGameOnTeleport) Loader::JoinGame();
        if (teleporterType == TeleporterType::Treehouse) Loader::ResetMapProperties();

        isTeleporting = false;
    }
}