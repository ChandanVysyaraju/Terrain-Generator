#pragma once
#include "TerrainShader.hpp"
#include "Camera.hpp"
#include "LightPos.hpp"

class Water {
public:
    Water();
    void resetParams();
    void draw(Shader& sh, Camera& campos, LightPos& light, float width, float height);

    float water_level;
private:
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    glm::vec3 color;
};