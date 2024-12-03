#include <glad/glad.h>
#include "scene_drawer.hpp"
#include <iostream>

SceneDrawer::SceneDrawer() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // Nearest for pixelated look.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, 160, 120, 0, GL_RED, GL_FLOAT, textureData.data());

    char infoLog[1024];
    int success;

    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertex_shader_code = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;

        void main() {
            gl_Position = vec4(aPos, 1.0);
        }
    )";
    glShaderSource(vertex_shader, 1, &vertex_shader_code, NULL);
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    glGetShaderInfoLog(vertex_shader, 1024, NULL, infoLog);
    if (!success) std::cout << infoLog;

    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragment_shader_code = R"(
        #version 330 core
        out vec4 FragColor;
        uniform sampler2D result_image;
        uniform vec2 resolution;
        const vec3 sand_color = vec3(203, 189, 147) / 255.0;

        void main() {
            float value = texture(result_image, gl_FragCoord.xy / resolution).r;
            FragColor = vec4(sand_color * value, 1.0);
        }
    )";
    glShaderSource(fragment_shader, 1, &fragment_shader_code, NULL);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    glGetShaderInfoLog(fragment_shader, 1024, NULL, infoLog);
    if (!success) std::cout << infoLog;

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    glGetShaderInfoLog(program, 1024, NULL, infoLog);
    if (!success) std::cout << infoLog;

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    textureData = std::vector<float>(160 * 120, 0);
    for (int y = 80; y < 100; ++y) {
        for (int x = 40; x < 120; ++x) {
            textureData[y * 160 + x] = 1.0;
        }
    }
}

void SceneDrawer::draw() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, 160, 120, 0, GL_RED, GL_FLOAT, textureData.data());

    glUseProgram(program);
    glUniform1i(glGetUniformLocation(program, "result_image"), 0);
    glUniform2f(glGetUniformLocation(program, "resolution"), 800, 600);
    
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

SceneDrawer::~SceneDrawer() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteTextures(1, &texture);
    glDeleteProgram(program);
}
