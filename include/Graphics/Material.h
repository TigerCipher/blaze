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

#ifndef BLAZE_MATERIAL_H
#define BLAZE_MATERIAL_H

#include <glm/glm.hpp>
#include "Types.h"

#include "Shader.h"

namespace blaze::gfx
{

struct material
{
    i32 diffuse_texture;
    i32 specular_texture;
    f32 shininess;
};

void bind_material(const shader& shader, const material& mat);


} // namespace blaze::gfx

#endif //BLAZE_MATERIAL_H
