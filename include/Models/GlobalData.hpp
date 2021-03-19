#pragma once
#include "typedefs.h"

namespace MapLoader
{
    class GlobalData
    {
        public: 
            Vector3 get_origin()
            {
                return origin;
            }

            void set_origin(Vector3 value)
            {
                if (
                    origin.x == value.x &&
                    origin.y == value.y &&
                    origin.z == value.z
                )
                return;

                origin = value;
                customOrigin = value;
                customOrigin.y += 54.0f;
            }

            Vector3 treeOrigin = {0.0f, 0.0f, 0.0f};

            Il2CppObject* bigTreeTeleportToMap = nullptr;
            
            Il2CppObject* bigTreePoint = nullptr;

            Il2CppObject* fallEmergencyTeleport = nullptr;

            bool isLegacyMap = false;
        private:
            Vector3 origin = {0.0f, 0.0f, 0.0f};
            Vector3 customOrigin = {0.0f, 54.0f, 0.0f};
    };
}