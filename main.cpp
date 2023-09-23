#include <glad.h>
#include <GLFW/glfw3.h>

#include "TerrainShader.hpp"
#include "Camera.hpp"
#include "LightPos.hpp"
#include "water.hpp"
#include "terrain.hpp"

#include <vector>
#include <algorithm>
#include <iostream>
#include <random>
#include <algorithm>
#include <cmath>

static float timeDelta = 0.0f;
static float prev_time = 0.0f;
static float prev = 0.0f;
static float diff = 0.0f;
static unsigned int count = 0;
static float FPS = 0.0f;
static bool wireframe = false;
static bool fps_count = false;
static bool cam_mode = true;

static int window_width = 1000;
static int window_height = 1000;
static Camera cam(glm::vec3(0.0f, 0.0f, 0.0f));

void frameSize_callBack(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double x_pos, double y_pos);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);


int main() {

	glfwInit();

	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(window_width, window_height, "Terrain Generation", NULL, NULL);
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSetFramebufferSizeCallback(window, frameSize_callBack);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwGetWindowSize(window, &window_width, &window_height);
	frameSize_callBack(window, window_width, window_height);

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glfwSwapBuffers(window);
	glfwPollEvents();

	Terrain terrain = Terrain(15.0f, 15.0f, 300);
	cam.set_position(glm::vec3(terrain.get_width() / 2.0f, 1.5f, terrain.get_height() / 2.0f));
	LightPos light = LightPos();
	Water water;
	
	Shader terrain_shader("C:/Users/chand/Documents/CG_Procedural_terrain/shaders/terrain/terrain.vert", "C:/Users/chand/Documents/CG_Procedural_terrain/shaders/terrain/terrain.frag", "C:/Users/chand/Documents/CG_Procedural_terrain/shaders/terrain/terrain.geom");
	Shader water_shader("C:/Users/chand/Documents/CG_Procedural_terrain/shaders/water/water.vert", "C:/Users/chand/Documents/CG_Procedural_terrain/shaders/water/water.frag");

	while (!glfwWindowShouldClose(window)) {
		float timeNow = static_cast<float>(glfwGetTime());
		timeDelta = timeNow - prev_time;
		prev_time = timeNow;

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		process_input(window);
		terrain.draw(terrain_shader, cam, light);
		terrain.update_height_data(terrain_shader);

		water.draw(water_shader, cam, light, 15.0f, 15.0f);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

// Processes continuous input
void process_input(GLFWwindow* window) {
	// Movement Controls
	if (cam_mode) {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) cam.move(Direction::Forward, timeDelta);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) cam.move(Direction::Backward, timeDelta);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) cam.move(Direction::Left, timeDelta);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) cam.move(Direction::Right, timeDelta);
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) cam.move(Direction::Up, timeDelta);
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) cam.move(Direction::Down, timeDelta);
	}
}

// Processes single click input
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	// Toggles Camera Movement Mode
	if (key == GLFW_KEY_F && action == GLFW_PRESS) {
		cam_mode = !cam_mode;
		if (cam_mode) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		} else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}

	// Closes the program
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void mouse_callback(GLFWwindow* window, double x_pos, double y_pos) {

	static float last_x = 900.0f / 2;
	static float last_y = 900.0f / 2;

	if (cam.get_first_mouse() == true) {
		last_x = (float)x_pos;
		last_y = (float)y_pos;
		cam.unset_first_mouse();
	}

	float x_offset = (float)x_pos - last_x;
	float y_offset = last_y - (float)y_pos;
	last_x = (float)x_pos;
	last_y = (float)y_pos;
	
	if (cam_mode)
		cam.rotate(x_offset, y_offset);
}

void frameSize_callBack(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	cam.set_aspect_ratio(width, height);
	window_width = width;
	window_height = height;
}
