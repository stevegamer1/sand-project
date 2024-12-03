#pragma once
#include <vector>

class SceneDrawer {
private:
    const float vertices[9] = {
        -1.0f,  3.0f,  0.0f,
        -1.0f, -1.0f,  0.0f,
         3.0f, -1.0f,  0.0f
    };
    unsigned int VAO;
    unsigned int VBO;
    unsigned int texture;
    unsigned int program;

public:
    std::vector<float> textureData;

    SceneDrawer();
    void draw();
    ~SceneDrawer();
};
