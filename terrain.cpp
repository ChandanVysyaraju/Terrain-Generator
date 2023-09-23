#include <glad.h>
#include <iostream>

#include "terrain.hpp"
#include "TerrainShader.hpp"

Terrain::Terrain(float width, float height, int subdivs) {
    this->seed = 314159265;
    this->width = width;
    this->height = height;
    this->subdivs = subdivs;
    reset_params();
    MeshGeneration();
}

void Terrain::MeshGeneration() {
	height_data.resize(static_cast<int>(width) * static_cast<int>(height), 0.0f);

	for (float z = 0.0f; z < height; z += height / subdivs) {
		for (float x = 0.0f; x < width; x += width / subdivs) {
			// First triangle
			mesh_vertices.push_back(x);
			mesh_vertices.push_back(0.0f);
			mesh_vertices.push_back(z);

			mesh_vertices.push_back(x);
			mesh_vertices.push_back(0.0f);
			mesh_vertices.push_back(z + height / subdivs);

			mesh_vertices.push_back(x + width / subdivs);
			mesh_vertices.push_back(0.0f);
			mesh_vertices.push_back(z);

			// Second triangle
			mesh_vertices.push_back(x + width / subdivs);
			mesh_vertices.push_back(0.0f);
			mesh_vertices.push_back(z);

			mesh_vertices.push_back(x);
			mesh_vertices.push_back(0.0f);
			mesh_vertices.push_back(z + height / subdivs);

			mesh_vertices.push_back(x + width / subdivs);
			mesh_vertices.push_back(0.0f);
			mesh_vertices.push_back(z + height / subdivs);
		}
	}

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, mesh_vertices.size() * sizeof(float), mesh_vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}

void Terrain::draw(Shader& sh, Camera& cam, LightPos& light) {
    glm::mat4 model = glm::mat4(1.0f);

    sh.bind();
    sh.set_mat4x4("model", model); 

	sh.set_float("x_pos", cam.xz_pos.x);
	sh.set_float("z_pos", cam.xz_pos.y);
    sh.set_mat4x4("vp", cam.vp_matrix());
    
	sh.set_int("seed", this->seed);
	sh.set_float("lacunarity", this->lacunarity);
	sh.set_float("gain", this->gain);
    sh.set_float("amp", this->amplitude);
    sh.set_float("freq", this->frequency);
    sh.set_float("fudge", this->contrast);

	sh.set_vec3("light_color", glm::vec3(light.color[0], light.color[1], light.color[2]));
	sh.set_vec3("light_pos", glm::vec3(light.pos[0], light.pos[1], light.pos[2]));

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, mesh_vertices.size() / 3);
    glBindVertexArray(0);
}

void Terrain::update_height_data(Shader& shader) {
	for (int z = 0; z < static_cast<int>(height); ++z) {
		for (int x = 0; x < static_cast<int>(width); ++x) {
			height_data[z * static_cast<int>(width) + x] = amplitude * frequency;
		}
	}
}

void Terrain::reset_params() {
    // Set defaults
    this->amplitude = 2.5f;
    this->frequency = 1.0f;
    this->gain = 0.5f;
    this->lacunarity = 2.0f;
    this->contrast = 1.2f;
}