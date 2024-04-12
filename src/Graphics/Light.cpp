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
#include "Core/Logger.h"

namespace blaze::gfx
{
void bind_light(const shader& shader, const light& light)
{
    shader.set_vec3("light.position", light.position);
    shader.set_vec3("light.ambient", light.ambient);
    shader.set_vec3("light.diffuse", light.diffuse);
    shader.set_vec3("light.specular", light.specular);
}

void gfx::bind_light(const shader& shader, const directional_light& light)
{
    shader.set_vec3("dirLight.direction", light.direction);
    shader.set_vec3("dirLight.ambient", light.ambient);
    shader.set_vec3("dirLight.diffuse", light.diffuse);
    shader.set_vec3("dirLight.specular", light.specular);
}

void gfx::bind_light(const shader& shader, const point_light& light, u32 index)
{
    shader.set_vec3("pointLights[" + std::to_string(index) + "].position", light.position);
    shader.set_float("pointLights[" + std::to_string(index) + "].constant", light.constant);
    shader.set_float("pointLights[" + std::to_string(index) + "].linear", light.linear);
    shader.set_float("pointLights[" + std::to_string(index) + "].quadratic", light.quadratic);
    shader.set_vec3("pointLights[" + std::to_string(index) + "].ambient", light.ambient);
    shader.set_vec3("pointLights[" + std::to_string(index) + "].diffuse", light.diffuse);
    shader.set_vec3("pointLights[" + std::to_string(index) + "].specular", light.specular);
}

void gfx::bind_light(const shader& shader, const spot_light& light)
{
    shader.set_vec3("spotLight.position", light.position);
    shader.set_vec3("spotLight.direction", light.direction);
    shader.set_float("spotLight.cutoff", light.cutoff);
    shader.set_float("spotLight.outerCutoff", light.outer_cutoff);
    shader.set_float("spotLight.constant", light.constant);
    shader.set_float("spotLight.linear", light.linear);
    shader.set_float("spotLight.quadratic", light.quadratic);
    shader.set_vec3("spotLight.ambient", light.ambient);
    shader.set_vec3("spotLight.diffuse", light.diffuse);
    shader.set_vec3("spotLight.specular", light.specular);
}

} // namespace blaze::gfx