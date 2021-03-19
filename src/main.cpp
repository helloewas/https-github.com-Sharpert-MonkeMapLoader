#include "modloader/shared/modloader.hpp"
#include "beatsaber-hook/shared/utils/logging.hpp"
#include "beatsaber-hook/shared/utils/utils.h"
#include "cosmeticsloader/shared/CosmeticLoader.hpp"
#include "typedefs.h"
#include <thread>

#include "Behaviours/GorillaMapTriggerBase.hpp"
#include "Behaviours/ObjectTrigger.hpp"
#include "Behaviours/PreviewOrb.hpp"
#include "Behaviours/RotateByHand.hpp"
#include "Behaviours/RoundEndActions.hpp"
#include "Behaviours/SurfaceClimbSettings.hpp"
#include "Behaviours/TagZone.hpp"
#include "Behaviours/Teleporter.hpp"
#include "Behaviours/Player.hpp"
#include "Behaviours/MapDescriptor.hpp"
#include "Behaviours/MapLoader.hpp"

#include "UI/MapView.hpp"
#include "UI/MapSelectorView.hpp"
#include "UI/MapSelectorViewManager.hpp"

#include "monkecomputer/shared/Register.hpp"

#include "Models/MapList.hpp"

#include "custom-types/shared/register.hpp"
#include "Utils/FileUtils.hpp"

ModInfo modInfo;

Logger& getLogger()
{
    static Logger* logger = new Logger(modInfo, LoggerOptions(false, true));
    return *logger;
}

bool getSceneName(Scene scene, std::string& output);

using namespace CosmeticsLoader;
using namespace MapLoader;

MAKE_HOOK_OFFSETLESS(Player_Awake, void, Il2CppObject* self)
{
    Player_Awake(self);
    Il2CppObject* gameObject = *il2cpp_utils::RunMethod(self, "get_gameObject");
    il2cpp_utils::RunGenericMethod(gameObject, "AddComponent", std::vector<Il2CppClass*>{classof(MapLoader::Player*)});
}

MAKE_HOOK_OFFSETLESS(GorillaComputer_Start, void, Il2CppObject* self)
{
    GorillaComputer_Start(self);
    MapList::Load();

    static std::vector<Il2CppClass*> goKlass = {il2cpp_utils::GetClassFromName("UnityEngine", "GameObject")};

    Il2CppObject* newGo = *il2cpp_utils::New(goKlass[0]);
    Loader* loader = *il2cpp_utils::RunGenericMethod<Loader*>(newGo, "AddComponent", std::vector<Il2CppClass*>{classof(Loader*)});
    loader->Initialize();
}
static bool disabledBecauseUnclimbable = false;
MAKE_HOOK_OFFSETLESS(Player_CollisionsSphereCast, bool, Il2CppObject* self, Vector3 startPosition, float sphereRadius, Vector3 movementVector, float precision, Vector3* finalPosition, RaycastHit* hitInfo)
{
    bool result = Player_CollisionsSphereCast(self, startPosition, sphereRadius, movementVector, precision, finalPosition, hitInfo);

    if (result)
    {
        float minimumRaycastDistance = *il2cpp_utils::GetFieldValue<float>(self, "minimumRaycastDistance");
        if (sphereRadius == minimumRaycastDistance)
        {
            Il2CppObject* collider = *il2cpp_utils::RunMethod(*hitInfo, "get_collider");
            Il2CppObject* go = *il2cpp_utils::RunMethod(collider, "get_gameObject");

            static std::vector<Il2CppClass*> surfaceKlass = {classof(SurfaceClimbSettings*)};
            SurfaceClimbSettings* settings = *il2cpp_utils::RunGenericMethod<SurfaceClimbSettings*>(go, "GetComponent", surfaceKlass);

            if (settings && settings->unClimbable)
            {
                bool disableMovement = *il2cpp_utils::GetFieldValue<bool>(self, "disableMovement");

                if (!disableMovement)
                {
                    il2cpp_utils::SetFieldValue(self, "disableMovement", true);
                    disabledBecauseUnclimbable = true;
                }
            }
        }
    }

    return result;
}

MAKE_HOOK_OFFSETLESS(Player_Update, void, Il2CppObject* self)
{
    Player_Update(self);

    bool disableMovement = *il2cpp_utils::GetFieldValue<bool>(self, "disableMovement");
    if (disableMovement && disabledBecauseUnclimbable)
    {
        il2cpp_utils::SetFieldValue(self, "disableMovement", false);
        disabledBecauseUnclimbable = false;
    }
}

static double lastGameEnd = 0.0;
MAKE_HOOK_OFFSETLESS(VRRig_PlayTagSound, void, Il2CppObject* self, int soundIndex)
{
    VRRig_PlayTagSound(self, soundIndex);

    Il2CppObject* photonNetworkController = *il2cpp_utils::GetFieldValue("", "PhotonNetworkController", "instance");
    if (photonNetworkController)
    {
        Il2CppString* currentGameTypeCS = *il2cpp_utils::GetFieldValue<Il2CppString*>(photonNetworkController, "currentGameType");
        std::string currentGameType = currentGameTypeCS ? to_utf8(csstrtostr(currentGameTypeCS)) : "";

        if (Loader::lobbyName != "" && currentGameType.find(Loader::lobbyName) != std::string::npos)
        {
            Il2CppObject* gorillaTagManager = *il2cpp_utils::GetFieldValue("", "GorillaTagManager", "instance");
            bool isCurrentlyTag = *il2cpp_utils::GetFieldValue<bool>(gorillaTagManager, "isCurrentlyTag");
            double timeInfectedGameEnded = *il2cpp_utils::GetFieldValue<double>(gorillaTagManager, "timeInfectedGameEnded");
            if (timeInfectedGameEnded > lastGameEnd)
            {
                lastGameEnd = timeInfectedGameEnded;
                RoundEndActions::RoundEnd();
            }
            else if (isCurrentlyTag) RoundEndActions::RoundEnd();
        }
    }
}

extern "C" void setup(ModInfo& info)
{
    info.id = ID;
    info.version = VERSION;

    modInfo = info;
}

extern "C" void load()
{
    getLogger().info("Loading mod...");
    Modloader::requireMod("MonkeComputer", "1.0.1");
    std::string mapDir = "/sdcard/ModData/com.AnotherAxiom.GorillaTag/Mods/MonkeMapLoader/CustomMaps/";
    FileUtils::mkdir(mapDir);

    INSTALL_HOOK_OFFSETLESS(getLogger(), GorillaComputer_Start, il2cpp_utils::FindMethodUnsafe("", "GorillaComputer", "Start", 0));
    INSTALL_HOOK_OFFSETLESS(getLogger(), Player_Awake, il2cpp_utils::FindMethodUnsafe("GorillaLocomotion", "Player", "Awake", 0));
    INSTALL_HOOK_OFFSETLESS(getLogger(), Player_CollisionsSphereCast, il2cpp_utils::FindMethodUnsafe("GorillaLocomotion", "Player", "CollisionsSphereCast", 6));
    INSTALL_HOOK_OFFSETLESS(getLogger(), Player_Update, il2cpp_utils::FindMethodUnsafe("GorillaLocomotion", "Player", "Update", 0));
    INSTALL_HOOK_OFFSETLESS(getLogger(), VRRig_PlayTagSound, il2cpp_utils::FindMethodUnsafe("", "VRRig", "PlayTagSound", 1));
    
    using namespace MapLoader;
    custom_types::Register::RegisterType<GorillaMapTriggerBase>();
    custom_types::Register::RegisterTypes<Teleporter, TagZone, Player, ObjectTrigger, RotateByHand, PreviewOrb, SurfaceClimbSettings, RoundEndActions, Loader, MapDescriptor>();
    custom_types::Register::RegisterTypes<MapSelectorView, MapView, MapSelectorViewManager>();

    GorillaUI::Register::RegisterViewManager<MapSelectorViewManager*>("Map Loader", VERSION);

    getLogger().info("Mod loaded!");
}

bool getSceneName(Scene scene, std::string& output)
{
    Il2CppString* csString = *il2cpp_utils::RunMethod<Il2CppString*>("UnityEngine.SceneManagement", "Scene", "GetNameInternal", scene.m_Handle);
    if (!csString) return false;
    output = to_utf8(csstrtostr(csString));
    return true; 
}