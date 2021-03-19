#include "UI/MapView.hpp"
#include "monkecomputer/shared/ViewLib/CustomComputer.hpp"
#include "Behaviours/MapLoader.hpp"
#include "UI/MapSelectorViewManager.hpp"

DEFINE_CLASS(MapLoader::MapView);

extern Logger& getLogger();

using namespace GorillaUI;

namespace MapLoader
{
    void MapView::Awake()
    {
        loaded = false;
    }

    void MapView::DidActivate(bool firstActivation)
    {
        loaded = false;
        Redraw();
    }

    void MapView::Load()
    {
        static std::vector<Il2CppClass*> loaderKlass = {classof(Loader*)};
        Loader* loader = *il2cpp_utils::RunGenericMethod<Loader*>("UnityEngine", "Object", "FindObjectOfType", loaderKlass);
        loader->LoadMap(*mapInfo);
    }

    void MapView::Redraw()
    {
        text = "";

        if (!loaded)
        {
            DrawHeader();
            DrawMap();
        }
        else
        {
            text += "\n\n\n\n\n";
            text += "           Loading Map!";
        }

        CustomComputer::Redraw();
    }
    
    void MapView::DrawHeader()
    {
        text += string_format("<color=#ffff00>== <color=#fdfdfd>Map Details</color> ==</color>\n");
    }
    
    void MapView::DrawMap()
    {
        text += "\n<size=60>";
        text += string_format("  Author: %s\n", mapInfo->packageInfo->descriptor.author.c_str());
        text += string_format("  Map Name: %s\n", mapInfo->packageInfo->descriptor.mapName.c_str());
        text += string_format("  Description: %s\n", mapInfo->packageInfo->descriptor.description.c_str());
        text += "\n</size>";
    }
    
    void MapView::OnKeyPressed(int key)
    {
        if (loaded)
        {
            MapSelectorViewManager* viewManager = reinterpret_cast<MapSelectorViewManager*>(CustomComputer::get_instance()->activeViewManager);
            viewManager->BackButtonWasPressed(this);
            return;
        }
        
        if (key == (int)EKeyboardKey::Enter)
        {
            loaded = true;
            Load();
        }

        Redraw();
    }
}