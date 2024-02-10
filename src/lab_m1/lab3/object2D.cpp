#include "object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2D::CreateSquare(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, -1), color),
        VertexFormat(corner + glm::vec3(length, length, -1), color),
        VertexFormat(corner + glm::vec3(0, length, -1), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}
Mesh* object2D::CreatePlant(
    const std::string& name,
    glm::vec3 leftPoint,
    glm::vec3 color,
    bool fill)
{
    // length on X axis
    float length = 80;
    // Y distance from the center point
    float barrelOffset = 10;
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(leftPoint, color),
        VertexFormat(leftPoint + glm::vec3(20, 50, 1), color),
        VertexFormat(leftPoint + glm::vec3(length / 2, barrelOffset, 1), color),
        VertexFormat(leftPoint + glm::vec3(length, barrelOffset, 1), color),
        VertexFormat(leftPoint + glm::vec3(length, -barrelOffset, 1), color),
        VertexFormat(leftPoint + glm::vec3(length / 2, -barrelOffset, 1), color),
        VertexFormat(leftPoint + glm::vec3(20, -50, 1), color),
    };

    Mesh* plant = new Mesh(name);
    std::vector<unsigned int> indices = 
    {   0, 1, 2,
        2, 3, 5,
        3, 4, 5,
        0, 2, 5,
        0, 5, 6
    };
    plant->InitFromData(vertices, indices);
    return plant;
}
Mesh* object2D::CreateStar(
    const std::string& name,
    glm::vec3 center,
    float length,
    glm::vec3 color,
    bool fill)
{
    // length on X axis
    // Y distance from the center point
    constexpr float pi = glm::pi<float>();
    float length2 = length / 3;
    std::vector<VertexFormat> vertices;
    for (int i = 0; i < 5; i++)
    {
        vertices.push_back(VertexFormat(center + glm::vec3(length * cos(2 * pi * i / 5 + pi / 2), length * sin(2 * pi * i / 5 + pi / 2), 2), color));
    }
    vertices.push_back(VertexFormat(center + glm::vec3(length2 * cos(2 * pi * 2 / 5 + pi / 2 + pi / 5), length2 * sin(2 * pi * 2 / 5 + pi / 2 + pi / 5), 2), color));
    Mesh* star = new Mesh(name);
    std::vector<unsigned int> indices =
    { 
        0,5,2,
        0,3,5,
        1,4,5      
    };
    star->InitFromData(vertices, indices);
    return star;
}
Mesh* object2D::CreateEnemy(const std::string& name, glm::vec3 center, float innerLength, float outerLength, glm::vec3 innerColor, glm::vec3 outerColor, bool fill)
{
    // length on X axis
    // Y distance from the center point
    constexpr float pi = glm::pi<float>();
    std::vector<VertexFormat> vertices;
    for (int i = 0; i < 6; i++)
    {
        vertices.push_back(VertexFormat(center + glm::vec3(outerLength * sin(pi * i / 3), outerLength * cos(pi * i / 3), 3), outerColor));
    }
    for (int i = 0; i < 6; i++)
    {
        vertices.push_back(VertexFormat(center + glm::vec3(innerLength * sin(pi * i / 3), innerLength * cos(pi * i / 3), 3), innerColor));
    }
    
    Mesh* enemy = new Mesh(name);
    std::vector<unsigned int> indices =
    {
        0,4,5,
        0,1,4,
        1,2,4,
        4,2,3,
        // second one
        6,10,11,
        6,7,10,
        10,7,8,
        10,8,9
    };
    enemy->InitFromData(vertices, indices);
    return enemy;
}
