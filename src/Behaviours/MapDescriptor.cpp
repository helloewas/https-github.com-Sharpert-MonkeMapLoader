#include "Behaviours/MapDescriptor.hpp"
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/document.h"
#include <unordered_map>
#include <map>
#include "Behaviours/Teleporter.hpp"
#include "Behaviours/TagZone.hpp"
#include "Behaviours/ObjectTrigger.hpp"
#include "Behaviours/SurfaceClimbSettings.hpp"
#include "Behaviours/RoundEndActions.hpp"

DEFINE_CLASS(MapLoader::MapDescriptor);

extern Logger& getLogger();

namespace MapLoader
{
    using MapBehaviourRepresentation = struct MapBehaviourRepresentation{
            Il2CppObject* first;
            std::string second;
        };
    using SerializeMap = std::vector<MapBehaviourRepresentation>; 
    using TeleporterMap = std::map<std::string, Teleporter*>;
    using TriggerMap = std::map<std::string, ObjectTrigger*>;

    void MapDescriptor::Initialize()
    {
        static std::vector<Il2CppClass*> textKlass = {il2cpp_utils::GetClassFromName("UnityEngine.UI", "Text")};
        Il2CppObject* go = *il2cpp_utils::RunMethod(this, "get_gameObject");
        Array<Il2CppObject*>* textComponents = *il2cpp_utils::RunGenericMethod<Array<Il2CppObject*>*>(go, "GetComponentsInChildren", textKlass, true);
        
        

        SerializeMap teleporters = {};
        SerializeMap teleportPoints = {};
        SerializeMap objectTriggers = {};
        SerializeMap triggerPoints = {};
        SerializeMap tagZones = {};
        SerializeMap surfaces = {};
        SerializeMap roundEndActions = {};
        SerializeMap actionObjects = {};


        getLogger().info("Handling all Text Components");
        for (int i = 0; i < textComponents->Length(); i++)
        {
            Il2CppObject* component = textComponents->values[i];
            Il2CppString* serializedCS = *il2cpp_utils::RunMethod<Il2CppString*>(component, "get_text");
            std::string serialized = to_utf8(csstrtostr(serializedCS));

            if (serialized.find("{") == std::string::npos || serialized.find("}") == std::string::npos) continue;
            Il2CppObject* gameObject = *il2cpp_utils::RunMethod(component, "get_gameObject");

            while (serialized.find("}, {") != std::string::npos)
            {
                getLogger().error("serialized was: \n%s", serialized.c_str());
                int seperationpos = serialized.find("}, {");
                std::string subObj = serialized.substr(0, seperationpos + 1);
                serialized = serialized.erase(0, seperationpos + 3);
                getLogger().error("serialized is now: \n%s", serialized.c_str());
                getLogger().error("subObj: \n%s", subObj.c_str());
                if (subObj.find("TeleporterName") != std::string::npos) teleporters.push_back({gameObject, subObj});
                else if (subObj.find("TeleportPoint") != std::string::npos) teleportPoints.push_back({gameObject, subObj});
                else if (subObj.find("ObjectTriggerName") != std::string::npos) objectTriggers.push_back({gameObject, subObj});
                else if (subObj.find("TriggeredBy") != std::string::npos) triggerPoints.push_back({gameObject, subObj});
                else if (subObj.find("TagZone") != std::string::npos) tagZones.push_back({gameObject, subObj});
                else if (subObj.find("SurfaceClimbSettings") != std::string::npos) surfaces.push_back({gameObject, subObj});
                else if (subObj.find("RoundEndActions") != std::string::npos) roundEndActions.push_back({gameObject, subObj});
                else if (subObj.find("RoundEndAction") != std::string::npos) actionObjects.push_back({gameObject, subObj});
                else getLogger().error("Could not find object type for object with textcomponent:\n%s", subObj.c_str());
            }

            if (serialized.find("TeleporterName") != std::string::npos) teleporters.push_back({gameObject, serialized});
            else if (serialized.find("TeleportPoint") != std::string::npos) teleportPoints.push_back({gameObject, serialized});
            else if (serialized.find("ObjectTriggerName") != std::string::npos) objectTriggers.push_back({gameObject, serialized});
            else if (serialized.find("TriggeredBy") != std::string::npos) triggerPoints.push_back({gameObject, serialized});
            else if (serialized.find("TagZone") != std::string::npos) tagZones.push_back({gameObject, serialized});
            else if (serialized.find("SurfaceClimbSettings") != std::string::npos) surfaces.push_back({gameObject, serialized});
            else if (serialized.find("RoundEndActions") != std::string::npos) roundEndActions.push_back({gameObject, serialized});
            else if (serialized.find("RoundEndAction") != std::string::npos) actionObjects.push_back({gameObject, serialized});
            else getLogger().error("Could not find object type for object with textcomponent:\n%s", serialized.c_str());
        }

        // initialize teleporters
        TeleporterMap teleporterMap = {};
        static std::vector<Il2CppClass*> teleporterKlass = {classof(Teleporter*)};
        getLogger().info("Handling teleporters");
        for (auto t : teleporters)
        {
            Teleporter* tele = *il2cpp_utils::RunGenericMethod<Teleporter*>(t.first, "AddComponent", teleporterKlass);
            rapidjson::Document doc;
            doc.Parse(t.second.c_str());

            TouchType touchType = (TouchType)doc["TouchType"].GetInt();
            float delay = doc["Delay"].GetFloat();
            std::string teleporterName = doc["TeleporterName"].GetString();
            bool tagOnTeleport = doc["TagOnTeleport"].GetBool();

            teleporterMap[teleporterName] = tele;

            tele->touchType = touchType;
            tele->Delay = delay;
            tele->tagOnTeleport = tagOnTeleport;
            getLogger().info("\t%s", teleporterName.c_str());
        }    

        // initialize teleport points
        getLogger().info("Handling teleportPoints");
        for (auto t : teleportPoints)
        {
            Il2CppObject* transform = *il2cpp_utils::RunMethod(t.first, "get_transform");
            rapidjson::Document doc;
            doc.Parse(t.second.c_str());

            std::string teleporter = doc["TeleportPoint"].GetString();

            il2cpp_utils::RunMethod(teleporterMap[teleporter]->teleportPoints, "Add", transform);
        }

        static Il2CppString* treeHomeTargetObjectName = il2cpp_utils::createcsstr("TreeHomeTargetObject", il2cpp_utils::StringType::Manual);
        Il2CppObject* treeHomeTargetObject = *il2cpp_utils::RunMethod("UnityEngine", "GameObject", "Find", treeHomeTargetObjectName);
        Il2CppObject* treeHomeTransform = *il2cpp_utils::RunMethod(treeHomeTargetObject, "get_transform");

        // if there are 0 points, set type to treehouse
        for (auto t : teleporterMap)
        {
            int pointCount = t.second->teleportPoints->size;
            if (pointCount == 0) 
            {
                t.second->teleporterType = TeleporterType::Treehouse;
                il2cpp_utils::RunMethod(t.second->teleportPoints, "Add", treeHomeTransform);
            }
        }
        
        // initialize object triggers
        static std::vector<Il2CppClass*> triggerKlass = {classof(ObjectTrigger*)};
        TriggerMap triggerMap = {};
        getLogger().info("Handling objectTriggers");
        for (auto t : objectTriggers)
        {
            ObjectTrigger* trigger = *il2cpp_utils::RunGenericMethod<ObjectTrigger*>(t.first, "AddComponent", triggerKlass);
            rapidjson::Document doc;
            doc.Parse(t.second.c_str());

            std::string objectTriggerName = doc["ObjectTriggerName"].GetString();
            bool onlyTriggerOnce = doc["OnlyTriggerOnce"].GetBool();
            bool disableObject = doc["DisableObject"].GetBool();
            
            trigger->onlyTriggerOnce = onlyTriggerOnce;
            trigger->disableObject = disableObject;

            triggerMap[objectTriggerName] = trigger;

            getLogger().info("\t%s", objectTriggerName.c_str());
        }

        // initialize trigger points
        getLogger().info("Handling triggered");
        for (auto t : triggerPoints)
        {
            rapidjson::Document doc;
            doc.Parse(t.second.c_str());
            std::string trigger = doc["TriggeredBy"].GetString();
            
            il2cpp_utils::RunMethod(triggerMap[trigger]->objectsToTrigger, "Add", t.first);
            getLogger().info("\t%s", trigger.c_str());
        }

        // initialize tagZones
        getLogger().info("Handling TagZones");
        static std::vector<Il2CppClass*> tagZoneKlass = {classof(TagZone*)};
        for (auto t : tagZones)
        {
            TagZone* zone = *il2cpp_utils::RunGenericMethod<TagZone*>(t.first, "AddComponent", tagZoneKlass);
        }

        // initialize surfaces
        getLogger().info("Handling Surfaces");
        static std::vector<Il2CppClass*> surfaceKlass = {classof(SurfaceClimbSettings*)};
        for (auto s : surfaces)
        {
            SurfaceClimbSettings* surface = *il2cpp_utils::RunGenericMethod<SurfaceClimbSettings*>(s.first, "AddComponent", surfaceKlass);
            rapidjson::Document doc;
            doc.Parse(s.second.c_str());
            
            bool unclimbable = doc["Unclimbable"].GetBool();
            float slipPercentage = doc["slipPercentage"].GetFloat();

            surface->unClimbable = unclimbable;
            surface->slipPercentage = slipPercentage;
            surface->Start();
        }
        
        getLogger().info("Handling Round end");
        static std::vector<Il2CppClass*> roundEndKlass = {classof(RoundEndActions*)};
        std::vector<RoundEndActions*> endActions = {};
        for (auto r : roundEndActions)
        {
            RoundEndActions* roundEndAction = *il2cpp_utils::RunGenericMethod<RoundEndActions*>(r.first, "AddComponent", roundEndKlass);
            rapidjson::Document doc;
            doc.Parse(r.second.c_str());

            bool respawnOnRoundEnd = doc["RespawnOnRoundEnd"].GetBool();

            roundEndAction->respawnOnRoundEnd = respawnOnRoundEnd;
            endActions.push_back(roundEndAction);
        }

        for (auto a : actionObjects)
        {
            rapidjson::Document doc;
            doc.Parse(a.second.c_str());
            std::string action = doc["RoundEndAction"].GetString();
            for (auto endAction : endActions)
            {
                if (action == "Disable")
                {
                    il2cpp_utils::RunMethod(endAction->objectsToDisable, "Add", a.first);
                }
                else if (action == "Enable")
                {
                    il2cpp_utils::RunMethod(endAction->objectsToEnable, "Add", a.first);
                }
            }
        }
    }
}