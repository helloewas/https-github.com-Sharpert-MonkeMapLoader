#include "Behaviours/TagZone.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "beatsaber-hook/shared/utils/utils.h"

DEFINE_CLASS(MapLoader::TagZone);

namespace MapLoader
{
    void TagZone::Awake()
    {
        canBeTagged = true;
    }

    void TagZone::Trigger(Il2CppObject* collider)
    {
        if (!canBeTagged)
            return;

        canBeTagged = false;
        TagLocalPlayer();

        std::thread ReTag([&](){
            for (int i = 0; i < 1000; i++)
            {
                usleep(1000);
            }

            canBeTagged = true;
        });

        ReTag.detach();
    }

    void TagZone::TagLocalPlayer()
    {
        bool inRoom = *il2cpp_utils::RunMethod<bool>("Photon.Pun", "PhotonNetwork", "get_InRoom");
        if (inRoom)
        {
            Array<Il2CppObject*>* playerList = *il2cpp_utils::RunMethod<Array<Il2CppObject*>*>("Photon.Pun", "PhotonNetwork", "get_PlayerList");
            Il2CppObject* localPlayer = *il2cpp_utils::RunMethod("Photon.Pun", "PhotonNetwork", "get_LocalPlayer");
            for (int i = 0; i < playerList->Length(); i++)
            {
                Il2CppObject* ply = playerList->values[i];
                Il2CppObject* gorillaTagManager = *il2cpp_utils::RunMethod("", "GorillaTagManager", "get_instance");
                static std::vector<Il2CppClass*> gameManagerKlass = {il2cpp_utils::GetClassFromName("", "GorillaGameManager")};
                Il2CppObject* gameManager = *il2cpp_utils::RunGenericMethod(gorillaTagManager, "GetComponent", gameManagerKlass);
                Il2CppObject* photonView = *il2cpp_utils::RunMethod("Photon.Pun", "PhotonView", "Get", gameManager);
                static Il2CppString* reportTagRPC = il2cpp_utils::createcsstr("ReportTagRPC", il2cpp_utils::StringType::Manual);
                int target = 2;
                Array<Il2CppObject*>* sendArr = reinterpret_cast<Array<Il2CppObject*>*>(il2cpp_functions::array_new(classof(Il2CppObject*), 2));
                sendArr->values[0] = ply;
                sendArr->values[1] = localPlayer;

                il2cpp_utils::RunMethod(photonView, "RPC", reportTagRPC, target, sendArr);
            }
        }
    }
}