#pragma once

#include "typedefs.h"
#include <string>
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/document.h"


namespace MapLoader
{
    class MapConfig
    {
        public:
            MapConfig(rapidjson::Value& val)
            {
                imagePath = val["imagePath"].GetString();
                cubeMapImagePath = val["cubemapImagePath"].GetString();
                rapidjson::Value& color = val["mapColor"];

                float r = color["r"].GetFloat();
                float g = color["g"].GetFloat();
                float b = color["b"].GetFloat();
                mapColor = (Color){r, g, b};
                gravity = val.HasMember("gravity") ? val["gravity"].GetFloat() : -9.8f;
            }

            std::string imagePath = "";
            std::string cubeMapImagePath = "";
            Color mapColor = {1.0f, 1.0f, 1.0f};
            float gravity = -9.8f;
    };
}