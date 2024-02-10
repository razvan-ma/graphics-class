#include "lab_m1/tema1/tema1.h"
#include <iostream>

#include "lab_m1/tema1/transform2D.h"
#include "lab_m1/lab3/object2D.h"

using namespace std;
using namespace m1;
using namespace transform2D;
using namespace object2D;

tema1::tema1()
{
    speed = 100;
    health = 3;
    money = 5;
}


tema1::~tema1()
{
}
Plot* tema1::getPlotFromPos(int x, int y) {
    for (int i = 0; i < 9; i++)
        if (x >= plots[i].x && x < plots[i].x + 120
            && y >= plots[i].y && y < plots[i].y + 120) 
            return &plots[i];
    return NULL;
}

bool tema1::isEnemyOfColorOnLine(glm::vec3 color, int y){
    for (int i = 0; i < enemies.size(); i++)
    {
        if (enemies[i].y == y && enemies[i].color == color)
            return true;
    }
    return false;
}

void tema1::shoot(glm::vec3 color, int x, int y, int k) {
    if (plots[k].lastTimeShooted + 2 < glfwGetTime()) {
        
        plots[k].lastTimeShooted = glfwGetTime();
        //cout << "trage la " << plots[k].lastTimeShooted << endl;
        Bullet bullet;
        bullet.color = color;
        bullet.x = x;
        bullet.y = y;
        bullets.push_back(bullet);
    }
}

bool collide(int x1, int y1, int len1, int x2, int y2, int len2) {
    if (glm::distance(glm::vec3(x1, y1, 0), glm::vec3(x2, y2, 0)) <= len1 + len2)
        return true;
    else
        return false;
}

void tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0, 0, 0);
    float squareSide = 100;

    angularStep = 10;

    
    trRed = glm::ivec2(50, 550);
    trBlue = glm::ivec2(200, 550);
    trYellow = glm::ivec2(350, 550);
    trPurple = glm::ivec2(500, 550);

    Mesh* barrier = CreateSquare("barrier", corner, squareSide, glm::vec3(1, 0, 0), true);
    AddMeshToList(barrier);

    Mesh* plot = CreateSquare("plot", corner, squareSide, glm::vec3(0.2, 0.9, 0.4), true);
    AddMeshToList(plot);

    Mesh* frame = CreateSquare("frame", corner, squareSide, glm::vec3(0.1, 0.1, 0.1), false);
    AddMeshToList(frame);

    Mesh* redPlant = CreatePlant("redPlant", corner, RED, true);
    AddMeshToList(redPlant);

    Mesh* bluePlant = CreatePlant("bluePlant", corner, glm::vec3(0, 0, 1), true);
    AddMeshToList(bluePlant);

    Mesh* yellowPlant = CreatePlant("yellowPlant", corner, glm::vec3(0.2, 1, 0), true);
    AddMeshToList(yellowPlant);

    Mesh* purplePlant = CreatePlant("purplePlant", corner, glm::vec3(0.2, 0, 0.5), true);
    AddMeshToList(purplePlant);

    Mesh* greyStar = CreateStar("greyStar", corner, 40 ,glm::vec3(0.6, 0.6, 0.6), true);
    AddMeshToList(greyStar);

    Mesh* redEnemy = CreateEnemy("redEnemy", corner, 50 , 30 , RED, GREY, true);
    AddMeshToList(redEnemy);
    Mesh* blueEnemy = CreateEnemy("blueEnemy", corner, 50, 30, BLUE, GREY, true);
    AddMeshToList(blueEnemy);
    Mesh* yellowEnemy = CreateEnemy("yellowEnemy", corner, 50, 30, YELLOW, GREY, true);
    AddMeshToList(yellowEnemy);
    Mesh* purpleEnemy = CreateEnemy("purpleEnemy", corner, 50, 30, PURPLE, GREY, true);
    AddMeshToList(purpleEnemy);

}

void tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.2, 0.2, 0.2, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void tema1::Update(float deltaTimeSeconds)
{
    // barrier
    angularStep = angularStep - 2 * deltaTimeSeconds;
    modelMatrix = glm::mat3(1);
    modelMatrix *= Translate(10, 30);
    modelMatrix *= Scale(0.8, 4.5);
    RenderMesh2D(meshes["barrier"], shaders["VertexColor"], modelMatrix);
    int k = 0;
    
    for (int i = 120; i <= 420; i += 150)
    {
        for (int j = 30; j <= 360; j += 165)
        {
            plots[k].x = i;
            plots[k].y = j;
            
            modelMatrix = glm::mat3(1);
            modelMatrix *= Translate(i, j);
            modelMatrix *= Scale(1.2 , 1.2);
            RenderMesh2D(meshes["plot"], shaders["VertexColor"], modelMatrix);
            if (plots[k].isEmpty == 0) {
                modelMatrix = glm::mat3(1);
                modelMatrix *= Translate(plots[k].x + 30, plots[k].y + 60);
                
                if (plots[k].color == RED) {
                    RenderMesh2D(meshes["redPlant"], shaders["VertexColor"], modelMatrix);
                    if (isEnemyOfColorOnLine(RED, plots[k].y + 60)) {
                        shoot(RED, plots[k].x + 90, plots[k].y + 60, k);
                    }
                }
                if (plots[k].color == BLUE) {
                    RenderMesh2D(meshes["bluePlant"], shaders["VertexColor"], modelMatrix);
                    if (isEnemyOfColorOnLine(BLUE, plots[k].y + 60)) {
                        shoot(BLUE, plots[k].x + 90, plots[k].y + 60, k);
                    }

                }
                if (plots[k].color == YELLOW) {
                    RenderMesh2D(meshes["yellowPlant"], shaders["VertexColor"], modelMatrix);
                    if (isEnemyOfColorOnLine(YELLOW, plots[k].y + 60)) {
                        shoot(YELLOW, plots[k].x + 90, plots[k].y + 60, k);
                    }
                }
                if (plots[k].color == PURPLE) {
                    RenderMesh2D(meshes["purplePlant"], shaders["VertexColor"], modelMatrix);
                    if (isEnemyOfColorOnLine(PURPLE, plots[k].y + 60)) {
                        shoot(PURPLE, plots[k].x + 90, plots[k].y + 60, k);
                    }
                }
            }
            if (plots[k].isEmpty == 1) {

                if (plots[k].scale > 0) {
                    modelMatrix = glm::mat3(1);
                    plots[k].scale = plots[k].scale - deltaTimeSeconds / 1.5;
                    modelMatrix *= Translate(plots[k].x + 30, plots[k].y + 60);
                    modelMatrix *= Translate(40 ,0);
                    modelMatrix *= Scale (plots[k].scale, plots[k].scale);
                    modelMatrix *= Translate(-40, 0);
                    if (plots[k].color == RED)
                        RenderMesh2D(meshes["redPlant"], shaders["VertexColor"], modelMatrix);
                    if(plots[k].color == BLUE)
                        RenderMesh2D(meshes["bluePlant"], shaders["VertexColor"], modelMatrix);
                    if(plots[k].color == YELLOW)
                        RenderMesh2D(meshes["yellowPlant"], shaders["VertexColor"], modelMatrix);
                    if (plots[k].color == PURPLE)
                        RenderMesh2D(meshes["purplePlant"], shaders["VertexColor"], modelMatrix);
                }
            }
            k++;
        }
    }
    //red turret
    modelMatrix = glm::mat3(1);
    modelMatrix *= Translate(80, 610);
    RenderMesh2D(meshes["redPlant"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= Translate(60, 520);
    modelMatrix *= Scale(0.5, 0.5);
    RenderMesh2D(meshes["greyStar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= Translate(50, 550);
    modelMatrix *= Scale(1.2, 1.2);
    RenderMesh2D(meshes["frame"], shaders["VertexColor"], modelMatrix);
    // blue turret
    modelMatrix = glm::mat3(1);
    modelMatrix *= Translate(230, 610);
    RenderMesh2D(meshes["bluePlant"], shaders["VertexColor"], modelMatrix);

    for (int i = 210; i <= 250; i += 40)
    {
        modelMatrix = glm::mat3(1);
        modelMatrix *= Translate(i, 520);
        modelMatrix *= Scale(0.5, 0.5);
        RenderMesh2D(meshes["greyStar"], shaders["VertexColor"], modelMatrix);
    }

    modelMatrix = glm::mat3(1);
    modelMatrix *= Translate(200, 550);
    modelMatrix *= Scale(1.2, 1.2);
    RenderMesh2D(meshes["frame"], shaders["VertexColor"], modelMatrix);
    // yellow turret
    modelMatrix = glm::mat3(1);
    modelMatrix *= Translate(380, 610);
    RenderMesh2D(meshes["yellowPlant"], shaders["VertexColor"], modelMatrix);

    for (int i = 360; i <= 400; i += 40)
    {
        modelMatrix = glm::mat3(1);
        modelMatrix *= Translate(i, 520);
        modelMatrix *= Scale(0.5, 0.5);
        RenderMesh2D(meshes["greyStar"], shaders["VertexColor"], modelMatrix);
    }

    modelMatrix = glm::mat3(1);
    modelMatrix *= Translate(350, 550);
    modelMatrix *= Scale(1.2, 1.2);
    RenderMesh2D(meshes["frame"], shaders["VertexColor"], modelMatrix);
    // purple turret
    modelMatrix = glm::mat3(1);
    modelMatrix *= Translate(530, 610);
    RenderMesh2D(meshes["purplePlant"], shaders["VertexColor"], modelMatrix);

    for (int i = 510; i <= 590; i+=40)
    {
        modelMatrix = glm::mat3(1);
        modelMatrix *= Translate(i, 520);
        modelMatrix *= Scale(0.5, 0.5);
        RenderMesh2D(meshes["greyStar"], shaders["VertexColor"], modelMatrix);
    }

    modelMatrix = glm::mat3(1);
    modelMatrix *= Translate(500, 550);
    modelMatrix *= Scale(1.2, 1.2);
    RenderMesh2D(meshes["frame"], shaders["VertexColor"], modelMatrix);

    int offset = 0;
    for (int i = 0; i < health; i++) {
        offset += 100;
        modelMatrix = glm::mat3(1);
        modelMatrix *= Translate(600 + offset, 580);
        modelMatrix *= Scale(0.8, 0.8);
        RenderMesh2D(meshes["barrier"], shaders["VertexColor"], modelMatrix);
    }
    offset = 0;
    for (int i = 0; i < money; i++)
    {
        offset += 40;
        modelMatrix = glm::mat3(1);
        
        modelMatrix *= Translate(680 + offset, 550);
        modelMatrix *= Scale(0.5, 0.5);
        RenderMesh2D(meshes["greyStar"], shaders["VertexColor"], modelMatrix);
    }

    modelMatrix = glm::mat3(1);
    modelMatrix *= Translate(window->GetCursorPosition().x, 720 - window->GetCursorPosition().y);

    if(holdRed == 1)
        RenderMesh2D(meshes["redPlant"], shaders["VertexColor"], modelMatrix);
    if (holdBlue == 1)
        RenderMesh2D(meshes["bluePlant"], shaders["VertexColor"], modelMatrix);
    if (holdYellow == 1) 
        RenderMesh2D(meshes["yellowPlant"], shaders["VertexColor"], modelMatrix);
    if (holdPurple == 1)
        RenderMesh2D(meshes["purplePlant"], shaders["VertexColor"], modelMatrix);
    
    if (glfwGetTime() >= spawnTime) {
        spawnTime += 3;
        int spawn = rand() % 2;
        if (spawn == 1) {
            int level = rand() % 3;
            Enemy enemy;
            int color = rand() % 4;
            if (color == 0)
                enemy.color = RED;
            if (color == 1)
                enemy.color = BLUE;
            if (color == 2)
                enemy.color = YELLOW;
            if (color == 3)
                enemy.color = PURPLE;
            if (level == 0)
                enemy.y = 90;
            if (level == 1)
                enemy.y = 255;
            if (level == 2)
                enemy.y = 420;
            enemies.push_back(enemy);
        }
    }
    if (glfwGetTime() >= moneyTime) {
        moneyTime += 5;
        int spawnMoney = rand() % 3;
        for (int i = 0; i < spawnMoney; i++)
        {
            int x = rand() % 1280;
            int y = rand() % 720;

            spawnedMoney.push_back(glm::vec2(x, y));
        }
    }
    for (int i = 0; i < enemies.size(); i++)
    {
        modelMatrix = glm::mat3(1);
        enemies[i].x -= deltaTimeSeconds * speed;
        modelMatrix *= Translate(enemies[i].x, enemies[i].y);
        modelMatrix *= Scale(enemies[i].scale, enemies[i].scale);
        if (enemies[i].color == RED)
            RenderMesh2D(meshes["redEnemy"], shaders["VertexColor"], modelMatrix);
        else if (enemies[i].color == BLUE)
            RenderMesh2D(meshes["blueEnemy"], shaders["VertexColor"], modelMatrix);
        else if (enemies[i].color == YELLOW)
            RenderMesh2D(meshes["yellowEnemy"], shaders["VertexColor"], modelMatrix);
        else if (enemies[i].color == PURPLE)
            RenderMesh2D(meshes["purpleEnemy"], shaders["VertexColor"], modelMatrix);
        for (int j = 0; j < bullets.size(); j++)
        {
            if (collide(enemies[i].x, enemies[i].y, 50, bullets[j].x, bullets[j].y, 40) && bullets[j].color == enemies[i].color) {
                bullets[j].isAlive = 0;
                enemies[i].health--;
                if (enemies[i].health <= 0)
                    enemies[i].isAlive = 0;
            }
        }
        for (int j = 0; j < 9; j++)
            if (collide(enemies[i].x, enemies[i].y, 50, plots[j].x + 60, plots[j].y + 60, 60) && plots[j].isEmpty == 0) {
                plots[j].isEmpty = 1;
                plots[j].scale -= 0.01f;
            }
     
        if (enemies[i].x < 75) {
            enemies[i].isAlive = 0;
            health--;
        }
    }
    for (int i = 0; i < bullets.size(); i++)
        if(bullets[i].isAlive == 0)
            bullets.erase(bullets.begin() + i);
    for (int i = 0; i < enemies.size(); i++)
        if (enemies[i].isAlive == 0) {
            if (enemies[i].scale > 0) {
                enemies[i].scale = enemies[i].scale - deltaTimeSeconds / 1.5f;
                enemies[i].x += deltaTimeSeconds * speed;
            }
        if(enemies[i].scale <= 0)
            enemies.erase(enemies.begin() + i);
        }
    for (int i = 0; i < bullets.size(); i++)
    {
        modelMatrix = glm::mat3(1);
        
        bullets[i].x += deltaTimeSeconds * speed * 5;
        modelMatrix *= Translate(bullets[i].x, bullets[i].y);
        modelMatrix *= Rotate(angularStep);
        RenderMesh2D(meshes["greyStar"], shaders["VertexColor"], modelMatrix);
    }
    for (int i = 0; i < spawnedMoney.size(); i++)
    {
        modelMatrix = glm::mat3(1);
        modelMatrix *= Translate(spawnedMoney[i].x, spawnedMoney[i].y);
        RenderMesh2D(meshes["greyStar"], shaders["VertexColor"], modelMatrix);
    }
}
void tema1::FrameEnd()
{
}

void tema1::OnInputUpdate(float deltaTime, int mods)
{

}


void tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    mouseY = 720 - mouseY;
    if (window->MouseHold(GLFW_MOUSE_BUTTON_1)) {
        for (int i = 0; i < spawnedMoney.size(); i++)
        {
            if (mouseX >= spawnedMoney[i].x - 25 && mouseX < spawnedMoney[i].x + 25 &&
                mouseY >= spawnedMoney[i].y - 25 && mouseY < spawnedMoney[i].y + 25) {
                spawnedMoney.erase(spawnedMoney.begin() + i);
                money++;
                return;
            }
        }
        holdBlue = holdRed = holdPurple = holdYellow = 0;
        if (mouseX >= trRed.x && mouseX < trRed.x + 120 &&
            mouseY >= trRed.y && mouseY < trRed.y + 120) {
            if (money >= 1)
                holdRed = 1;
        }
        if (mouseX >= trBlue.x && mouseX < trBlue.x + 120 &&
            mouseY >= trBlue.y && mouseY < trBlue.y + 120) {
            if (money >= 2)
                holdBlue = 1;
        }
        if (mouseX >= trYellow.x && mouseX < trYellow.x + 120 &&
            mouseY >= trYellow.y && mouseY < trYellow.y + 120) {
            if (money >= 2)
                holdYellow = 1;
        }
        if (mouseX >= trPurple.x && mouseX < trPurple.x + 120 &&
            mouseY >= trPurple.y && mouseY < trPurple.y + 120) {
            if (money >= 3)
                holdPurple = 1;
        }
    }
    if (window->MouseHold(GLFW_MOUSE_BUTTON_2))
    {
        Plot* plot = getPlotFromPos(mouseX, mouseY);
        if (plot != NULL) {
            plot->isEmpty = 1;
            plot->scale -= 0.01f;
        }
    }

}
void tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
    if (!window->MouseHold(GLFW_MOUSE_BUTTON_1)) {
        
        Plot* placePlot = getPlotFromPos(mouseX, 720 - mouseY);
        if (placePlot == NULL || placePlot->isEmpty == 0){
            holdBlue = holdRed = holdPurple = holdYellow = 0;
            return;
        }
        if (placePlot->isEmpty == 1) {
            if (holdRed == 1 && money >= 1) {
                placePlot->isEmpty = 0;
                money--;
                placePlot->color = RED;
                placePlot->scale = 1;
                holdRed = 0;
            }
            if (holdBlue == 1 && money >= 2) {
                placePlot->isEmpty = 0;
                money -= 2;
                placePlot->color = BLUE;
                placePlot->scale = 1;
                holdBlue = 0;
            }
            if (holdYellow == 1 && money >= 2) {
                placePlot->isEmpty = 0;
                money -= 2;
                placePlot->color = YELLOW;
                placePlot->scale = 1;
                holdYellow = 0;
            }
            if (holdPurple == 1 && money >= 3) {
                placePlot->isEmpty = 0;
                money -= 3;
                placePlot->color = PURPLE;
                placePlot->scale = 1;
                holdPurple = 0;
            }
        }
    }
}
void tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void tema1::OnWindowResize(int width, int height)
{
}
