#include "material.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <glad/gl.h>

Material::Material(const char *vertex_shader_path, const char *fragment_shader_path)
{
    std::string vertex_code;
    std::string fragment_code;
    std::ifstream vertex_file;
    std::ifstream fragment_file;

    vertex_file.open(vertex_shader_path);
    fragment_file.open(fragment_shader_path);

    if (!vertex_file.is_open())
    {
        std::cout << "Shader Path: " << vertex_shader_path << std::endl;
        throw std::runtime_error("Failed to open Vertex Shader file ");
    }
    if (!fragment_file.is_open())
    {
        std::cout << "Shader Path: " << fragment_shader_path << std::endl;
        throw std::runtime_error("Failed to open Fragment Shader file ");
    }

    std::stringstream vertex_stream, fragment_stream;

    vertex_stream << vertex_file.rdbuf();
    fragment_stream << fragment_file.rdbuf();

    vertex_file.close();
    fragment_file.close();

    vertex_code = vertex_stream.str();
    fragment_code = fragment_stream.str();

    uint32_t vertex, fragment;
    int success;
    char info_log[512];

    const char *vertex_code_cstr = vertex_code.c_str();
    const char *fragment_code_cstr = fragment_code.c_str();

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertex_code_cstr, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, info_log);

        std::cout << "Shader Location: " << vertex_shader_path << std::endl;
        std::cout << "Shader Compilation Error:\n" << info_log << std::endl;

        throw std::runtime_error("Failed to compile vertex shader");
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragment_code_cstr, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, info_log);

        std::cout << "Shader Location: " << fragment_shader_path << std::endl;
        std::cout << "Shader Compilation Error:\n" << info_log << std::endl;

        throw std::runtime_error("Failed to compile fragment shader");
    }

    m_id = glCreateProgram();
    glAttachShader(m_id, vertex);
    glAttachShader(m_id, fragment);
    glLinkProgram(m_id);

    glGetProgramiv(m_id, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(m_id, 512, NULL, info_log);
        std::cout << "Shader Program Link Error:\n" << info_log << std::endl;
        throw std::runtime_error("Failed to link shader program");
    }
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}
Material::~Material()
{
    glDeleteProgram(m_id);
}
