#pragma once

#include <string>
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/document.h"

namespace MapLoader
{
    class Descriptor
    {
        public:
            Descriptor(rapidjson::Value& val)
            {
                author = val["author"].GetString();
                mapName = val["objectName"].GetString();
                description = val["description"].GetString();
            }
            
            std::string author = "";
            std::string mapName = "";
            std::string description = "";
    };
}