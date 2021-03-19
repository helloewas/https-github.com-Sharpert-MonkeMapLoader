#include "Utils/RoomUtils.hpp"
#include "beatsaber-hook/shared/utils/utils.h"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"

extern Logger& getLogger();
namespace MapLoader::RoomUtils
{
    void JoinModdedLobby(std::string map)
    {
        std::string gameModeName = "infection_MOD_" + map;
        getLogger().info("Attempting to join a room with gamemode %s", gameModeName.c_str());
        Il2CppObject* photonNetworkController = *il2cpp_utils::GetFieldValue("", "PhotonNetworkController", "instance");

        bool InRoom = *il2cpp_utils::RunMethod<bool>("Photon.Pun", "PhotonNetwork", "get_InRoom");

        Il2CppObject* currentRoom = *il2cpp_utils::RunMethod("Photon.Pun", "PhotonNetwork", "get_CurrentRoom");
        std::string gameMode = "";
        if (currentRoom)
        {
            Il2CppObject* customProperties = *il2cpp_utils::RunMethod(currentRoom, "get_CustomProperties");
            static Il2CppString* gameModeLabel = il2cpp_utils::createcsstr("gameMode", il2cpp_utils::StringType::Manual);
            Il2CppString* gameModeCS = *il2cpp_utils::RunMethod<Il2CppString*>(customProperties, "get_Item", (Il2CppObject*)gameModeLabel);
            gameMode = gameModeCS ? to_utf8(csstrtostr(gameModeCS)) : "";
        }
        if (InRoom && gameMode.find("privatetag") != std::string::npos) return;

        Il2CppString* gameType = il2cpp_utils::createcsstr(gameModeName);

        Il2CppString* queueCS = il2cpp_utils::createcsstr("DEFAULT");
        Il2CppObject* gorillaComputer = *il2cpp_utils::GetFieldValue("", "GorillaComputer", "instance");
        il2cpp_utils::SetFieldValue(gorillaComputer, "currentQueue", queueCS);

        il2cpp_utils::RunMethod("UnityEngine", "PlayerPrefs", "SetString", il2cpp_utils::createcsstr("currentQueue"), queueCS);
        il2cpp_utils::RunMethod("UnityEngine", "PlayerPrefs", "Save");

        if (InRoom && gameMode != gameModeName)
        {
            il2cpp_utils::SetFieldValue(photonNetworkController, "currentGameType", gameType);
            il2cpp_utils::SetFieldValue(photonNetworkController, "attemptingToConnect", true);

            Array<Il2CppObject*>* offlineVRRig = *il2cpp_utils::GetFieldValue<Array<Il2CppObject*>*>(photonNetworkController, "offlineVRRig");

            for (int i = 0; i < offlineVRRig->Length(); i++)
            {
                Il2CppObject* skinnedMeshRenderer = offlineVRRig->values[i];
                il2cpp_utils::RunMethod(skinnedMeshRenderer, "set_enabled", true);
            }

            il2cpp_utils::RunMethod("Photon.Pun", "PhotonNetwork", "Disconnect");
            getLogger().info("Disconnected from room");
            return;
        }

        bool attemptingToConnect = *il2cpp_utils::GetFieldValue<bool>(photonNetworkController, "attemptingToConnect");
        if (!InRoom && !attemptingToConnect)
        {
            il2cpp_utils::SetFieldValue(photonNetworkController, "currentGameType", gameType);
            il2cpp_utils::SetFieldValue(photonNetworkController, "attemptingToConnect", true);
            il2cpp_utils::RunMethod(photonNetworkController, "AttemptToConnectToRoom");
            getLogger().info("Attempting room connect");
        }
    }
}