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
#include "Graphics/Primitives.h"

#include <gl/glew.h>

namespace blaze::gfx
{

void primitive::create(const shader& shader)
{
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, (i64) (m_vertices.size() * sizeof(vertex)), m_vertices.data(), GL_STATIC_DRAW);

    shader.bind_attribute("aPos", (void*) offsetof(vertex, position));
    shader.bind_attribute("aTexCoord", (void*) offsetof(vertex, tex_coords));
}

primitive::~primitive()
{
    // TODO should check if the gl context is still alive first as well
    if (m_vao != u32_invalid_id)
    {
        destroy();
    }
}

void primitive::destroy()
{
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
    m_vao = m_vbo = u32_invalid_id;
}

void primitive::draw() const
{
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, (i32) m_vertices.size());
}


cube::cube(f32 size)
{
    f32 half_size = size / 2.0f;
    // 36 vertices for a cube
    m_vertices = {
        { { -half_size, -half_size, -half_size }, { 0.0f, 0.0f } },
        {  { half_size, -half_size, -half_size }, { 1.0f, 0.0f } },
        {   { half_size, half_size, -half_size }, { 1.0f, 1.0f } },
        {   { half_size, half_size, -half_size }, { 1.0f, 1.0f } },
        {  { -half_size, half_size, -half_size }, { 0.0f, 1.0f } },
        { { -half_size, -half_size, -half_size }, { 0.0f, 0.0f } },

        {  { -half_size, -half_size, half_size }, { 0.0f, 0.0f } },
        {   { half_size, -half_size, half_size }, { 1.0f, 0.0f } },
        {    { half_size, half_size, half_size }, { 1.0f, 1.0f } },
        {    { half_size, half_size, half_size }, { 1.0f, 1.0f } },
        {   { -half_size, half_size, half_size }, { 0.0f, 1.0f } },
        {  { -half_size, -half_size, half_size }, { 0.0f, 0.0f } },

        {   { -half_size, half_size, half_size }, { 1.0f, 0.0f } },
        {  { -half_size, half_size, -half_size }, { 1.0f, 1.0f } },
        { { -half_size, -half_size, -half_size }, { 0.0f, 1.0f } },
        { { -half_size, -half_size, -half_size }, { 0.0f, 1.0f } },
        {  { -half_size, -half_size, half_size }, { 0.0f, 0.0f } },
        {   { -half_size, half_size, half_size }, { 1.0f, 0.0f } },

        {    { half_size, half_size, half_size }, { 1.0f, 0.0f } },
        {   { half_size, half_size, -half_size }, { 1.0f, 1.0f } },
        {  { half_size, -half_size, -half_size }, { 0.0f, 1.0f } },
        {  { half_size, -half_size, -half_size }, { 0.0f, 1.0f } },
        {   { half_size, -half_size, half_size }, { 0.0f, 0.0f } },
        {    { half_size, half_size, half_size }, { 1.0f, 0.0f } },

        { { -half_size, -half_size, -half_size }, { 0.0f, 1.0f } },
        {  { half_size, -half_size, -half_size }, { 1.0f, 1.0f } },
        {   { half_size, -half_size, half_size }, { 1.0f, 0.0f } },
        {   { half_size, -half_size, half_size }, { 1.0f, 0.0f } },
        {  { -half_size, -half_size, half_size }, { 0.0f, 0.0f } },
        { { -half_size, -half_size, -half_size }, { 0.0f, 1.0f } },

        {  { -half_size, half_size, -half_size }, { 0.0f, 1.0f } },
        {   { half_size, half_size, -half_size }, { 1.0f, 1.0f } },
        {    { half_size, half_size, half_size }, { 1.0f, 0.0f } },
        {    { half_size, half_size, half_size }, { 1.0f, 0.0f } },
        {   { -half_size, half_size, half_size }, { 0.0f, 0.0f } },
        {  { -half_size, half_size, -half_size }, { 0.0f, 1.0f } }
    };
}

} // namespace blaze::gfx