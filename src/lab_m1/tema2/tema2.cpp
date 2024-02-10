#include "lab_m1/tema2/tema2.h"
#include "lab_m1/lab5/lab_camera.h"
#include "lab_m1/tema2/MovementStates.h"
#include <vector>
#include <iostream>

using namespace m1;
/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */

tema2::tema2()
{
    playerTank = new Tank();
    camera = new implemented::Camera();
    playerTank->position = glm::vec3(0, 1, 0);
    camera->position = glm::vec3(0, 4, 10);
    playerTank->isPlayer = true;
    tanks.push_back(playerTank);
}

tema2::~tema2() {}

void tema2::Init()
{
    Mesh* mesh = new Mesh("base");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "base.obj");
    meshes[mesh->GetMeshID()] = mesh;
    mesh = new Mesh("plane");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane.obj");
    meshes[mesh->GetMeshID()] = mesh;
    mesh = new Mesh("wheels");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "wheels.obj");
    meshes[mesh->GetMeshID()] = mesh;
    mesh = new Mesh("turret");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "turret.obj");
    meshes[mesh->GetMeshID()] = mesh;
    mesh = new Mesh("barrel");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "barrel.obj");
    meshes[mesh->GetMeshID()] = mesh;
    mesh = new Mesh("sphere");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
    meshes[mesh->GetMeshID()] = mesh;
    mesh = new Mesh("bullet");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
    meshes[mesh->GetMeshID()] = mesh;
    mesh = new Mesh("building");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
    meshes[mesh->GetMeshID()] = mesh;
    // + shader
    Shader* shader = new Shader("LabShader");
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "lab6", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "lab6", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    shaders[shader->GetName()] = shader;
    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
    
    GenerateBuildings(5);
    GenerateEnemies(10);
}

void tema2::GenerateBuildings(int count) 
{
    static std::random_device randomDevice;
    static std::default_random_engine randomEngine(randomDevice());

    std::uniform_int_distribution<int> uniformDistPos(-100, 100);
    std::uniform_int_distribution<int> uniformDistScale(20, 40);
    for (int i = 0; i < count; i++)
    {
        Building b;
        b.position.x = uniformDistPos(randomEngine);
        b.position.y = 0;
        b.position.z = uniformDistPos(randomEngine);
        b.scaleFactor.x = uniformDistScale(randomEngine);
        b.scaleFactor.y = uniformDistScale(randomEngine);
        b.scaleFactor.z = uniformDistScale(randomEngine);
        buildings.push_back(b);
    }
}

void tema2::GenerateEnemies(int count) {
    static std::random_device randomDevice;
    static std::default_random_engine randomEngine(randomDevice());
    std::uniform_int_distribution<int> uniformDistPos(-100, 100);
    for (int i = 0; i < count; i++)
    {
        Tank *tank = new Tank();
        tank->position.x = uniformDistPos(randomEngine);
        tank->position.y = 1;
        tank->position.z = uniformDistPos(randomEngine);
        tanks.push_back(tank);
    }
}

void tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.16, 0.55, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void tema2::Update(float deltaTimeSeconds)
{
    stateTime += deltaTimeSeconds;
    if (stateTime >= 2) {
        for (Tank* tank:tanks)
            tank->state = GetNextMovementState(tank->state);
        stateTime = 0.f;
    }
    for (Tank* tank:tanks)
    {
        if (tank != playerTank) {
            switch (tank->state)
            {
            case m1::MovementState::GoingForward:
                tank->TranslateForward(deltaTimeSeconds);
                break;
            case m1::MovementState::GoingBackward:
                tank->TranslateForward(-deltaTimeSeconds);
                break;
            case m1::MovementState::InPlaceRotationLeft:
                tank->RotateFirstPerson_OY(-deltaTimeSeconds);
                break;
            case m1::MovementState::InPlaceRotationRight:
                tank->RotateFirstPerson_OY(deltaTimeSeconds);
                break;
            default:
                break;
            }
            if (glm::distance(tank->position, playerTank->position) < 30)
                tank->Shoot();
        }
    }
    RenderWorld(deltaTimeSeconds);
    HandleBulletTankCollisions();
    HandleTankCollisions();
    HandleBuildingsCollision();
    for (int i = 0; i < tanks.size(); i++)
        if (tanks[i]->health <= 0) 
            tanks.erase(tanks.begin() + i);
    if (Engine::GetElapsedTime() >= 60 || playerTank->health <= 0)
        Exit();

}

void tema2::RenderWorld(float deltaTimeSeconds) 
{
    glm::mat4 modelMatrix = glm::mat4(1);
    RenderMesh(meshes["plane"], shaders["LabShader"], modelMatrix);
    for (auto tank: tanks)
    {
        RenderTank(tank, deltaTimeSeconds);
    }
    for (Building b:buildings)
    {
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, b.position);
        modelMatrix = glm::scale(modelMatrix, b.scaleFactor);
        RenderMesh(meshes["building"], shaders["LabShader"], modelMatrix);
    }
}

void tema2::FrameEnd()
{
  
}

void tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    GLuint colorLocation = glGetUniformLocation(shader->program, "object_color");

    GLuint gradeLocation = glGetUniformLocation(shader->program, "Grade");
    float grade = 0;
    glm::vec3 color = meshColors[mesh->GetMeshID()];
    if (mesh == meshes["base"]) {
        if (renderingTank->health == 2) {
            color -= 0.1f;
            grade = 0.2f;
        }
        if (renderingTank->health == 1) {
            color -= 0.1f;
            grade = 0.4f;
        }
    }

    glUniform3f(colorLocation, color.x, color.y, color.z);
    glUniform1f(gradeLocation, grade);

    mesh->Render();
}

void tema2::RenderTank(Tank *tank, float deltaTimeSeconds)
{
    for (int i = 0; i < tank->bullets.size(); i++) 
    {
        tank->bullets[i].UpdatePosition(deltaTimeSeconds);
        if (tank->bullets[i].isDead)
            tank->bullets.erase(tank->bullets.begin() + i);
    }
    renderingTank = tank;
    glm::mat4 modelMatrix = glm::mat4(1);
    RenderMesh(meshes["plane"], shaders["LabShader"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, tank->position - glm::vec3(0, 0.5f, 0));
    modelMatrix = glm::rotate(modelMatrix, tank->rotation, glm::vec3_up);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
    RenderMesh(meshes["wheels"], shaders["LabShader"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, tank->position);
    modelMatrix = glm::rotate(modelMatrix, tank->rotation, glm::vec3_up);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.7f));

    RenderMesh(meshes["base"], shaders["LabShader"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, tank->position + glm::vec3(0, 0.8f, 0));
    modelMatrix = glm::rotate(modelMatrix, tank->turretRotationY, glm::vec3_up);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.7f));
    RenderMesh(meshes["turret"], shaders["LabShader"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, tank->position + glm::vec3(0, 0.8f, 0));
    if (tank->turretRotationX < 0)
        tank->turretRotationX = 0;
    if (tank->turretRotationX > 0.4)
        tank->turretRotationX = 0.4;
    modelMatrix = glm::rotate(modelMatrix, tank->turretRotationY, glm::vec3_up);
    modelMatrix = glm::rotate(modelMatrix, tank->turretRotationX, glm::vec3_right);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.7f));
    RenderMesh(meshes["barrel"], shaders["LabShader"], modelMatrix);
    for (Bullet bullet: tank->bullets)
    {
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, bullet.position);

        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
        RenderMesh(meshes["bullet"], shaders["LabShader"], modelMatrix);

    }
    tank->shootCooldown -= deltaTimeSeconds;
}

bool CheckCollision(const BoundingSphere& sphere1, const BoundingSphere& sphere2) {
    float distance = glm::distance(sphere1.center, sphere2.center);
    float totalRadius = sphere1.radius + sphere2.radius;
    return distance < totalRadius;
}

bool BulletTankCollision(Bullet bullet, Tank tank) {

    BoundingSphere bulletSphere{ bullet.position, 0.1f};
    BoundingSphere tankSphere{ tank.position, 4.0f};

    return CheckCollision(bulletSphere, tankSphere);
}

void tema2::HandleBulletTankCollisions() {
    for (Tank* tank : tanks) {
        for (int i = 0; i < tank->bullets.size(); i++) {
            for (Tank* hitTank : tanks)
                if (hitTank != tank->bullets[i].tank) 
                    if (BulletTankCollision(tank->bullets[i], *hitTank)) {

                        tank->bullets[i].isDead = 1;
                        hitTank->health -= 1;
                        break;
                    }
            if (tank->bullets[i].isDead) 
                tank->bullets.erase(tank->bullets.begin() + i);
        }
    }
}

bool TankCollision(Tank tank1, Tank tank2) {

    BoundingSphere tank1Sphere{ tank1.position, 4.0f };
    BoundingSphere tank2Sphere{ tank2.position, 4.0f };
    return CheckCollision(tank1Sphere, tank2Sphere);
}

void tema2::HandleTankCollisions() {
    for (Tank* tank1 : tanks) {
        for (Tank* tank2 : tanks)
        {
            if(tank1 != tank2)
            if (TankCollision(*tank1, *tank2)) 
            {
                glm::vec3 dif = tank1->position - tank2->position;
                float P = 2 * 4 - glm::distance(tank1->position, tank2->position);
                if (tank1 == playerTank) {
                    tank1->TranslateForward(-P * 0.5f);
                    glm::vec3 forwardXZ = glm::normalize(glm::vec3(playerTank->forward.x, 0.0f, playerTank->forward.z));
                    camera->position += forwardXZ * -P * 0.5f;
                    tank2->TranslateForward(-P * 0.5f);
                }
            }
        }
    }
}

void tema2:: HandleBuildingsCollision()
{
    for (Building b : buildings)
    {
        
        for (Tank* tank : tanks)
        {
             
            if (tank->position.x <= b.position.x + b.scaleFactor.x / 2 && tank->position.x >= b.position.x - b.scaleFactor.x / 2
                && tank->position.z <= b.position.z + b.scaleFactor.z / 2 && tank->position.z >= b.position.z - b.scaleFactor.z / 2) {
                tank->TranslateForward(-0.5f);
                if (tank == playerTank) {
                    glm::vec3 forwardXZ = glm::normalize(glm::vec3(playerTank->forward.x, 0.0f, playerTank->forward.z));
                    camera->position += forwardXZ * -0.5f;
                }
            }
            for (int i = 0;i < tank->bullets.size();i++)
                if (tank->bullets[i].position.x <= b.position.x + b.scaleFactor.x / 2 && tank->bullets[i].position.x >= b.position.x - b.scaleFactor.x / 2
                    && tank->bullets[i].position.z <= b.position.z + b.scaleFactor.z / 2 && tank->bullets[i].position.z >= b.position.z - b.scaleFactor.z / 2)
                    tank->bullets.erase(tank->bullets.begin() + i);
            
                

        }
        
    }
}

void tema2::OnInputUpdate(float deltaTime, int mods)
{
    if (window->KeyHold(GLFW_KEY_W)) {
        playerTank->TranslateForward(deltaTime * playerTank->speed);
        glm::vec3 forwardXZ = glm::normalize(glm::vec3(playerTank->forward.x, 0.0f, playerTank->forward.z));
        camera->position += forwardXZ * deltaTime * playerTank->speed;
    }

    if (window->KeyHold(GLFW_KEY_A)) {
        camera->RotateThirdPerson_OY(deltaTime);
        playerTank->RotateFirstPerson_OY(deltaTime);
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        playerTank->TranslateForward(-deltaTime * playerTank->speed);
        glm::vec3 forwardXZ = glm::normalize(glm::vec3(playerTank->forward.x, 0.0f, playerTank->forward.z));
        camera->position += forwardXZ * -deltaTime * playerTank->speed;
    }
    if (window->KeyHold(GLFW_KEY_D)) {
        camera->RotateThirdPerson_OY(-deltaTime);
        playerTank->RotateFirstPerson_OY(-deltaTime);
    }
}

void tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    float sensitivity = 0.001f;
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
        float sensitivity = 0.001f;
        camera->RotateThirdPerson_OY(deltaX * sensitivity);
    }
    else
    {
        playerTank->turretRotationY -= deltaX * sensitivity;
        playerTank->turretRotationX -= deltaY * sensitivity;
    }
}

void tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT)) {
        
        playerTank->Shoot();
    }
}