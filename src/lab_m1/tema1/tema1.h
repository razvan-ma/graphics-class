#pragma once

#include "components/simple_scene.h"
#include <vector>

#define RED glm::vec3(1, 0.25, 0)
#define GREY glm::vec3(0.6, 0.6, 0.6)
#define BLUE glm::vec3(0, 0, 1)
#define YELLOW glm::vec3(0.2, 1, 0)
#define PURPLE glm::vec3(0.2, 0, 0.5)
typedef struct Plot
{
    glm::vec3 color;
    bool isEmpty = 1;
    int x, y;
    float scale = 1;
    float lastTimeShooted = 0;
};
 struct Enemy
{
    bool isAlive;
    glm::vec3 color;
    float x = 1280;
    float y;
    int health = 3;
    float scale = 1;
};
typedef struct Bullet
{
    bool isAlive;
    glm::vec3 color;
    float x;
    float y;
};
namespace m1
{
    class tema1 : public gfxc::SimpleScene
    {
    public:
        tema1();
        ~tema1();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        Plot* getPlotFromPos(int x, int y);
        bool isEnemyOfColorOnLine(glm::vec3 color, int y);
        void shoot(glm::vec3 color, int x, int y, int k);

    protected:
        float cx, cy;
        glm::mat3 modelMatrix;
        float angularStep;
        int health;
        int money;
        float speed; // same for bullet and enemy
        glm::ivec2 trRed, trBlue, trYellow, trPurple; // transforms of shop frames
        // used to render plant at mouse/ on plot
        int holdRed, renderRed, holdBlue, renderBlue, holdYellow, renderYellow, holdPurple, renderPurple;
        Plot plots[9];
        double shootTimes[9] = {0};
        double spawnTime = 2;
        double moneyTime = 4;
        std::vector<glm::vec2> spawnedMoney;
        std::vector<Enemy> enemies;
        std::vector<Bullet> bullets; // bigger weapons bigger weapons
    };

}   // namespace m1
