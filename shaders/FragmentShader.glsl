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

// TODO(student): Declare any other uniforms
uniform int is_spot;
uniform float cut_off_angle;

uniform vec3 object_color;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    vec3 N = normalize(world_normal);
	vec3 L = normalize(light_position - world_position);
	vec3 V = normalize(eye_position - world_position);
	vec3 H = normalize(L + V);
	vec3 R = normalize(reflect(L, world_normal));


    // TODO(student): Define ambient, diffuse and specular light components
    float emissive_light = material_ke;

    // 0.25 = intensitatea ambientala globala
    float ambient_light = material_kd * 0.25;

    float diffuse_light = material_kd * max (dot (world_normal, L), 0);

    float specular_light = 0;

    // It's important to distinguish between "reflection model" and
    // "shading method". In this shader, we are experimenting with the Phong
    // (1975) and Blinn-Phong (1977) reflection models, and we are using the
    // Phong (1975) shading method. Don't mix them up!
    if (diffuse_light > 0)
    {
        specular_light = material_ks * pow (max(dot(V, R), 0), material_shininess);
    }

    // TODO(student): If (and only if) the light is a spotlight, we need to do
    // some additional things.
    // TODO(student): Compute the total light. You can just add the components
    // together, but if you're feeling extra fancy, you can add individual
    // colors to the light components. To do that, pick some vec3 colors that
    // you like, and multiply them with the respective light components.
    float light = ambient_light;

    if (is_spot == 1) // spotLight
	{
		float cut_off, spot_light, spot_light_limit;
		cut_off = radians(cut_off_angle);
		spot_light = dot(-L, light_direction);
		spot_light_limit = cos(cut_off);
		
		if (spot_light > cos(cut_off))
		{	 
			float linear_att, light_att_factor;		
			linear_att = (spot_light - spot_light_limit) / (1.f - spot_light_limit);
			light_att_factor = linear_att * linear_att;
			light = ambient_light + light_att_factor * (diffuse_light + specular_light);
		} 

	} else if (is_spot == 0) {
        float d	= distance (light_position, world_position);
        float attenuation_factor = 1 / (d * d);
	    light = ambient_light + attenuation_factor * (diffuse_light + specular_light);
	}


    // TODO(student): Write pixel out color
    vec3 color = object_color * light;
    out_color = vec4(color, 1);

}
