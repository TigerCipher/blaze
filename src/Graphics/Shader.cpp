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
    LOG_DEBUG("Reading shader file: {}", file);
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

GLenum get_basic_type(u32 type)
{
    switch (type)
    {
    case GL_FLOAT_VEC2:
    case GL_FLOAT_VEC3:
    case GL_FLOAT_VEC4:
    case GL_FLOAT: return GL_FLOAT;
    case GL_INT_VEC2:
    case GL_INT_VEC3:
    case GL_INT_VEC4:
    case GL_INT: return GL_INT;
    case GL_UNSIGNED_INT_VEC2:
    case GL_UNSIGNED_INT_VEC3:
    case GL_UNSIGNED_INT_VEC4:
    case GL_UNSIGNED_INT: return GL_UNSIGNED_INT;
    case GL_BYTE: return GL_BYTE;
    case GL_UNSIGNED_BYTE: return GL_UNSIGNED_BYTE;
    case GL_SHORT: return GL_SHORT;
    case GL_UNSIGNED_SHORT: return GL_UNSIGNED_SHORT;
    default: return GL_FLOAT;
    }
}

i32 get_attribute_size(u32 type)
{
    switch (type)
    {
    case GL_FLOAT_VEC2:
    case GL_INT_VEC2:
    case GL_UNSIGNED_INT_VEC2: return 2;
    case GL_FLOAT_VEC3:
    case GL_INT_VEC3:
    case GL_UNSIGNED_INT_VEC3: return 3;
    case GL_FLOAT_VEC4:
    case GL_INT_VEC4:
    case GL_UNSIGNED_INT_VEC4: return 4;
    case GL_FLOAT:
    case GL_INT:
    case GL_UNSIGNED_INT:
    case GL_BYTE:
    case GL_UNSIGNED_BYTE:
    case GL_SHORT:
    case GL_UNSIGNED_SHORT: return 1;
    default: return 1;
    }
}

i32 get_stride(u32 type)
{
    switch (type)
    {
    case GL_FLOAT_VEC2: return 2 * sizeof(f32);
    case GL_FLOAT_VEC3: return 3 * sizeof(f32);
    case GL_FLOAT_VEC4: return 4 * sizeof(f32);
    case GL_FLOAT: return sizeof(f32);
    case GL_INT_VEC2: return 2 * sizeof(i32);
    case GL_INT_VEC3: return 3 * sizeof(i32);
    case GL_INT_VEC4: return 4 * sizeof(i32);
    case GL_INT: return sizeof(i32);
    case GL_UNSIGNED_INT_VEC2: return 2 * sizeof(u32);
    case GL_UNSIGNED_INT_VEC3: return 3 * sizeof(u32);
    case GL_UNSIGNED_INT_VEC4: return 4 * sizeof(u32);
    case GL_UNSIGNED_INT: return sizeof(u32);
    case GL_BYTE: return sizeof(i8);
    case GL_UNSIGNED_BYTE: return sizeof(u8);
    case GL_SHORT: return sizeof(i16);
    case GL_UNSIGNED_SHORT: return sizeof(u16);
    default: return 0;
    }
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
    LOG_INFO("Shader [{}] loaded successfully", m_name);
    return true;
}

bool shader::compile(const std::string& vertex_shader, const std::string& fragment_shader)
{
    LOG_DEBUG("Compiling shader: {}", m_name);
    const char* vsrc = vertex_shader.c_str();
    const char* fsrc = fragment_shader.c_str();

    LOG_DEBUG("[{}] Compiling vertex shader: {}", m_name, m_vertex_file);
    // Compile vertex shader
    u32 vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vsrc, nullptr);
    glCompileShader(vertex);
    if (check_error(vertex, "VERTEX"))
    {
        glDeleteShader(vertex);
        return false;
    }

    LOG_DEBUG("[{}] Compiling fragment shader: {}", m_name, m_fragment_file);
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

    LOG_DEBUG("[{}] Linking shaders", m_name);
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

    // Cache uniform and attribute names/locations
    i32 count, max_name_length;
    // Uniforms
    glGetProgramiv(m_id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_name_length);
    glGetProgramiv(m_id, GL_ACTIVE_UNIFORMS, &count);
    for (i32 i = 0; i < count; ++i)
    {
        std::vector<char> name(max_name_length);
        i32               size;
        u32               type;
        glGetActiveUniform(m_id, i, max_name_length, nullptr, &size, &type, name.data());
        m_uniforms[name.data()] = glGetUniformLocation(m_id, name.data());
        LOG_DEBUG("[{}] Uniform: {} - Location: {}", m_name, name.data(), m_uniforms[name.data()]);
    }
    // Attributes
    glGetProgramiv(m_id, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &max_name_length);
    glGetProgramiv(m_id, GL_ACTIVE_ATTRIBUTES, &count);
    for (i32 i = 0; i < count; ++i)
    {
        std::vector<char> name(max_name_length);
        i32               size;
        u32               type;
        glGetActiveAttrib(m_id, i, max_name_length, nullptr, &size, &type, name.data());
        m_attributes[name.data()] = { glGetAttribLocation(m_id, name.data()), get_attribute_size(type), get_basic_type(type) };
        m_total_stride += get_stride(type);
        LOG_DEBUG("[{}] Attribute: {} - Location: {}", m_name, name.data(), m_attributes[name.data()].location);
    }

    // Clean up
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    LOG_INFO("Shader [{}] compiled successfully", m_name);
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
    glUniform1i(m_uniforms.at(name), (i32) value);
}

void shader::set_int(const std::string& name, i32 value) const
{
    glUniform1i(m_uniforms.at(name), value);
}

void shader::set_float(const std::string& name, f32 value) const
{
    glUniform1f(m_uniforms.at(name), value);
}

void shader::set_vec2(const std::string& name, const glm::vec2& value) const
{
    glUniform2fv(m_uniforms.at(name), 1, &value[0]);
}

void shader::set_vec3(const std::string& name, const glm::vec3& value) const
{
    glUniform3fv(m_uniforms.at(name), 1, &value[0]);
}

void shader::set_vec4(const std::string& name, const glm::vec4& value) const
{
    glUniform4fv(m_uniforms.at(name), 1, &value[0]);
}

void shader::set_mat4(const std::string& name, const glm::mat4& mat) const
{
    glUniformMatrix4fv(m_uniforms.at(name), 1, GL_FALSE, &mat[0][0]);
}

void shader::bind_attribute(const std::string& name, const void* pointer, bool normalized) const
{
    // Check if attribute exists
    auto it = m_attributes.find(name);
    if (it != m_attributes.end())
    {
        vertex_attribute attr     = it->second;
        u32              location = attr.location;
        glVertexAttribPointer(location, attr.count, attr.type, attr.normalized, m_total_stride, pointer);
        glEnableVertexAttribArray(location);
    }
}


void set_shaders_path(const std::string& path)
{
    shaders_path = path;
}

} // namespace blaze::gfx