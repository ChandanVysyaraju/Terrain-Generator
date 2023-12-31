#include <glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "TerrainShader.hpp"

#include <string>
#include <fstream>
#include <iostream>

// Create a shader given a file path to a vertex and fragment shader
Shader::Shader(const char* vrtx_path, const char* frag_path, const char* geom_path) :
    vertex_FileName(vrtx_path), frag_FileName(frag_path), geometry_FileName(geom_path)
{
    // Vertex Shader
    std::string v_line, v_text;
    std::ifstream v_file(vrtx_path);
    while (std::getline(v_file, v_line)) {
        v_text += v_line + "\n";
    }
    v_file.close();
    const char* v_source = v_text.c_str();

    unsigned int vertex_shader;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &v_source, NULL);
    glCompileShader(vertex_shader);
    checkErrors(vertex_shader, ShaderType::Vertex);


    // Fragment Shader
    std::string f_line, f_text;
    std::ifstream f_file(frag_path);
    while (std::getline(f_file, f_line)) {
        f_text += f_line + "\n";
    }
    f_file.close();
    const char* f_source = f_text.c_str();

    unsigned int fragment_shader;
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &f_source, NULL);
    glCompileShader(fragment_shader);
    checkErrors(fragment_shader, ShaderType::Fragment);


    // Geometry Shader
    unsigned int geometry_shader;
    if (std::string(geom_path) != "none") {
        std::string g_line, g_text;
        std::ifstream g_file(geom_path);
        while (std::getline(g_file, g_line)) {
            g_text += g_line + "\n";
        } 
        g_file.close();
        const char* g_source = g_text.c_str(); 

        geometry_shader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry_shader, 1, &g_source, NULL);
        glCompileShader(geometry_shader);
        checkErrors(geometry_shader, ShaderType::Geometry);
    }

    // Linking
    this->ID = glCreateProgram();
    glAttachShader(this->ID, vertex_shader);
    glAttachShader(this->ID, fragment_shader);
    if (std::string(geom_path) != "none") glAttachShader(this->ID, geometry_shader);
    glLinkProgram(this->ID);
    checkErrors(this->ID, ShaderType::ShaderProgram);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    if (std::string(geom_path) != "none") glDeleteShader(geometry_shader);
}

// Check for either linking for compile errors
void Shader::checkErrors(unsigned int ID, ShaderType type) {
    int success;
    char errorLog[512];
    switch (type) {
    case ShaderType::Vertex:
        glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(ID, 512, NULL, errorLog);
            std::cout << "Problem compiling VERTEX shader: " << this->vertex_FileName << "\n" << errorLog << std::endl;
        }
        break;
    case ShaderType::Fragment:
        glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(ID, 512, NULL, errorLog);
            std::cout << "Problem compiling FRAGMENT shader: " << this->frag_FileName << "\n" << errorLog << std::endl;
        }
        break;
    case ShaderType::Geometry:
        glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(ID, 512, NULL, errorLog);
            std::cout << "Problem compiling GEOMETRY shader: " << this->geometry_FileName << "\n" << errorLog << std::endl;
        }
        break;
    case ShaderType::ShaderProgram:
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(ID, 512, NULL, errorLog);
            std::cout << "Problem linking " << this->vertex_FileName << " and " << this->frag_FileName << ((this->geometry_FileName == "none") ? ":" : " and " + std::string(this->geometry_FileName) + ":")  << "\n" << errorLog << std::endl;
        }
        break;
    default:
        std::cout << "Provided shader type was invalid, no errors have been checked" << std::endl;
        break;
    }
}

// Bind shader to OpenGL state
void Shader::bind() {
    glUseProgram(this->ID);
}

// Unbind shader from OpenGL state
void Shader::unbind() {
    glUseProgram(0);
}

// Send an integer uniform to the shader
void Shader::set_int(const std::string& variable_name, int value) {
    glUniform1i(glGetUniformLocation(this->ID, variable_name.c_str()), value);
}

// Send a float uniform to the shader
void Shader::set_float(const std::string& variable_name, float value) {
    glUniform1f(glGetUniformLocation(this->ID, variable_name.c_str()), value);
}

// Send a boolean uniform to the shader
void Shader::set_bool(const std::string& variable_name, bool value) {
    glUniform1i(glGetUniformLocation(this->ID, variable_name.c_str()), value);
}

// Send a 4x4 matrix uniform to the shader
void Shader::set_mat4x4(const std::string& variable_name, glm::mat4 value) {
    glUniformMatrix4fv(glGetUniformLocation(this->ID, variable_name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

// Send a vec3 uniform to the shader
void Shader::set_vec3(const std::string& variable_name, glm::vec3 value) {
    glUniform3fv(glGetUniformLocation(this->ID, variable_name.c_str()), 1, glm::value_ptr(value));
}

// Send a vec2 uniform to the shader
void Shader::set_vec2(const std::string& variable_name, glm::vec2 value) {
    glUniform2fv(glGetUniformLocation(this->ID, variable_name.c_str()), 1, glm::value_ptr(value));
}

