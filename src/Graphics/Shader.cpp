//  ------------------------------------------------------------------------------
//
//  blaze
//     Copyright 2024 Matthew Rogers
//
//     Licensed under the Apache License, Version 2.0 (the "License");
//     you may not use this file except in compliance with the License.
//     You may obtain a copy of the License at
//
//         http://www.apache.org/licenses/LICENSE-2.0
//
//     Unless required by applicable law or agreed to in writing, software
//     distributed under the License is distributed on an "AS IS" BASIS,
//     WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//     See the License for the specific language governing permissions and
//     limitations under the License.
//
//  ------------------------------------------------------------------------------
#include <fstream>
#include <sstream>
#include <utility>
#include <GL/glew.h>

#include "Graphics/Shader.h"
#include "Core/Logger.h"

namespace blaze::gfx
{

namespace
{
std::string shaders_path = "./assets/shaders/";


std::string read_file(const std::string& file)
{
    std::string   source{};
    std::ifstream stream;
    stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        stream.open(file);
        std::stringstream buffer;
        buffer << stream.rdbuf();
        stream.close();
        source = buffer.str();
    } catch (std::ifstream::failure& e)
    {
        LOG_ERROR("Failed to read shader file [{}]: {}", file, e.what());
        return {};
    }
    return source;
}

bool check_error(u32 id, const std::string& type)
{
    i32  success;
    char info_log[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(id, 1024, nullptr, info_log);
            LOG_ERROR("Shader compilation error: {}", info_log);
            return true;
        }
    } else
    {
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(id, 1024, nullptr, info_log);
            LOG_ERROR("Shader linking error: {}", info_log);
            return true;
        }
    }

    return false;
}

} // anonymous namespace

shader::shader(std::string shader_name) : m_name(std::move(shader_name)) {}

bool shader::load()
{
    m_vertex_file               = shaders_path + m_name + ".vs";
    m_fragment_file             = shaders_path + m_name + ".fs";
    std::string vertex_source   = read_file(m_vertex_file);
    std::string fragment_source = read_file(m_fragment_file);
    if (vertex_source.empty() || fragment_source.empty())
    {
        return false;
    }

    if (!compile(vertex_source, fragment_source))
    {
        return false;
    }

    return true;
}

bool shader::compile(const std::string& vertex_shader, const std::string& fragment_shader)
{
    const char* vsrc = vertex_shader.c_str();
    const char* fsrc = fragment_shader.c_str();

    // Compile vertex shader
    u32 vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vsrc, nullptr);
    glCompileShader(vertex);
    if (check_error(vertex, "VERTEX"))
    {
        glDeleteShader(vertex);
        return false;
    }

    // Compile fragment shader
    u32 fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fsrc, nullptr);
    glCompileShader(fragment);
    if (check_error(fragment, "FRAGMENT"))
    {
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        return false;
    }

    // Link shaders
    m_id = glCreateProgram();
    glAttachShader(m_id, vertex);
    glAttachShader(m_id, fragment);
    glLinkProgram(m_id);
    if (check_error(m_id, "PROGRAM"))
    {
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        return false;
    }

    // Clean up
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return true;
}

shader::~shader()
{
    if (m_id != u32_invalid_id)
    {
        glDeleteProgram(m_id);
    }
}

void shader::bind() const
{
    glUseProgram(m_id);
}

void shader::destroy()
{
    glDeleteProgram(m_id);
    m_id = u32_invalid_id;
}

void shader::set_bool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), (i32) value);
}

void shader::set_int(const std::string& name, i32 value) const
{
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
}

void shader::set_float(const std::string& name, f32 value) const
{
    glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
}

void shader::set_vec2(const std::string& name, const glm::vec2& value) const
{
    glUniform2fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]);
}

void shader::set_vec3(const std::string& name, const glm::vec3& value) const
{
    glUniform3fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]);
}

void shader::set_vec4(const std::string& name, const glm::vec4& value) const
{
    glUniform4fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]);
}

void shader::set_mat4(const std::string& name, const glm::mat4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}


void set_shaders_path(const std::string& path)
{
    shaders_path = path;
}

} // namespace blaze::gfx