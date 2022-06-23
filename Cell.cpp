#pragma once
#include "components/simple_scene.h"
#include "components/transform.h"

using namespace std;

class Cell
{
public:
    float index_x;
    float index_y;
    glm::vec3 color;

    Cell() {
        index_x = 0;
        index_y = 0;
        color = glm::vec3(0, 0, 0);
    }
    Cell(float index_x, float index_y, float r, float g, float b) {
        this->index_x = index_x;
        this->index_y = index_y;
        this->color = glm::vec3(r, g, b);
    }

};