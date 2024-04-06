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
#include "Graphics/Light.h"

namespace blaze::gfx
{
void bind_light(const shader& shader, const light& light)
{
    shader.set_vec3("light.position", light.position);
    shader.set_vec3("light.ambient", light.ambient);
    shader.set_vec3("light.diffuse", light.diffuse);
    shader.set_vec3("light.specular", light.specular);
}
} // namespace blaze::gfx