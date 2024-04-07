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

#ifndef BLAZE_RENDERITEM_H
#define BLAZE_RENDERITEM_H

#include <glm/glm.hpp>
#include "Types.h"
#include "Material.h"
#include "Primitives.h"

namespace blaze::gfx
{

class render_item
{
public:
    render_item(primitive* obj, const material& mat) : m_object(obj), m_material(mat) {}
    ~render_item();

    void set_model(const glm::mat4& model) { m_model = model; }
    void draw(const shader& shader) const;

private:
    primitive* m_object;
    material   m_material;
    glm::mat4  m_model;
};

} // namespace blaze::gfx

#endif //BLAZE_RENDERITEM_H
