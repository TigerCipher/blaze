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

#ifndef BLAZE_LIGHT_H
#define BLAZE_LIGHT_H

#include <glm/glm.hpp>
#include "Types.h"
#include "Shader.h"

namespace blaze::gfx
{

struct light
{
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

struct directional_light
{
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

struct point_light
{
    glm::vec3 position;

    f32 constant = 1.0f; // ever really need this? pretty much will always be 1
    f32 linear;
    f32 quadratic;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

struct spot_light
{
    glm::vec3 position;
    glm::vec3 direction;

    f32 constant = 1.0f;
    f32 linear;
    f32 quadratic;

    f32 cutoff;
    f32 outer_cutoff;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

void bind_light(const shader& shader, const light& light);

void bind_light(const shader& shader, const directional_light& light);

void bind_light(const shader& shader, const point_light& light, u32 index);

void bind_light(const shader& shader, const spot_light& light);

}

#endif //BLAZE_LIGHT_H
