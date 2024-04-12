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
#include "Graphics/Material.h"

namespace blaze::gfx
{
void bind_material(const shader& shader, const material& mat)
{
    if (!mat.diffuse || !mat.specular)
    {
        return;
    }
    mat.diffuse->bind();
    mat.specular->bind(1);
    shader.bind();
    shader.set_float("material.shininess", mat.shininess);
}
} // namespace blaze::gfx