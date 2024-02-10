#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace object2D
{

    // Create square with given bottom left corner, length and color
    Mesh* CreateSquare(const std::string &name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
    Mesh* CreatePlant(const std::string& name, glm::vec3 leftPoint, glm::vec3 color, bool fill);
    Mesh* CreateStar(const std::string& name, glm::vec3 center, float length, glm::vec3 color, bool fill);
    Mesh* CreateEnemy(const std::string& name, glm::vec3 center, float innerLength, float outerLength, glm::vec3 innerColor, glm::vec3 outerColor, bool fill);
 
}
