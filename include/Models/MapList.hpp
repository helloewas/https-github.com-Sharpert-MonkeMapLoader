#pragma once
#include "Models/MapInfo.hpp"
#include <vector>

namespace MapLoader
{
    class MapList
    {
        public:
            static void Load();

            static std::vector<MapInfo>& get_maps()
            {
                return maps;
            }

            static MapInfo& get_map(int i)
            {
                return maps[i];
            }

        private:
            static inline std::vector<MapInfo> maps = {};

    };
}