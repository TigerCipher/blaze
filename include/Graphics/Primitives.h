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

#ifndef BLAZE_PRIMITIVES_H
#define BLAZE_PRIMITIVES_H

#include "Types.h"
#include "Shader.h"
#include "GLCore.h"
#include <glm/glm.hpp>
#include <vector>


namespace blaze::gfx
{

struct vertex_position
{
    glm::vec3 position;
};

struct vertex_position_texcoords : vertex_position
{
    glm::vec2 texcoords;
    vertex_position_texcoords(glm::vec3 pos, glm::vec2 tex) : vertex_position{ pos }, texcoords{ tex } {}
};

struct vertex_position_normal : vertex_position
{
    glm::vec3 normal;
    vertex_position_normal(glm::vec3 pos, glm::vec3 norm) : vertex_position{ pos }, normal{ norm } {}
};

struct vertex_position_normal_texcoords : vertex_position_normal
{
    glm::vec2 texcoords;
    vertex_position_normal_texcoords(glm::vec3 pos, glm::vec3 norm, glm::vec2 tex) :
        vertex_position_normal{ pos, norm }, texcoords{ tex }
    {}
};

class primitive
{
public:
    primitive() = default;
    virtual ~primitive();

    void create(const shader& shader);
    void create_from_existing_vbo(const shader& shader, u32 attrib_count);
    void destroy();

    void bind() const;
    void draw(bool also_bind = true) const;

    virtual void bind_buffer_data() const = 0;

    constexpr u32 vbo() const { return m_vbo; }

protected:
    u32 m_vao{ u32_invalid_id };
    u32 m_vbo{ u32_invalid_id };
    u64 m_count{};

    std::unordered_map<std::string, const void*> m_attrib_offsets{};
};

template<typename T>
    requires std::same_as<T, vertex_position> || std::derived_from<T, vertex_position>
class cube : public primitive
{
public:
    cube(const sptr<cube<T>>& other)
    {
        m_attrib_offsets = other->m_attrib_offsets;
        m_vbo            = other->m_vbo;
        m_count          = other->m_count;
    }

    cube(f32 size = 1.0f)
    {
        f32 half_size = size / 2.0f;
        if constexpr (std::same_as<T, vertex_position>)
        {
            // clang-format off
            m_vertices.emplace_back(glm::vec3 { -half_size, -half_size, -half_size });
            m_vertices.emplace_back(glm::vec3  { half_size, -half_size, -half_size });
            m_vertices.emplace_back(glm::vec3   { half_size, half_size, -half_size });
            m_vertices.emplace_back(glm::vec3   { half_size, half_size, -half_size });
            m_vertices.emplace_back(glm::vec3  { -half_size, half_size, -half_size });
            m_vertices.emplace_back(glm::vec3 { -half_size, -half_size, -half_size });
            m_vertices.emplace_back(glm::vec3  { -half_size, -half_size, half_size });
            m_vertices.emplace_back(glm::vec3   { half_size, -half_size, half_size });
            m_vertices.emplace_back(glm::vec3    { half_size, half_size, half_size });
            m_vertices.emplace_back(glm::vec3    { half_size, half_size, half_size });
            m_vertices.emplace_back(glm::vec3   { -half_size, half_size, half_size });
            m_vertices.emplace_back(glm::vec3  { -half_size, -half_size, half_size });
            m_vertices.emplace_back(glm::vec3   { -half_size, half_size, half_size });
            m_vertices.emplace_back(glm::vec3  { -half_size, half_size, -half_size });
            m_vertices.emplace_back(glm::vec3 { -half_size, -half_size, -half_size });
            m_vertices.emplace_back(glm::vec3 { -half_size, -half_size, -half_size });
            m_vertices.emplace_back(glm::vec3  { -half_size, -half_size, half_size });
            m_vertices.emplace_back(glm::vec3   { -half_size, half_size, half_size });
            m_vertices.emplace_back(glm::vec3    { half_size, half_size, half_size });
            m_vertices.emplace_back(glm::vec3   { half_size, half_size, -half_size });
            m_vertices.emplace_back(glm::vec3  { half_size, -half_size, -half_size });
            m_vertices.emplace_back(glm::vec3  { half_size, -half_size, -half_size });
            m_vertices.emplace_back(glm::vec3   { half_size, -half_size, half_size });
            m_vertices.emplace_back(glm::vec3    { half_size, half_size, half_size });
            m_vertices.emplace_back(glm::vec3 { -half_size, -half_size, -half_size });
            m_vertices.emplace_back(glm::vec3  { half_size, -half_size, -half_size });
            m_vertices.emplace_back(glm::vec3   { half_size, -half_size, half_size });
            m_vertices.emplace_back(glm::vec3   { half_size, -half_size, half_size });
            m_vertices.emplace_back(glm::vec3  { -half_size, -half_size, half_size });
            m_vertices.emplace_back(glm::vec3 { -half_size, -half_size, -half_size });
            m_vertices.emplace_back(glm::vec3  { -half_size, half_size, -half_size });
            m_vertices.emplace_back(glm::vec3   { half_size, half_size, -half_size });
            m_vertices.emplace_back(glm::vec3    { half_size, half_size, half_size });
            m_vertices.emplace_back(glm::vec3    { half_size, half_size, half_size });
            m_vertices.emplace_back(glm::vec3   { -half_size, half_size, half_size });
            m_vertices.emplace_back(glm::vec3  { -half_size, half_size, -half_size });
            // clang-format on
            m_attrib_offsets["aPos"] = (void*) offsetof(vertex_position, position);
        } else if constexpr (std::same_as<T, vertex_position_texcoords>)
        {
            // clang-format off
            m_vertices.emplace_back(glm::vec3 { -half_size, -half_size, -half_size }, glm::vec2{ 0.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3  { half_size, -half_size, -half_size }, glm::vec2{ 1.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3   { half_size, half_size, -half_size }, glm::vec2{ 1.0f, 1.0f });
            m_vertices.emplace_back(glm::vec3   { half_size, half_size, -half_size }, glm::vec2{ 1.0f, 1.0f });
            m_vertices.emplace_back(glm::vec3  { -half_size, half_size, -half_size }, glm::vec2{ 0.0f, 1.0f });
            m_vertices.emplace_back(glm::vec3 { -half_size, -half_size, -half_size }, glm::vec2{ 0.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3  { -half_size, -half_size, half_size }, glm::vec2{ 0.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3   { half_size, -half_size, half_size }, glm::vec2{ 1.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3    { half_size, half_size, half_size }, glm::vec2{ 1.0f, 1.0f });
            m_vertices.emplace_back(glm::vec3    { half_size, half_size, half_size }, glm::vec2{ 1.0f, 1.0f });
            m_vertices.emplace_back(glm::vec3   { -half_size, half_size, half_size }, glm::vec2{ 0.0f, 1.0f });
            m_vertices.emplace_back(glm::vec3  { -half_size, -half_size, half_size }, glm::vec2{ 0.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3   { -half_size, half_size, half_size }, glm::vec2{ 1.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3  { -half_size, half_size, -half_size }, glm::vec2{ 1.0f, 1.0f });
            m_vertices.emplace_back(glm::vec3 { -half_size, -half_size, -half_size }, glm::vec2{ 0.0f, 1.0f });
            m_vertices.emplace_back(glm::vec3 { -half_size, -half_size, -half_size }, glm::vec2{ 0.0f, 1.0f });
            m_vertices.emplace_back(glm::vec3  { -half_size, -half_size, half_size }, glm::vec2{ 0.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3   { -half_size, half_size, half_size }, glm::vec2{ 1.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3    { half_size, half_size, half_size }, glm::vec2{ 1.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3   { half_size, half_size, -half_size }, glm::vec2{ 1.0f, 1.0f });
            m_vertices.emplace_back(glm::vec3  { half_size, -half_size, -half_size }, glm::vec2{ 0.0f, 1.0f });
            m_vertices.emplace_back(glm::vec3  { half_size, -half_size, -half_size }, glm::vec2{ 0.0f, 1.0f });
            m_vertices.emplace_back(glm::vec3   { half_size, -half_size, half_size }, glm::vec2{ 0.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3    { half_size, half_size, half_size }, glm::vec2{ 1.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3 { -half_size, -half_size, -half_size }, glm::vec2{ 0.0f, 1.0f });
            m_vertices.emplace_back(glm::vec3  { half_size, -half_size, -half_size }, glm::vec2{ 1.0f, 1.0f });
            m_vertices.emplace_back(glm::vec3   { half_size, -half_size, half_size }, glm::vec2{ 1.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3   { half_size, -half_size, half_size }, glm::vec2{ 1.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3  { -half_size, -half_size, half_size }, glm::vec2{ 0.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3 { -half_size, -half_size, -half_size }, glm::vec2{ 0.0f, 1.0f });
            m_vertices.emplace_back(glm::vec3  { -half_size, half_size, -half_size }, glm::vec2{ 0.0f, 1.0f });
            m_vertices.emplace_back(glm::vec3   { half_size, half_size, -half_size }, glm::vec2{ 1.0f, 1.0f });
            m_vertices.emplace_back(glm::vec3    { half_size, half_size, half_size }, glm::vec2{ 1.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3    { half_size, half_size, half_size }, glm::vec2{ 1.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3   { -half_size, half_size, half_size }, glm::vec2{ 0.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3  { -half_size, half_size, -half_size }, glm::vec2{ 0.0f, 1.0f });
            // clang-format on
            m_attrib_offsets["aPos"]       = (void*) offsetof(vertex_position_texcoords, position);
            m_attrib_offsets["aTexCoords"] = (void*) offsetof(vertex_position_texcoords, texcoords);
        } else if constexpr (std::same_as<T, vertex_position_normal>)
        {
            // clang-format off
            m_vertices.emplace_back(glm::vec3 { -half_size, -half_size, -half_size }, glm::vec3{ 0.0f, 0.0f, -1.0f });
            m_vertices.emplace_back(glm::vec3  { half_size, -half_size, -half_size }, glm::vec3{ 0.0f, 0.0f, -1.0f });
            m_vertices.emplace_back(glm::vec3   { half_size, half_size, -half_size }, glm::vec3{ 0.0f, 0.0f, -1.0f });
            m_vertices.emplace_back(glm::vec3   { half_size, half_size, -half_size }, glm::vec3{ 0.0f, 0.0f, -1.0f });
            m_vertices.emplace_back(glm::vec3  { -half_size, half_size, -half_size }, glm::vec3{ 0.0f, 0.0f, -1.0f });
            m_vertices.emplace_back(glm::vec3 { -half_size, -half_size, -half_size }, glm::vec3{ 0.0f, 0.0f, -1.0f });
            m_vertices.emplace_back(glm::vec3  { -half_size, -half_size, half_size }, glm::vec3{ 0.0f, 0.0f, 1.0f });
            m_vertices.emplace_back(glm::vec3   { half_size, -half_size, half_size }, glm::vec3{ 0.0f, 0.0f, 1.0f });
            m_vertices.emplace_back(glm::vec3    { half_size, half_size, half_size }, glm::vec3{ 0.0f, 0.0f, 1.0f });
            m_vertices.emplace_back(glm::vec3    { half_size, half_size, half_size }, glm::vec3{ 0.0f, 0.0f, 1.0f });
            m_vertices.emplace_back(glm::vec3   { -half_size, half_size, half_size }, glm::vec3{ 0.0f, 0.0f, 1.0f });
            m_vertices.emplace_back(glm::vec3  { -half_size, -half_size, half_size }, glm::vec3{ 0.0f, 0.0f, 1.0f });
            m_vertices.emplace_back(glm::vec3   { -half_size, half_size, half_size }, glm::vec3{ -1.0f, 0.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3  { -half_size, half_size, -half_size }, glm::vec3{ -1.0f, 0.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3 { -half_size, -half_size, -half_size }, glm::vec3{ -1.0f, 0.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3 { -half_size, -half_size, -half_size }, glm::vec3{ -1.0f, 0.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3  { -half_size, -half_size, half_size }, glm::vec3{ -1.0f, 0.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3   { -half_size, half_size, half_size }, glm::vec3{ -1.0f, 0.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3    { half_size, half_size, half_size }, glm::vec3{ 1.0f, 0.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3   { half_size, half_size, -half_size }, glm::vec3{ 1.0f, 0.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3  { half_size, -half_size, -half_size }, glm::vec3{ 1.0f, 0.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3  { half_size, -half_size, -half_size }, glm::vec3{ 1.0f, 0.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3   { half_size, -half_size, half_size }, glm::vec3{ 1.0f, 0.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3    { half_size, half_size, half_size }, glm::vec3{ 1.0f, 0.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3 { -half_size, -half_size, -half_size }, glm::vec3{ 0.0f, -1.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3  { half_size, -half_size, -half_size }, glm::vec3{ 0.0f, -1.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3   { half_size, -half_size, half_size }, glm::vec3{ 0.0f, -1.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3   { half_size, -half_size, half_size }, glm::vec3{ 0.0f, -1.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3  { -half_size, -half_size, half_size }, glm::vec3{ 0.0f, -1.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3 { -half_size, -half_size, -half_size }, glm::vec3{ 0.0f, -1.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3  { -half_size, half_size, -half_size }, glm::vec3{ 0.0f, 1.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3   { half_size, half_size, -half_size }, glm::vec3{ 0.0f, 1.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3    { half_size, half_size, half_size }, glm::vec3{ 0.0f, 1.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3    { half_size, half_size, half_size }, glm::vec3{ 0.0f, 1.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3   { -half_size, half_size, half_size }, glm::vec3{ 0.0f, 1.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3  { -half_size, half_size, -half_size }, glm::vec3{ 0.0f, 1.0f, 0.0f });
            // clang-format on
            m_attrib_offsets["aPos"]    = (void*) offsetof(vertex_position_normal, position);
            m_attrib_offsets["aNormal"] = (void*) offsetof(vertex_position_normal, normal);
        } else if constexpr (std::same_as<T, vertex_position_normal_texcoords>)
        {
            // clang-format off
            m_vertices.emplace_back(glm::vec3 { -half_size, -half_size, -half_size }, glm::vec3{ 0.0f, 0.0f, -1.0f }, glm::vec2{ 0.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3  { half_size, -half_size, -half_size }, glm::vec3{ 0.0f, 0.0f, -1.0f }, glm::vec2{ 1.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3   { half_size, half_size, -half_size }, glm::vec3{ 0.0f, 0.0f, -1.0f }, glm::vec2{ 1.0f, 1.0f });
            m_vertices.emplace_back(glm::vec3   { half_size, half_size, -half_size }, glm::vec3{ 0.0f, 0.0f, -1.0f }, glm::vec2{ 1.0f, 1.0f });
            m_vertices.emplace_back(glm::vec3  { -half_size, half_size, -half_size }, glm::vec3{ 0.0f, 0.0f, -1.0f }, glm::vec2{ 0.0f, 1.0f });
            m_vertices.emplace_back(glm::vec3 { -half_size, -half_size, -half_size }, glm::vec3{ 0.0f, 0.0f, -1.0f }, glm::vec2{ 0.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3  { -half_size, -half_size, half_size }, glm::vec3{ 0.0f, 0.0f, 1.0f } , glm::vec2{ 0.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3   { half_size, -half_size, half_size }, glm::vec3{ 0.0f, 0.0f, 1.0f } , glm::vec2{ 1.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3    { half_size, half_size, half_size }, glm::vec3{ 0.0f, 0.0f, 1.0f } , glm::vec2{ 1.0f, 1.0f });
            m_vertices.emplace_back(glm::vec3    { half_size, half_size, half_size }, glm::vec3{ 0.0f, 0.0f, 1.0f } , glm::vec2{ 1.0f, 1.0f });
            m_vertices.emplace_back(glm::vec3   { -half_size, half_size, half_size }, glm::vec3{ 0.0f, 0.0f, 1.0f } , glm::vec2{ 0.0f, 1.0f });
            m_vertices.emplace_back(glm::vec3  { -half_size, -half_size, half_size }, glm::vec3{ 0.0f, 0.0f, 1.0f } , glm::vec2{ 0.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3   { -half_size, half_size, half_size }, glm::vec3{ -1.0f, 0.0f, 0.0f }, glm::vec2{ 1.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3  { -half_size, half_size, -half_size }, glm::vec3{ -1.0f, 0.0f, 0.0f }, glm::vec2{ 1.0f, 1.0f });
            m_vertices.emplace_back(glm::vec3 { -half_size, -half_size, -half_size }, glm::vec3{ -1.0f, 0.0f, 0.0f }, glm::vec2{ 0.0f, 1.0f });
            m_vertices.emplace_back(glm::vec3 { -half_size, -half_size, -half_size }, glm::vec3{ -1.0f, 0.0f, 0.0f }, glm::vec2{ 0.0f, 1.0f });
            m_vertices.emplace_back(glm::vec3  { -half_size, -half_size, half_size }, glm::vec3{ -1.0f, 0.0f, 0.0f }, glm::vec2{ 0.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3   { -half_size, half_size, half_size }, glm::vec3{ -1.0f, 0.0f, 0.0f }, glm::vec2{ 1.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3    { half_size, half_size, half_size }, glm::vec3{ 1.0f, 0.0f, 0.0f } , glm::vec2{ 1.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3   { half_size, half_size, -half_size }, glm::vec3{ 1.0f, 0.0f, 0.0f } , glm::vec2{ 1.0f, 1.0f });
            m_vertices.emplace_back(glm::vec3  { half_size, -half_size, -half_size }, glm::vec3{ 1.0f, 0.0f, 0.0f } , glm::vec2{ 0.0f, 1.0f });
            m_vertices.emplace_back(glm::vec3  { half_size, -half_size, -half_size }, glm::vec3{ 1.0f, 0.0f, 0.0f } , glm::vec2{ 0.0f, 1.0f });
            m_vertices.emplace_back(glm::vec3   { half_size, -half_size, half_size }, glm::vec3{ 1.0f, 0.0f, 0.0f } , glm::vec2{ 0.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3    { half_size, half_size, half_size }, glm::vec3{ 1.0f, 0.0f, 0.0f } , glm::vec2{ 1.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3 { -half_size, -half_size, -half_size }, glm::vec3{ 0.0f, -1.0f, 0.0f }, glm::vec2{ 0.0f, 1.0f });
            m_vertices.emplace_back(glm::vec3  { half_size, -half_size, -half_size }, glm::vec3{ 0.0f, -1.0f, 0.0f }, glm::vec2{ 1.0f, 1.0f });
            m_vertices.emplace_back(glm::vec3   { half_size, -half_size, half_size }, glm::vec3{ 0.0f, -1.0f, 0.0f }, glm::vec2{ 1.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3   { half_size, -half_size, half_size }, glm::vec3{ 0.0f, -1.0f, 0.0f }, glm::vec2{ 1.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3  { -half_size, -half_size, half_size }, glm::vec3{ 0.0f, -1.0f, 0.0f }, glm::vec2{ 0.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3 { -half_size, -half_size, -half_size }, glm::vec3{ 0.0f, -1.0f, 0.0f }, glm::vec2{ 0.0f, 1.0f });
            m_vertices.emplace_back(glm::vec3  { -half_size, half_size, -half_size }, glm::vec3{ 0.0f, 1.0f, 0.0f } , glm::vec2{ 0.0f, 1.0f });
            m_vertices.emplace_back(glm::vec3   { half_size, half_size, -half_size }, glm::vec3{ 0.0f, 1.0f, 0.0f } , glm::vec2{ 1.0f, 1.0f });
            m_vertices.emplace_back(glm::vec3    { half_size, half_size, half_size }, glm::vec3{ 0.0f, 1.0f, 0.0f } , glm::vec2{ 1.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3    { half_size, half_size, half_size }, glm::vec3{ 0.0f, 1.0f, 0.0f } , glm::vec2{ 1.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3   { -half_size, half_size, half_size }, glm::vec3{ 0.0f, 1.0f, 0.0f } , glm::vec2{ 0.0f, 0.0f });
            m_vertices.emplace_back(glm::vec3  { -half_size, half_size, -half_size }, glm::vec3{ 0.0f, 1.0f, 0.0f } , glm::vec2{ 0.0f, 1.0f });
            // clang-format on
            m_attrib_offsets["aPos"]       = (void*) offsetof(vertex_position_normal_texcoords, position);
            m_attrib_offsets["aNormal"]    = (void*) offsetof(vertex_position_normal_texcoords, normal);
            m_attrib_offsets["aTexCoords"] = (void*) offsetof(vertex_position_normal_texcoords, texcoords);
        }

        m_count = m_vertices.size();
    }

    void bind_buffer_data() const override { buffer_data(m_vbo, (i64) m_vertices.size() * sizeof(T), m_vertices.data()); }


    static sptr<cube<T>> create_from_existing(const sptr<cube<T>>& c) { sptr<cube<T>> ret = make_sptr<cube<T>>(1.f); }

private:
    std::vector<T> m_vertices{};
};


} // namespace blaze::gfx

#endif //BLAZE_PRIMITIVES_H
