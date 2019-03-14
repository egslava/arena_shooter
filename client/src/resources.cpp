#include <stdafx.h>
#include "resources.h"

const char *vertex_shader_code = ""
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aNormal;\n"
//"layout (location = 1) in vec3 aCol;\n"
"layout (location = 2) in vec2 aTex;\n"
"\n"
"uniform mat4 model;\n"
"uniform mat4 projection;\n"
"uniform mat4 camera;\n"
"out vec4 fragColor;\n"
"out vec2 fragTexCoord;\n"
"out vec3 w_position;\n"
"out vec3 w_normal;\n"
"out vec3 v_cam_pos;\n"
"\n"
"void main()\n"
"{\n"
"   v_cam_pos = (inverse(camera) * vec4(0,0,0,1)).xyz;\n"
//"	fragColor = vec4(aCol, 1);\n"
"	fragTexCoord = aTex;\n"
"    w_normal = (model * vec4(aNormal, 1.0)).xyz;\n"
"    w_position = (model * vec4(aPos.x, aPos.y, aPos.z, 1.0)).xyz;\n"
"    gl_Position = projection * camera * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"

"}   \n";

const char *fragment_shader_code = ""
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec4 fragColor;\n"
"in vec2 fragTexCoord;\n"
"in vec3 w_position;\n"
"in vec3 w_normal;\n"
"in vec3 v_cam_pos;\n"

"uniform vec4 mat_color;\n"
"uniform mat4 mat_model;\n"
"uniform mat4 camera;\n"
"uniform sampler2D texture0;\n"
"uniform vec4 ambient_color;\n"
"\n"
"void main()\n"
"{\n"
"   vec3 w_camera = (inverse(camera) * vec4(0,0,0,1)).xyz;\n"
"   vec3 w_pos = w_position;\n"
"   vec3 w_to_camera = normalize(w_camera - w_position);\n"
"   vec3 w_light = w_to_camera;\n"

"   float light_cam_bright = min(1, abs(dot(w_light, w_normal)) / pow(max(1, distance(w_pos, w_camera)), 1.01));\n"
//"   float light_cam_bright = min(1, abs(dot(w_normal, w_normal)) / pow(max(1, distance(w_pos, w_camera)), 2.0));\n"

//"   FragColor = vec4(1,1,1, 1)*light_cam_bright;\n"
"    vec4 tex_color = texture(texture0, fragTexCoord);\n"
"    vec4 out_color = vec4(0.5 * (light_cam_bright*mat_color).rgb + tex_color.rgb, tex_color.w) + ambient_color;\n"
"    out_color.a *= mat_color.a;\n"
"    FragColor = out_color ;\n"

//// reinhard tone mapping
//"    const float gamma = 5.2;\n"
//"    vec4 mapped = out_color / (out_color + vec4(1.0));\n"
////    // gamma correction
//"    mapped = pow(mapped, vec4(1.0 / gamma));\n"
//"    FragColor = vec4(mapped.rgb, out_color.a);\n"


//"    FragColor = texture(texture0, fragTexCoord);\n"
"}\n";


std::vector<float> _triangle_texcoords = {
    1.0f, 0.0f,  // lower-right corner
    0.0f, 0.0f,  // lower-left corner
    0.5f, 1.0f   // top-center corner
};

std::vector<float> _square_points {
    -0.5f,  -0.5f,  0.0f,
    -0.5f,   0.5f,  0.0f,
     0.5f,   0.5f,  0.0f,

    -0.5f,  -0.5f,  0.0f,
    0.5f,   0.5f,  0.0f,
    0.5f, -0.5f,  0.0f,
};

//std::vector<float> _square_points {
//    0.0f,    0.0f,  0.0f,
//    0.0f,  480.f,  0.0f,
//  640.0f,  480.0f,  0.0f,

//    0.0f,    0.f,  0.0f,
//  640.0f,  480.0f,  0.0f,
//  640.0f,    0.0f,  0.0f,
//};

std::vector<float> _square_colors {
    1.0f,  0.0f,  0.0f,
    0.0f,  1.0f,  0.0f,
    0.0f,  0.0f,  1.0f,

    1.0f,  0.0f,  0.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
};

std::vector<float> _square_texcoords = {
    0.0f, 0.0f,  // lower-left corner
    0.0f, 1.0f,   // upper-left corner
    1.0f, 1.0f,  // upper-right corner

    0.0f, 0.0f,  // lower-left corner
    1.0f, 1.0f,  // upper-right corner
    1.0f, 0.0f,  // lower-right corner
};
