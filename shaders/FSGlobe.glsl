#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

uniform float Time;

// Uniform properties
uniform sampler2D texture;

uniform vec3 eye_position;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;


uniform int is_spot;
uniform int is_floor;
uniform int is_globe;

uniform vec3 disco_ball_position;

uniform vec3 object_color;

// Output
layout(location = 0) out vec3 out_color;


float PointLightContribution(vec3 lightPos)
{
    vec3 N = normalize(world_normal);
	vec3 V = normalize(eye_position - world_position);
	//calculele componentelor difuze si speculare din modelul Phong de iluminare pentru lumina punctiforma
    vec3 L = normalize(lightPos - world_position);
    vec3 H = normalize(L + V);

    float diffuse_light = material_kd * max (dot (N, L), 0);

    float specular_light = 0;

    if (diffuse_light > 0)
    {
        specular_light = material_ks * pow (max(dot(N, H), 0), material_shininess);
    }

	return (diffuse_light + specular_light);
}

void main()
{
    if (is_globe == 1) {

        // light_dir  este directia de iluminare
        vec3 light_dir = world_position - disco_ball_position;

        // texcoord este coordonata de textura utilizata pentru esantionare
        vec2 texcoord;
        texcoord.x = (1.0 / (2 * 3.14159)) * atan (light_dir.x, light_dir.z) - Time / 10;
        texcoord.y = (1.0 / 3.14159) * acos (light_dir.y / length (light_dir));
 
        // color este culoarea corespunzatoare pozitiei world_position
        vec3 color = texture2D(texture, texcoord).xyz;
        out_color = color;

    
        float light_radius = 5;
        vec3 light_globe = vec3(0.f, 0.f, 0.f);
       
        float attenuation_factor = 0.0f;

        float d	= distance (disco_ball_position, world_position);

        if (d < light_radius) {
            attenuation_factor = pow(light_radius - d, 2);
        }

        light_globe += color * (attenuation_factor * PointLightContribution(disco_ball_position));
        out_color = vec3(light_globe + color); 
    
    }
   
}
