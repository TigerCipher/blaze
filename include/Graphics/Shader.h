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

#ifndef BLAZE_SHADER_H
#define BLAZE_SHADER_H

#include <unordered_map>

#include "Types.h"
#include <glm/glm.hpp>

namespace blaze::gfx
{

struct vertex_attribute
{
    i32  location   = -1;
    i32  count      = -1;
    u32  type       = u32_invalid_id;
    bool normalized = false;
};

class shader
{
public:
    shader(std::string shader_name);
    ~shader();

    bool load();
    void bind() const;
    void destroy();

    // This will call both glEnableVertexAttribArray and glVertexAttribPointer
    void bind_attribute(const std::string& name, const void* pointer, bool normalized = false) const;

    void set_bool(const std::string& name, bool value) const;
    void set_int(const std::string& name, i32 value) const;
    void set_float(const std::string& name, f32 value) const;
    void set_vec2(const std::string& name, const glm::vec2& value) const;
    void set_vec3(const std::string& name, const glm::vec3& value) const;
    void set_vec4(const std::string& name, const glm::vec4& value) const;
    void set_mat4(const std::string& name, const glm::mat4& mat) const;

private:
    bool compile(const std::string& vertex_shader, const std::string& fragment_shader);

    u32         m_id{ u32_invalid_id };
    std::string m_name{};
    std::string m_vertex_file{};
    std::string m_fragment_file{};
    i32         m_total_stride{};

    std::unordered_map<std::string, i32>              m_uniforms{};
    std::unordered_map<std::string, vertex_attribute> m_attributes{};
};

[[maybe_unused]] void set_shaders_path(const std::string& path);

} // namespace blaze::gfx

#endif //BLAZE_SHADER_H
