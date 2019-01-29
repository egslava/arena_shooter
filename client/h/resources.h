#ifndef SHADERS_H
#define SHADERS_H

const char *vertex_shader_code = ""
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aCol;\n"
"layout (location = 2) in vec2 aTex;\n"
"\n"
"out vec4 fragColor;\n"
"out vec2 fragTexCoord;\n"
"\n"
"void main()\n"
"{\n"
"	fragColor = vec4(aCol, 1);\n"
"	fragTexCoord = aTex;\n"
"    gl_Position = vec4(aPos.x, aPos.y, -aPos.z, 1.0);\n"
"}   \n";

const char *fragment_shader_code = ""
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec4 fragColor;\n"
"in vec2 fragTexCoord;\n"

"uniform sampler2D texture0;\n"
"\n"
"void main()\n"
"{\n"
"    FragColor = texture(texture0, fragTexCoord);\n"
//"    FragColor = texture(texture0, fragTexCoord) * fragColor;\n"
"}\n";

//"    FragColor = fragColor;\n"

std::vector<float> _triangle_points {
   0.5f, -0.5f,  0.0f,
  -0.5f, -0.5f,  0.0f,
    0.0f,  0.5f,  0.0f,
};

std::vector<float> _triangle_colors {
   0.0f,  1.0f,  0.0f,
   0.0f,  0.0f,  1.0f,
   1.0f,  0.0f,  0.0f,
};

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
#endif // SHADERS_H
