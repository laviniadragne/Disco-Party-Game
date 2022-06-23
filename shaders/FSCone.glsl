#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;

uniform float material_ke;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform int is_spot;
uniform float cut_off_angle;
uniform vec3 lightConePos[4];
uniform vec3 lightConeColor[4];

uniform vec3 object_color;

// Output
layout(location = 0) out vec4 out_color;

void main()
{

    float light = material_ke;

    // TODO(student): Write pixel out color
    vec3 color = object_color * light;
    out_color = vec4(color, 0.5f);

}

