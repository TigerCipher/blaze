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
    virtual ~primitive();

    void create();
    void destroy();

    void bind() const;
    void draw(bool also_bind = true) const;

    virtual void bind_buffer_data() const       = 0;
    virtual void bind_vertex_attributes() const = 0;

    constexpr u32 vbo() const { return m_vbo; }
    constexpr u32 count() const { return m_count; }
    constexpr i32 stride() const { return m_stride; }

protected:
    u32  m_vao{ u32_invalid_id };
    u32  m_vbo{ u32_invalid_id };
    u64  m_count{};
    i32  m_stride{};
    bool m_owns_vbo{ true };
};

template<typename T>
    requires std::same_as<T, vertex_position> || std::derived_from<T, vertex_position>
class cube : public primitive
{
public:
    template<typename C>
        requires std::same_as<C, vertex_position> || std::derived_from<C, vertex_position>
    cube(const sptr<cube<C>>& other)
    {
        m_vbo      = other->vbo();
        m_count    = other->count();
        m_stride   = other->stride();
        m_owns_vbo = false;
    }

    cube(f32 size = 1.0f)
    {
        m_stride      = sizeof(T);
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
        }

        m_count = m_vertices.size();
    }

    void bind_vertex_attributes() const override
    {
        if constexpr (std::same_as<T, vertex_position>)
        {
            bind_vertex_vec3_attribute(0, m_stride, (void*) offsetof(vertex_position, position));
        } else if constexpr (std::same_as<T, vertex_position_texcoords>)
        {
            bind_vertex_vec3_attribute(0, m_stride, (void*) offsetof(vertex_position_texcoords, position));
            bind_vertex_vec2_attribute(1, m_stride, (void*) offsetof(vertex_position_texcoords, texcoords));
        } else if constexpr (std::same_as<T, vertex_position_normal>)
        {
            bind_vertex_vec3_attribute(0, m_stride, (void*) offsetof(vertex_position_normal, position));
            bind_vertex_vec3_attribute(1, m_stride, (void*) offsetof(vertex_position_normal, normal));
        } else if constexpr (std::same_as<T, vertex_position_normal_texcoords>)
        {
            bind_vertex_vec3_attribute(0, m_stride, (void*) offsetof(vertex_position_normal_texcoords, position));
            bind_vertex_vec3_attribute(1, m_stride, (void*) offsetof(vertex_position_normal_texcoords, normal));
            bind_vertex_vec2_attribute(2, m_stride, (void*) offsetof(vertex_position_normal_texcoords, texcoords));
        }
    }

    void bind_buffer_data() const override { buffer_data(m_vbo, (i64) m_vertices.size() * sizeof(T), m_vertices.data()); }

private:
    std::vector<T> m_vertices{};
};


} // namespace blaze::gfx

#endif //BLAZE_PRIMITIVES_H
