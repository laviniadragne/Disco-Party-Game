#pragma once
#include "components/simple_scene.h"
#include "components/transform.h"

using namespace std;

class Dancer
{
public:
    float index_x;
    float index_z;
    float speed;

    Dancer() {
        index_x = 0;
        index_z = 0;
        speed = 0;
    }
    Dancer(float index_x, float index_z, float speed) {
        this->index_x = index_x;
        this->index_z = index_z;
        this->speed = speed;
    }

};