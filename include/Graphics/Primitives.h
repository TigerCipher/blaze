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
#include <glm/glm.hpp>
#include <vector>


namespace blaze::gfx
{

struct vertex
{
    glm::vec3 position;
    glm::vec2 tex_coords;
};

class primitive
{
public:
    primitive()          = default;
    virtual ~primitive();

    void create(const shader& shader);
    void destroy();

    void bind() const;
    void draw(bool also_bind = true) const;

protected:
    u32 m_vao{ u32_invalid_id };
    u32 m_vbo{ u32_invalid_id };
    std::vector<vertex> m_vertices{};
};

class cube : public primitive
{
public:
    cube(f32 size = 1.0f);
};

} // namespace blaze::gfx

#endif //BLAZE_PRIMITIVES_H
