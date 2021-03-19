#pragma once

#include <string>
#include "Models/MapPackageInfo.hpp"
namespace MapLoader
{
    class MapInfo
    {
        public:
            MapInfo(std::string filePath) : filePath(filePath) {};
            void Load();
            Il2CppObject* LoadCubeMap();
            std::string filePath = "";
            MapPackageInfo* packageInfo;
    };
}