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
#include "Graphics/GLCore.h"


namespace blaze::gfx
{

void primitive::create()
{
    GL_CALL(glGenVertexArrays(1, &m_vao));
    GL_CALL(glBindVertexArray(m_vao));

    if (m_owns_vbo)
    {
        GL_CALL(glGenBuffers(1, &m_vbo));
        bind_buffer_data();
    } else
    {
        GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
    }

    bind_vertex_attributes();
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
    if (m_vao != u32_invalid_id)
    {
        LOG_DEBUG("Destroying primitive, vao: {}, vbo: {}", m_vao, m_vbo);
        GL_CALL(glDeleteBuffers(1, &m_vbo));
        GL_CALL(glDeleteVertexArrays(1, &m_vao));
        m_vao = m_vbo = u32_invalid_id;
    }
}

void primitive::draw(bool also_bind) const
{
    if (also_bind)
    {
        bind();
    }
    GL_CALL(glDrawArrays(GL_TRIANGLES, 0, (i32) m_count));
}

void primitive::bind() const
{
    GL_CALL(glBindVertexArray(m_vao));
}


} // namespace blaze::gfx