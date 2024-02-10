#pragma once

#include "components/simple_scene.h"
#include "lab_m1/lab5/lab_camera.h"
#include "lab_m1/tema2/MovementStates.h"
#include <random>
#include <map>
#include <string>
using namespace std;

namespace m1
{
    class Tank;
    struct Bullet {
        glm::vec3 position;
        glm::vec3 forward;
        Tank *tank;
        glm::vec3 velocity;
        bool isDead = 0;
        Bullet(glm::vec3 position, glm::vec3 forward, glm::vec3 initialVelocity) {
            this->forward = forward;
            this->position = position ;
            velocity = initialVelocity;
        }
        void Bullet::UpdatePosition(float deltaTime) {
            velocity.y -= 10 * deltaTime;

            position += velocity * deltaTime;
            if (position.y <= 0) {
                isDead = 1;
            }
        }
    };
    struct BoundingSphere {
        glm::vec3 center;
        float radius;
    };
    struct Building {
        glm::vec3 position;
        glm::vec3 scaleFactor;
    };
    class tema2 : public gfxc::SimpleScene
    {
    public:
        tema2();
        ~tema2();

        void Init() override;
        
    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        
        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) override;
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void RenderWorld(float deltaTimeSeconds);
        void RenderTank(Tank *tank, float deltaTimeSeconds);
        void HandleTankCollisions();
        void HandleBulletTankCollisions();
        void GenerateBuildings(int count);
        void HandleBuildingsCollision();
        void GenerateEnemies(int count);

    public:
        
        implemented::Camera* camera;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;
        Tank* playerTank;
        Tank* renderingTank;
        std::vector<Tank*> tanks;
        std::vector<Building> buildings;
        std::unordered_map<std::string, glm::vec3> meshColors = {
            {"plane", glm::vec3(0, 1, 0.6f)},
            {"wheels", glm::vec3(0.35f, 0.35f, 0.35f)},
            {"base", glm::vec3(0.35f, 0.60f, 0.35f)},
            {"turret", glm::vec3(0.28f, 0.74f, 0.51f)},
            {"building", glm::vec3(0.65f, 0.65f, 0.65f)},
            {"barrel", glm::vec3(0.5, 0.5, 0.5)}
        };
        float stateTime = 0.f;

    };
    class Tank : public implemented::Camera
    {
        public:
            Tank()
            {
                position = glm::vec3(0, 0, 0);
                forward = glm::vec3(0, 0, -1);
                up = glm::vec3(0, 1, 0);
                right = glm::vec3(1, 0, 0);
                distanceToTarget = 0;

            }

            Tank(const glm::vec3& position, const glm::vec3& center, const glm::vec3& up)
            {
                this->position = position;
                forward = glm::normalize(center - position);
                right = glm::cross(forward, up);
                this->up = glm::cross(right, forward);
                distanceToTarget = 0;
            }

            ~Tank()
            { }
            void TranslateForward(float distance)
            {
                glm::vec3 forwardXZ = glm::normalize(glm::vec3(forward.x, 0.0f, forward.z));
                position += forwardXZ * distance;
            }
            void TranslateForwardCamera(implemented::Camera *camera,float distance)
            {
                glm::vec3 forwardXZ = glm::normalize(glm::vec3(forward.x, 0.0f, forward.z));
                position += forwardXZ * distance;
                
                camera->position += forwardXZ * distance;
            }
            void RotateFirstPerson_OY(float angle)
            {
                implemented::Camera::RotateFirstPerson_OY(angle);
                turretRotationY += angle;
            }
            void RotateFirstPerson_OX(float angle)
            {
                implemented::Camera::RotateFirstPerson_OX(angle);
                turretRotationX += angle;
            }
            void Shoot() {
                if (shootCooldown <= 0) {
                    glm::quat rotation = glm::angleAxis(turretRotationY, glm::vec3_up);

                    glm::vec3 barrelEndpoint = position + (glm::toMat3(rotation) * barrelOffset) + glm::vec3_up * turretRotationX * 4.0f;

                    glm::vec3 bulletDirection = glm::normalize(barrelEndpoint - position);
                    float bulletSpeed = 25.0f;

                    glm::vec3 bulletVelocity = bulletDirection * bulletSpeed;
                    bulletVelocity.y -= 2;
                
                    Bullet bullet(barrelEndpoint, forward, bulletVelocity);
                    bullet.tank = this;
                    bullets.push_back(bullet);
                    shootCooldown = 1;
                }
            }
        public:
            int health = 3;
            float speed = 10;
            float turretRotationY = 0;
            float turretRotationX = 0;
            float shootCooldown = 0;
            std::vector<Bullet> bullets;
            glm::vec3 barrelOffset = glm::vec3(0.0f, 1.0f, -4.0f);
            MovementState state = MovementState::GoingForward;
            bool isPlayer = false;
        
    };
    
}   // namespace m1
