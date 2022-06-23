#pragma once

#include <Windows.h>
#include "components/simple_scene.h"
#include "components/transform.h"
#include "Cell.cpp"
#include "Dancer.cpp"

#define RED glm::vec3(1, 0, 0)
#define FLOOR_X -3.5
#define FLOOR_Y 0
#define FLOOR_Z -3.5
#define LEN_FLOOR 8
#define LEN_CELL 1
#define NUM_DANCERS 6
#define MIN_SPEED_DANCER 1
#define MAX_SPEED_DANCER 3
#define LIM_SUP 3.85
#define LIM_INF -3.85
#define TOP_LIGHT 0.5
#define H_CONE 1
#define NUM_CONES 4
#define MAX_RAD 20
#define MIN_RAD -20
#define RESOLUTION 16

namespace m1
{
    class Tema3 : public gfxc::SimpleScene
    {
     public:
        Tema3();
        ~Tema3();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        Texture2D* CreateRandomTexture(unsigned int width, unsigned int height);
        void GenerateLightConePosition();
        void GenerateLightPosition();
        void DrawRoof();
        void GenerateFloor();
        void DrawWalls();
        void DrawFloor();
        void GenerateWalls();
        void DrawInitDancers();
        void DrawGlobe();
        Mesh* CreateCone(const std::string& name, glm::vec3 leftBottomCorner,
                                float radius, glm::vec3 color, bool fill);
        void DrawCones(float deltaTimeSeconds);
        void RenderRoofMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture);
        void RenderGlobeMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture);
        void RenderFloorMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture, const glm::vec3& color);
        void RenderDancerMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture, const glm::vec3& color);
        void UpdatePosFloorLights(glm::vec3 pos_target, float lim_radius);
        void DrawDancers(float deltaTimeSeconds);
        void MoveDancers(float deltaTimeSeconds);
        void GenerateDancers();
        void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, glm::vec3 lightPosition, const glm::vec3 &color = glm::vec3(1));
        void RenderConeMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::vec3 lightPosition, const glm::vec3& color);
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        glm::vec3 lightPosition;
        glm::vec3 lightDirection;
        unsigned int materialShininess;
        float materialKe;
        float materialKd;
        float materialKs;

        // TODO(student): If you need any other class variables, define them here.
        int isSpotlight;
        int isFloorLight;
        int isGlobeLight;
        float cut_off_Angle;
        float angle_ox, angle_oy, angle_oz;
        float light_radius_globe;

        // vectorul de culori ale celulelor
        std::unordered_map<std::string, Texture2D*> mapTextures;
        vector<Cell*> cells;
        vector<Cell*> walls;
        vector<glm::vec3> lightPositions;
        vector<glm::vec3> colors;
        vector<Dancer*> dancers;
        glm::vec3 pointLightPos[9];
        glm::vec3 pointLightColor[9];
        glm::vec3 lightConePos[4];
        glm::vec3 lightConeColor[4];
        glm::vec3 lightDirections[4];
        glm::vec3 disco_ball_position;

    };
}   // namespace m1
