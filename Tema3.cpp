#include "lab_m1/Tema3/Tema3.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema3::Tema3()
{
}


Tema3::~Tema3()
{
}


void Tema3::Init()
{
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = CreateCone("cone", glm::vec3(0, 1, 0), 1, glm::vec3(0, 0, 0), 0);
        meshes[mesh->GetMeshID()] = mesh;
    }


    // Create a shader program for drawing face polygon with the color of the normal
    {
        Shader *shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
    {
        Shader* shader = new Shader("FloorShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "FSFloor.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
    {
        Shader* shader = new Shader("DancerShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "FSDancer.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
    {
        Shader* shader = new Shader("ConeShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "FSCone.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
    {
        Shader* shader = new Shader("GlobeShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "FSGlobe.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
    {
        Shader* shader = new Shader("RoofShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "FSRoof.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        mapTextures["random"] = CreateRandomTexture(RESOLUTION, RESOLUTION);
    }

    // Adauga muzica
    PlaySound(TEXT("Melody.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

    // Light & material properties
    {
        GenerateLightPosition();
        lightPosition = glm::vec3(0, 1, 1);
        GenerateLightConePosition();
        lightDirection = glm::vec3(0, -1, 0);
        materialShininess = 30;
        materialKd = 0.5;
        materialKs = 0.5;
        materialKe = 0.6;
    }

    // initial e activata doar lumina din podea
    isSpotlight = 0;
    isFloorLight = 1;
    isGlobeLight = 0;
    cut_off_Angle = 45;
    angle_ox = 0;
    angle_oy = 0;
    angle_oz = 0;

    //GenerateFloor();
    GenerateDancers();
    DrawInitDancers(); 
    GenerateWalls();
    
}


void Tema3::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

Texture2D* Tema3::CreateRandomTexture(unsigned int width, unsigned int height)
{
    GLuint textureID = 0;
    unsigned int channels = 3;
    unsigned int size = width * height * channels;
    unsigned char* data = new unsigned char[size];

    srand(time(0));

    // TODO(student): Generate random texture data
    for (int i = 0; i < size; i++) {
        unsigned char ran_num = rand();
        data[i] = ran_num;
    }

    // TODO(student): Generate and bind the new texture ID
    glGenTextures(1, &textureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);


    if (GLEW_EXT_texture_filter_anisotropic) {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
    }

    // TODO(student): Set the texture parameters (MIN_FILTER, MAG_FILTER and WRAPPING MODE) using glTexParameteri
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);



    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    CheckOpenGLError();

    // Use glTexImage2D to set the texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    // TODO(student): Generate texture mip-maps
    glGenerateMipmap(GL_TEXTURE_2D);

    CheckOpenGLError();

    // Save the texture into a wrapper Texture2D class for using easier later during rendering phase
    Texture2D* texture = new Texture2D();
    texture->Init(textureID, width, height, channels);

    SAFE_FREE_ARRAY(data);
    return texture;
}


// Genereaza vectorii de pozitie pentru luminile conurilor
void Tema3::GenerateLightConePosition() {
    lightConePos[0] = glm::vec3(-2, TOP_LIGHT, -2);
    lightConePos[1] = glm::vec3(-2, TOP_LIGHT, 2);
    lightConePos[2] = glm::vec3(2, TOP_LIGHT, -2);
    lightConePos[3] = glm::vec3(2, TOP_LIGHT, 2);
    for (int i = 0; i < 4; i++) {
        lightDirections[i] = glm::vec3(0, -1, 0);
    }
}

// Generez vectorul de lumini pentru podea
void Tema3::GenerateLightPosition() {
    for (int i = 0; i < LEN_FLOOR; i++) {
        for (int j = 0; j < LEN_FLOOR; j++) {
            // desenez celula
            float x, y, z;
            x = (FLOOR_X + i);
            y = FLOOR_Y;
            z = (FLOOR_Z + j);
            lightPositions.push_back(glm::vec3(x, y, z));
        }
    }
}

void Tema3::GenerateFloor() {
    srand(static_cast <unsigned> (time(0)));
    for (int i = 0; i < LEN_FLOOR; i++) {
        for (int j = 0; j < LEN_FLOOR; j++) {
            // generez culoarea random
            float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
            float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
            float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
            Cell* new_cell = new Cell(i, j, r, g, b);
            cells.push_back(new_cell);
            colors.push_back(glm::vec3(r, g, b));
        }
    }
}

void Tema3::DrawFloor() {
    int cnt = 0;
    for (int i = 0; i < LEN_FLOOR; i++) {
        for (int j = 0; j < LEN_FLOOR; j++) {
            // desenez celula
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(FLOOR_X + i, FLOOR_Y, FLOOR_Z + j));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 0.05, 1));
            RenderFloorMesh(meshes["box"], shaders["FloorShader"], modelMatrix, mapTextures["random"], cells[cnt]->color);
            cnt++;
        }
    }
    // deseneaza fundatia neagra
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -0.65, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(8, 1.2, 8));
    RenderFloorMesh(meshes["box"], shaders["LabShader"], modelMatrix, mapTextures["random"], glm::vec3(0));
}

void Tema3::GenerateWalls() {
    float i, j;
    i = -4.5;
    for (j = -3.5; j < 4; j++) {
    //  generez culoarea random
        Cell* new_cell = new Cell(i, j, 0, 0, 0);
        walls.push_back(new_cell);
    }

    j = -4.5;
    for (i = -3.5; i < 4; i++) {
        // generez culoarea random
        Cell* new_cell = new Cell(i, j, 0, 0, 0);
        walls.push_back(new_cell);
    }

    i = 4.5;
    for (j = -3.5; j < 4; j++) {
        // generez culoarea random
        Cell* new_cell = new Cell(i, j, 0, 0, 0);
        walls.push_back(new_cell);
    }
}

void Tema3::DrawWalls() {
    for (int i = 0; i < walls.size(); i++) {
        // desenez peretele
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(walls[i]->index_x, FLOOR_Y + 1, walls[i]->index_y));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 2, 1));
        light_radius_globe = 6;
        float lim_radius = 2;
        UpdatePosFloorLights(glm::vec3(walls[i]->index_x, FLOOR_Y + 1, walls[i]->index_y), lim_radius);
        RenderDancerMesh(meshes["box"], shaders["DancerShader"], modelMatrix, mapTextures["random"], glm::vec3(0, 0, 0));
    }
}

void Tema3::UpdatePosFloorLights(glm::vec3 pos_target, float lim_radius) {

    int cnt = 0;
    
    for (int i = 0; i < 64; i++) {
        // calculez distanta celula target
        glm::vec3 pos_ceil =  glm::vec3(cells[i]->index_x + FLOOR_X, FLOOR_Y, cells[i]->index_y + FLOOR_Z);

        float d = distance(pos_target, pos_ceil);
        if (d < lim_radius) {
            pointLightPos[cnt] = glm::vec3(cells[i]->index_x + FLOOR_X, FLOOR_Y, cells[i]->index_y + FLOOR_Z);
            pointLightColor[cnt] = cells[i]->color;
            cnt++;
        }
    }

    // pun 0 la final
    while (cnt < 9) {
        pointLightPos[cnt] = glm::vec3(0);
        pointLightColor[cnt] = glm::vec3(0);
        cnt++;
    }
}

void Tema3::GenerateDancers() {

    while (dancers.size() < NUM_DANCERS) {
        // generez random pozitiile si viteza de deplasare
        float x = 0 + (std::rand() % ((LEN_FLOOR - 1) - 0 + 1));
        float z = 0 + (std::rand() % ((LEN_FLOOR - 1) - 0 + 1));
        float speed = MIN_SPEED_DANCER + static_cast <float> (rand()) / (static_cast <float>
                    (RAND_MAX / (MAX_SPEED_DANCER - MIN_SPEED_DANCER)));

        int ok = 0;
        // parcurg sa vad daca nu exista deja pe pozitia aia un dansator
        for (int i = 0; i < dancers.size(); i++) {
            // verific sa nu am in acea pozitie deja un dansator
            if ((dancers[i]->index_x - FLOOR_X) == x && (dancers[i]->index_z - FLOOR_Z) == z) {
                ok = 1;
                break;
            }
        }

        if (ok == 0) {
            Dancer* new_dancer = new Dancer(x + FLOOR_X, z + FLOOR_Z, speed);
            dancers.push_back(new_dancer);
        }
    }
}

void Tema3::DrawInitDancers() {
    for (int i = 0; i < dancers.size(); i++) {
        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(dancers[i]->index_x, FLOOR_Y, dancers[i]->index_z));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3, 0.5, 0.1));
            light_radius_globe = 6;
            /*UpdatePosFloorLights(glm::vec3(dancers[i]->index_x, FLOOR_Y, dancers[i]->index_z));
            RenderDancerMesh(meshes["box"], shaders["DancerShader"], modelMatrix, mapTextures["random"], glm::vec3(0, 0, 0));*/
        }
    }
}

// Muta dansatorii
void Tema3::MoveDancers(float deltaTimeSeconds) {
    for (int i = 0; i < dancers.size(); i++) {
        // generez o directie random de deplasare
        // 1 : jos (z++), 2 : dreapta(x++), 3 : sus(z--), 4 : stanga(x--)
        // 5 : stanga-jos, 6 : dreapta-jos, 7 : stanga-sus, 8 : dreapta-sus
        int dir = 1 + (std::rand() % (8 - 1 + 1));

        if (dir == 1) {
            float new_z = dancers[i]->index_z + dancers[i]->speed * deltaTimeSeconds;
            // daca ma incadrez in chenar, actualizez
            if (new_z <= LIM_SUP) {
                dancers[i]->index_z = new_z;
            }
        }
        if (dir == 2) {
            float new_x = dancers[i]->index_x + dancers[i]->speed * deltaTimeSeconds;
            if (new_x <= LIM_SUP) {
                dancers[i]->index_x = new_x;
            }
        }
        if (dir == 3) {
            float new_z = dancers[i]->index_z - dancers[i]->speed * deltaTimeSeconds;

            if (new_z >= LIM_INF) {
                dancers[i]->index_z = new_z;
            }
        }
        if (dir == 4) {
            float new_x = dancers[i]->index_x - dancers[i]->speed * deltaTimeSeconds;
            if (new_x >= LIM_INF) {
                dancers[i]->index_x = new_x;
            }
        }
        if (dir == 5) {
            float new_x = dancers[i]->index_x - dancers[i]->speed * deltaTimeSeconds;
            float new_z = dancers[i]->index_z + dancers[i]->speed * deltaTimeSeconds;

            if (new_x >= LIM_INF && new_z <= LIM_SUP) {
                dancers[i]->index_x = new_x;
                dancers[i]->index_z = new_z;
            }
        }
        if (dir == 6) {
            float new_x = dancers[i]->index_x + dancers[i]->speed * deltaTimeSeconds;
            float new_z = dancers[i]->index_z + dancers[i]->speed * deltaTimeSeconds;

            if (new_x <= LIM_SUP && new_z <= LIM_SUP) {
                dancers[i]->index_x = new_x;
                dancers[i]->index_z = new_z;
            }
        }
        if (dir == 7) {
            float new_x = dancers[i]->index_x - dancers[i]->speed * deltaTimeSeconds;
            float new_z = dancers[i]->index_z - dancers[i]->speed * deltaTimeSeconds;

            if (new_x >= LIM_INF && new_z >= LIM_INF) {
                dancers[i]->index_x = new_x;
                dancers[i]->index_z = new_z;
            }
        }
        if (dir == 8) {
            float new_x = dancers[i]->index_x + dancers[i]->speed * deltaTimeSeconds;
            float new_z = dancers[i]->index_z - dancers[i]->speed * deltaTimeSeconds;

            if (new_x <= LIM_SUP && new_z >= LIM_INF) {
                dancers[i]->index_x = new_x;
                dancers[i]->index_z = new_z;
            }
        }
    }
}

// Deseneaza dansatorii
void Tema3::DrawDancers(float deltaTimeSeconds) {
    for (int i = 0; i < dancers.size(); i++) {
        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(dancers[i]->index_x, FLOOR_Y + 0.25, dancers[i]->index_z));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3, 0.5, 0.1));
            light_radius_globe = 6;
            float lim_radius = 1.5;
            UpdatePosFloorLights(glm::vec3(dancers[i]->index_x, FLOOR_Y + 0.25, dancers[i]->index_z), lim_radius);
            RenderDancerMesh(meshes["box"], shaders["DancerShader"], modelMatrix, mapTextures["random"], glm::vec3(0, 0, 0));
        }

    }
}

void Tema3::DrawCones(float deltaTimeSeconds) {
    // se vor desena doar fatetele fata
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);

    // aceasta directiva este folosita pentru nu se scrie in depth buffer
    glDepthMask(GL_FALSE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    float speed = 10;

    // Conurile de lumina
    for (int i = 0; i < NUM_CONES; i++) {
        // glm::vec3(tan(spot_angle), 1, tan(spot_angle))* inaltime
        // generez random o directie de rotatie
        // 1  + ox;
        // 2  - ox;
        // 3  + oy;
        // 4  - oy;
        // std::srand(static_cast<unsigned int>(std::time(nullptr)));
        int dir = 1 + (std::rand() % (6 - 1 + 1));

        if (dir == 1) {
            angle_ox += deltaTimeSeconds * speed;
            if (angle_ox > MAX_RAD) {
                angle_ox -= deltaTimeSeconds * speed;
            }
        }
        if (dir == 2) {
            angle_ox -= deltaTimeSeconds * speed;
            if (angle_ox < MIN_RAD) {
                angle_ox += deltaTimeSeconds * speed;
            }
        }
        if (dir == 3) {
            angle_oy += deltaTimeSeconds * speed;
            if (angle_oy > MAX_RAD) {
                angle_oy -= deltaTimeSeconds * speed;
            }
        }
        if (dir == 4) {
            angle_oy -= deltaTimeSeconds * speed;
            if (angle_oy < MIN_RAD) {
                angle_oy += deltaTimeSeconds * speed;
            }
        }
        if (dir == 5) {
            angle_oz += deltaTimeSeconds * speed;
            if (angle_oz > MAX_RAD) {
                angle_oz -= deltaTimeSeconds * speed;
            }
        }
        if (dir == 6) {
            angle_oz -= deltaTimeSeconds * speed;
            if (angle_oz < MIN_RAD) {
                angle_oz += deltaTimeSeconds * speed;
            }
        }

        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, lightConePos[i]);

        glm::mat4 rotate_matrix = glm::mat4(1.0f);
        rotate_matrix = glm::rotate(rotate_matrix, RADIANS(angle_ox), glm::vec3(1, 0, 0));
        rotate_matrix = glm::rotate(rotate_matrix, RADIANS(angle_oy), glm::vec3(0, 1, 0));
        rotate_matrix = glm::rotate(rotate_matrix, RADIANS(angle_oz), glm::vec3(0, 0, 1));

        modelMatrix = glm::rotate(modelMatrix, RADIANS(angle_ox), glm::vec3(1, 0, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(angle_oy), glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(angle_oz), glm::vec3(0, 0, 1));

        lightDirections[i] = glm::vec3(0, -1, 0);
        lightDirections[i] = glm::vec3(rotate_matrix * glm::vec4(lightDirections[i], 1));

        modelMatrix = glm::scale(modelMatrix, glm::vec3(tan(RADIANS(cut_off_Angle)), 1.5, tan(RADIANS(cut_off_Angle)) * H_CONE));
        RenderFloorMesh(meshes["cone"], shaders["ConeShader"], modelMatrix, mapTextures["random"], cells[i]->color);
    }
    
    // se dezactiveaza actiunile tuturor directivelor apelate anterior
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    //glDisable(GL_CULL_FACE);
}

void Tema3::DrawRoof() {
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 2, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(8, 0.05, 8));
    RenderRoofMesh(meshes["box"], shaders["RoofShader"], modelMatrix, mapTextures["random"]);
}

void Tema3::DrawGlobe() {
    glm::mat4 modelMatrix = glm::mat4(1);
    disco_ball_position = glm::vec3(0, 1.7, 0);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1.7, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5, 0.5, 0.5));
    RenderGlobeMesh(meshes["sphere"], shaders["GlobeShader"], modelMatrix, mapTextures["random"]);
}

void Tema3::Update(float deltaTimeSeconds)
{
  
    GenerateFloor();

    for (int i = 0; i < NUM_CONES; i++) {
        lightConeColor[i] = colors[i];
    }

    DrawFloor();
    MoveDancers(deltaTimeSeconds);

    DrawWalls();
    DrawRoof();

    if (isGlobeLight == 1) {
        DrawGlobe();
    }

    DrawDancers(deltaTimeSeconds);


    if (isSpotlight == 1) {
        DrawCones(deltaTimeSeconds);
    }

    cells.clear();
    colors.clear();
}


void Tema3::FrameEnd()
{
    DrawCoordinateSystem();
}

Mesh* Tema3::CreateCone(const std::string& name, glm::vec3 top,
                        float radius, glm::vec3 color, bool fill)
{
    int N = 50;
    float angle = 0;
    float step = 360.0f / N;
    vector<VertexFormat> vertices;


    // Centrul cercului
    vertices.push_back(VertexFormat(top, color, glm::vec3(0.0, 1.0, 0.0)));

    for (int i = 1; i <= N; i++)
    {
        float angle_rad = (angle * M_PI) / 180.0f;
        vertices.push_back(VertexFormat(glm::vec3(cos(angle_rad) * radius, -1, sin(angle_rad) * radius),
            color));
        angle += step;
    }

    vector<unsigned int> indices;
    indices.push_back(0);
    for (int i = 1; i <= N; i++)
    {
        indices.push_back(i);
    }

    indices.push_back(1);

    Mesh* new_square = new Mesh(name);
    new_square->SetDrawMode(GL_TRIANGLE_FAN);

    new_square->InitFromData(vertices, indices);

    return new_square;
}

void Tema3::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, glm::vec3 lightPosition, const glm::vec3 &color)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Set shader uniforms for light properties
    int light_position = glGetUniformLocation(shader->program, "light_position");
    glUniform3f(light_position, lightPosition.x, lightPosition.y, lightPosition.z);

    int light_direction = glGetUniformLocation(shader->program, "light_direction");
    glUniform3f(light_direction, lightDirection.x, lightDirection.y, lightDirection.z);

    // Set eye position (camera position) uniform
    glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
    int eye_position = glGetUniformLocation(shader->program, "eye_position");
    glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

    // Set material property uniforms (shininess, ke, kd, ks, object color) 
    int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(material_shininess, materialShininess);

    int material_ke = glGetUniformLocation(shader->program, "material_ke");
    glUniform1f(material_ke, materialKe);

    int material_kd = glGetUniformLocation(shader->program, "material_kd");
    glUniform1f(material_kd, materialKd);

    int material_ks = glGetUniformLocation(shader->program, "material_ks");
    glUniform1f(material_ks, materialKs);

    int object_color = glGetUniformLocation(shader->program, "object_color");
    glUniform3f(object_color, color.r, color.g, color.b);

    // TODO(student): Set any other shader uniforms that you need
    int is_spot = glGetUniformLocation(shader->program, "is_spot");
    glUniform1i(is_spot, isSpotlight);

    int cut_off_angle = glGetUniformLocation(shader->program, "cut_off_angle");
    glUniform1f(cut_off_angle, cut_off_Angle);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Tema3::RenderFloorMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture, const glm::vec3& color)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    GLuint light_cone = glGetUniformLocation(shader->program, "lightConePos");
    glUniform3fv(light_cone, 4, glm::value_ptr(lightConePos[0]));

    GLuint light_cone_color = glGetUniformLocation(shader->program, "lightConeColor");
    glUniform3fv(light_cone_color, 4, glm::value_ptr(lightConeColor[0]));

    int material_ke = glGetUniformLocation(shader->program, "material_ke");
    glUniform1f(material_ke, materialKe);

    glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
    int eye_position = glGetUniformLocation(shader->program, "eye_position");
    glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

    GLuint light_direction = glGetUniformLocation(shader->program, "light_directions");
    glUniform3fv(light_direction, 4, glm::value_ptr(lightDirections[0]));

    // Set material property uniforms (shininess, ke, kd, ks, object color) 
    int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(material_shininess, materialShininess / 2);

    int material_kd = glGetUniformLocation(shader->program, "material_kd");
    glUniform1f(material_kd, (float) materialKd / 2);

    int material_ks = glGetUniformLocation(shader->program, "material_ks");
    glUniform1f(material_ks, (float) materialKs / 2);

    int is_spot = glGetUniformLocation(shader->program, "is_spot");
    glUniform1i(is_spot, isSpotlight);

    int is_floor = glGetUniformLocation(shader->program, "is_floor");
    glUniform1i(is_floor, isFloorLight);

    int is_globe = glGetUniformLocation(shader->program, "is_globe");
    glUniform1i(is_globe, isGlobeLight);

    int cut_off_angle = glGetUniformLocation(shader->program, "cut_off_angle");
    glUniform1f(cut_off_angle, cut_off_Angle);

    int object_color = glGetUniformLocation(shader->program, "object_color");
    glUniform3f(object_color, color.r, color.g, color.b);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    int disco_ball_position1 = glGetUniformLocation(shader->program, "disco_ball_position");
    glUniform3f(disco_ball_position1, disco_ball_position.x, disco_ball_position.y, disco_ball_position.z);

    {
        int loc_time_matrix = glGetUniformLocation(shader->program, "Time");
        glUniform1f(loc_time_matrix, Engine::GetElapsedTime());
    }

    if (texture)
    {
        // TODO(student): Do these:
        // - activate texture location 0
        // - bind the texture1 ID
        // - send theuniform value
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture1"), 0);
    }

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


void Tema3::RenderDancerMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture, const glm::vec3& color)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Set shader uniforms for light properties
    GLuint light_pos = glGetUniformLocation(shader->program, "pointLightPos");
    glUniform3fv(light_pos, 9, glm::value_ptr(pointLightPos[0]));

    GLuint light_color = glGetUniformLocation(shader->program, "pointLightColor");
    glUniform3fv(light_color, 9, glm::value_ptr(pointLightColor[0]));

    GLuint light_cone = glGetUniformLocation(shader->program, "lightConePos");
    glUniform3fv(light_cone, 4, glm::value_ptr(lightConePos[0]));

    GLuint light_cone_color = glGetUniformLocation(shader->program, "lightConeColor");
    glUniform3fv(light_cone_color, 4, glm::value_ptr(lightConeColor[0]));

    // Set eye position (camera position) uniform
    glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
    int eye_position = glGetUniformLocation(shader->program, "eye_position");
    glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

    GLuint light_direction = glGetUniformLocation(shader->program, "light_directions");
    glUniform3fv(light_direction, 4, glm::value_ptr(lightDirections[0]));

    // Set material property uniforms (shininess, ke, kd, ks, object color) 
    int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(material_shininess, materialShininess);

    int material_kd = glGetUniformLocation(shader->program, "material_kd");
    glUniform1f(material_kd, materialKd);

    int material_ks = glGetUniformLocation(shader->program, "material_ks");
    glUniform1f(material_ks, materialKs);

    int light_radius_globe1 = glGetUniformLocation(shader->program, "light_radius_globe");
    glUniform1f(light_radius_globe1, light_radius_globe);

    int disco_ball_position1 = glGetUniformLocation(shader->program, "disco_ball_position");
    glUniform3f(disco_ball_position1, disco_ball_position.x, disco_ball_position.y, disco_ball_position.z);

    // TODO(student): Set any other shader uniforms that you need
    int is_spot = glGetUniformLocation(shader->program, "is_spot");
    glUniform1i(is_spot, isSpotlight);

    int is_floor = glGetUniformLocation(shader->program, "is_floor");
    glUniform1i(is_floor, isFloorLight);

    int is_globe = glGetUniformLocation(shader->program, "is_globe");
    glUniform1i(is_globe, isGlobeLight);

    int cut_off_angle = glGetUniformLocation(shader->program, "cut_off_angle");
    glUniform1f(cut_off_angle, cut_off_Angle);

    int object_color = glGetUniformLocation(shader->program, "object_color");
    glUniform3f(object_color, color.r, color.g, color.b);


    {
        int loc_time_matrix = glGetUniformLocation(shader->program, "Time");
        glUniform1f(loc_time_matrix, Engine::GetElapsedTime());
    }

    if (texture)
    {
        // TODO(student): Do these:
        // - activate texture location 0
        // - bind the texture1 ID
        // - send theuniform value
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "textur"), 0);
    }

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Tema3::RenderConeMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::vec3 lightPosition, const glm::vec3& color)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Set shader uniforms for light properties
    int light_position = glGetUniformLocation(shader->program, "light_position");
    glUniform3f(light_position, lightPosition.x, lightPosition.y, lightPosition.z);

    int light_direction = glGetUniformLocation(shader->program, "light_direction");
    glUniform3f(light_direction, lightDirection.x, lightDirection.y, lightDirection.z);

    // Set eye position (camera position) uniform
    glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
    int eye_position = glGetUniformLocation(shader->program, "eye_position");
    glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

    // Set material property uniforms (shininess, kd, ks, object color) 
    int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(material_shininess, materialShininess);

    int material_ke = glGetUniformLocation(shader->program, "material_ke");
    glUniform1f(material_ke, materialKe);

    int material_kd = glGetUniformLocation(shader->program, "material_kd");
    glUniform1f(material_kd, materialKd);

    int material_ks = glGetUniformLocation(shader->program, "material_ks");
    glUniform1f(material_ks, materialKs);

    int object_color = glGetUniformLocation(shader->program, "object_color");
    glUniform3f(object_color, color.r, color.g, color.b);

    // TODO(student): Set any other shader uniforms that you need
    int is_spot = glGetUniformLocation(shader->program, "is_spot");
    glUniform1i(is_spot, isSpotlight);

    int cut_off_angle = glGetUniformLocation(shader->program, "cut_off_angle");
    glUniform1f(cut_off_angle, cut_off_Angle);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Tema3::RenderGlobeMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Set eye position (camera position) uniform
    glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
    int eye_position = glGetUniformLocation(shader->program, "eye_position");
    glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

    // Set material property uniforms (shininess, kd, ks, object color) 
    int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(material_shininess, materialShininess);

    int material_kd = glGetUniformLocation(shader->program, "material_kd");
    glUniform1f(material_kd, materialKd);

    int material_ks = glGetUniformLocation(shader->program, "material_ks");
    glUniform1f(material_ks, materialKs);

    int is_spot = glGetUniformLocation(shader->program, "is_spot");
    glUniform1i(is_spot, isSpotlight);

    int is_floor = glGetUniformLocation(shader->program, "is_floor");
    glUniform1i(is_floor, isFloorLight);

    int disco_ball_position1 = glGetUniformLocation(shader->program, "disco_ball_position");
    glUniform3f(disco_ball_position1, disco_ball_position.x, disco_ball_position.y, disco_ball_position.z);

    int is_globe = glGetUniformLocation(shader->program, "is_globe");
    glUniform1i(is_globe, isGlobeLight);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    {
        int loc_time_matrix = glGetUniformLocation(shader->program, "Time");
        glUniform1f(loc_time_matrix, Engine::GetElapsedTime());
    }

    if (texture)
    {
        // TODO(student): Do these:
        // - activate texture location 0
        // - bind the texture1 ID
        // - send theuniform value
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture"), 0);
    }

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Tema3::RenderRoofMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
    int eye_position = glGetUniformLocation(shader->program, "eye_position");
    glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

    // Set material property uniforms (shininess, kd, ks, object color) 
    int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(material_shininess, (float) materialShininess / 2);

    int material_kd = glGetUniformLocation(shader->program, "material_kd");
    glUniform1f(material_kd, (float) materialKd / 2);

    int material_ks = glGetUniformLocation(shader->program, "material_ks");
    glUniform1f(material_ks, (float) materialKs / 2);

    int is_spot = glGetUniformLocation(shader->program, "is_spot");
    glUniform1i(is_spot, isSpotlight);

    int is_floor = glGetUniformLocation(shader->program, "is_floor");
    glUniform1i(is_floor, isFloorLight);

    int disco_ball_position1 = glGetUniformLocation(shader->program, "disco_ball_position");
    glUniform3f(disco_ball_position1, disco_ball_position.x, disco_ball_position.y, disco_ball_position.z);

    int is_globe = glGetUniformLocation(shader->program, "is_globe");
    glUniform1i(is_globe, isGlobeLight);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    {
        int loc_time_matrix = glGetUniformLocation(shader->program, "Time");
        glUniform1f(loc_time_matrix, Engine::GetElapsedTime());
    }

    if (texture)
    {
        // TODO(student): Do these:
        // - activate texture location 0
        // - bind the texture1 ID
        // - send theuniform value
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture1"), 0);
    }

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema3::OnInputUpdate(float deltaTime, int mods)
{
  
}


void Tema3::OnKeyPress(int key, int mods)
{
    // Add key press event

    // Cu C schimb spotlight
    if (key == GLFW_KEY_C)
    {
        if (isSpotlight == 0)
            isSpotlight = 1;
        else
            isSpotlight = 0;
    }

    // F controleaza lumina din podea
    if (key == GLFW_KEY_F)
    {
        if (isFloorLight == 0)
            isFloorLight = 1;
        else
            isFloorLight = 0;
    }

    if (key == GLFW_KEY_G)
    {
        if (isGlobeLight == 0)
            isGlobeLight = 1;
        else
            isGlobeLight = 0;
    }

}


void Tema3::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema3::OnWindowResize(int width, int height)
{
}
