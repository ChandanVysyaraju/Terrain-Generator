#pragma once
#include <glm/glm.hpp>

#include <string>

enum class ShaderType {
    Vertex,
    Fragment,
    Geometry,
    ShaderProgram,
};

class Shader {
public:
    unsigned int ID;
    
    Shader(const char* vertex_shader_path, const char* fragment_shader_path, const char* geometry_shader_path = "none");

    /*Uniform setting methods*/

    void set_int(const std::string& variable_name, int value);
    void set_float(const std::string& variable_name, float value);
    void set_bool(const std::string& varibale_name, bool value);
    void set_mat4x4(const std::string& variable_name, glm::mat4 value);
    void set_vec3(const std::string& variable_name, glm::vec3 value);
    void set_vec2(const std::string& variable_name, glm::vec2 value);

    /*Binding methods*/

    void bind();
    void unbind();
private:
    const char* vertex_FileName;
    const char* frag_FileName;
    const char* geometry_FileName;
    void checkErrors(unsigned int ID, ShaderType type);
};