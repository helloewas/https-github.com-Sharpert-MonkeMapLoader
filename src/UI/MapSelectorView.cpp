#include "UI/MapSelectorView.hpp"
#include "UI/MapView.hpp"
#include "monkecomputer/shared/GorillaUI.hpp"
#include "monkecomputer/shared/Helpers/SelectionHelper.hpp"
#include "monkecomputer/shared/Helpers/PageHelper.hpp"
#include "monkecomputer/shared/ViewLib/CustomComputer.hpp"
#include "Models/MapList.hpp"

DEFINE_CLASS(MapLoader::MapSelectorView);

extern Logger& getLogger();

using namespace GorillaUI;

#define MOD_PAGE_SIZE 8

namespace MapLoader
{
    void MapSelectorView::Awake()
    {
        mapView = nullptr;

        if (!selectionHandler) selectionHandler = new UISelectionHandler(EKeyboardKey::Up, EKeyboardKey::Down, EKeyboardKey::Enter, true, true);
        if (!pageSelectionHandler) pageSelectionHandler = new UISelectionHandler(EKeyboardKey::Left, EKeyboardKey::Right, EKeyboardKey::Enter, false, true);

        selectionHandler->max = MOD_PAGE_SIZE;
        std::vector<MapInfo>& maps = MapList::get_maps();
        mapCount = maps.size();
        pageCount = PageHelper::GetPageAmount(maps, MOD_PAGE_SIZE);
        pageSelectionHandler->max = pageCount;

        if (maps.size() < MOD_PAGE_SIZE)
        {
            selectionHandler->max = maps.size();
        }
    }

    void MapSelectorView::DidActivate(bool firstActivation)
    {
        std::function<void(int)> fun = std::bind(&MapSelectorView::ShowMap, this, std::placeholders::_1);
        selectionHandler->selectionCallback = fun;
        Redraw();        
    }

    void MapSelectorView::ShowMap(int index)
    {
        if (!mapView) mapView = GorillaUI::CreateView<MapView*>();
        if (mapCount == 0) return;
        if (!mapView)
        {
            getLogger().error("Map View Was Not Created!");
            return;
        }

        index += (MOD_PAGE_SIZE * pageSelectionHandler->currentSelectionIndex);
        getLogger().info("Loading map %d", index);
        ((MapView*)mapView)->mapInfo = &MapList::get_map(index);
        CustomComputer* instance = CustomComputer::get_instance();
        if (instance)
        {
            GorillaUI::Components::ViewManager* activeViewManager = instance->activeViewManager;
            if (activeViewManager)
            {
                activeViewManager->ReplaceTopView(mapView);
            }
            else getLogger().error("active ViewManager was nullptr");
        }
        else getLogger().error("Computer instance was nullptr");
    }

    void MapSelectorView::Redraw()
    {
        text = "";

        DrawHeader();
        DrawMaps();

        CustomComputer::Redraw();
    }
    
    void MapSelectorView::DrawHeader()
    {
        text += string_format("<color=#ffff00>== <color=#fdfdfd>Maps</color> ==</color> page %d/%d\n", pageSelectionHandler->currentSelectionIndex, pageCount - 1);
    }
    
    void MapSelectorView::DrawMaps()
    {
        std::vector<MapInfo> infos = PageHelper::GetPage(MapList::get_maps(), MOD_PAGE_SIZE, pageSelectionHandler->currentSelectionIndex);
        selectionHandler->max = infos.size();
        selectionHandler->currentSelectionIndex = selectionHandler->currentSelectionIndex >= infos.size() ? infos.size() - 1 : selectionHandler->currentSelectionIndex;
        std::vector<std::string> mapNames = {};

        for (auto m : infos)
        {
            mapNames.push_back((m.packageInfo->descriptor.mapName));
        }

        SelectionHelper::DrawSelection(mapNames, selectionHandler->currentSelectionIndex, text);
    }
    
    void MapSelectorView::OnKeyPressed(int key)
    {
        selectionHandler->HandleKey((EKeyboardKey)key);
        pageSelectionHandler->HandleKey((EKeyboardKey)key);
        Redraw();
    }
}