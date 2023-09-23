#pragma once
#include <vector>
#include "TerrainShader.hpp"
#include "camera.hpp"
#include "water.hpp"
#include "LightPos.hpp"

class Terrain {
public:
    Terrain(float width, float height, int subdivs);
    void draw(Shader& shader, Camera& cam, LightPos& light);
    void reset_params();

    int get_width() const { return static_cast<int>(width); }
    int get_height() const { return static_cast<int>(height); }
    void update_height_data(Shader& sh);
    const std::vector<float>& get_height_data() const { return height_data; }

    float contrast;
    int seed;
    bool wireframe_rendering;
    float amplitude;
    float frequency;
    float gain;
    float lacunarity;
    

    float get_width() {return width;}
    float get_height() {return height;}
    int get_subdivs() {return subdivs;}
private:
    float width;
    float height;
    int subdivs;

    unsigned int VAO;
    unsigned int VBO;

    std::vector<float> mesh_vertices;
    std::vector<float> height_data;
    void MeshGeneration();
};