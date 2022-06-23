#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

uniform float Time;

// Uniform properties
uniform sampler2D textur;

uniform vec3 eye_position;

uniform float material_ke;

uniform int is_spot;
uniform int is_floor;
uniform int is_globe;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;
uniform float cut_off_angle;

uniform vec3 light_directions[4];
uniform vec3 lightConePos[4];
uniform vec3 lightConeColor[4];

uniform vec3 disco_ball_position;

uniform vec3 object_color;

// Output
layout(location = 0) out vec4 out_color;


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
   
    float emissive_light = material_ke;
    vec3 color = vec3(0, 0, 0);

    if (is_floor == 1) {
        float light = emissive_light;
        // TODO(student): Write pixel out color
        color = object_color * light;
        // out_color = vec4(color, 1);
    }

     vec3 light_spot = vec3(0.f, 0.f, 0.f);
     vec3 light_globe = vec3(0.f, 0.f, 0.f);

     if (is_spot == 1) {
        vec3 N = normalize(world_normal);
        float light_radius = 4;

        for (int i = 0; i < 4; i++) {

            vec3 light_position = lightConePos[i];
            vec3 L = normalize(light_position - world_position);
	        vec3 V = normalize(eye_position - world_position);
	        vec3 H = normalize(L + V);
	        vec3 R = normalize(reflect(L, world_normal));

            float diffuse_light = material_kd * max (dot (world_normal, L), 0);

            float specular_light = 0;

            if (diffuse_light > 0)
            {
                specular_light = material_ks * pow (max(dot(V, R), 0), material_shininess);
            }

            float cut_off, spot_light, spot_light_limit;

		    cut_off = radians(cut_off_angle);
		    spot_light = dot(-L, light_directions[i]);
		    spot_light_limit = cos(cut_off);
		
		    if (spot_light > cos(cut_off))
		    {	 
                float d	= distance (lightConePos[i], world_position);
                if (d < light_radius) 
                {
			        float linear_att, light_att_factor;		
			        linear_att = (spot_light - spot_light_limit) / (1.f - spot_light_limit);
			        light_att_factor = pow(linear_att, 2);

			        light_spot += 4 * lightConeColor[i] * light_att_factor * (diffuse_light + specular_light);
		        }    
            }
        }
     }
    
    vec3 color_globe = vec3(1);

    if (is_globe == 1) {

        // light_dir  este directia de iluminare
        vec3 light_dir = world_position - disco_ball_position;

        // texcoord este coordonata de textura utilizata pentru esantionare
        vec2 texcoord;
        texcoord.x = (1.0 / (2 * 3.14159)) * atan (light_dir.x, light_dir.z) - Time / 10;
        texcoord.y = (1.0 / 3.14159) * acos (light_dir.y / length (light_dir));
 
        // color este culoarea corespunzatoare pozitiei world_position
        color_globe = texture2D(textur, texcoord).xyz;

        float light_radius = 6;

        float attenuation_factor = 0.0f;

        float d	= distance (disco_ball_position, world_position);

        if (d < light_radius) {
            attenuation_factor = pow(light_radius - d, 2);
        }

        light_globe +=  0.4 * color_globe * (attenuation_factor * PointLightContribution(disco_ball_position));        
    }
     out_color = vec4(color + light_spot + light_globe, 1); 
   
}
