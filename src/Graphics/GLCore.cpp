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
#include "Graphics/GLCore.h"

#include <gl/glew.h>
#include <iostream>

namespace blaze::gfx
{

namespace
{
bool is_init = false;
} // anonymous namespace

bool init()
{
    if (is_init)
    {
        return false;
    }
    glewExperimental = true;
    u32 status = glewInit();
    if (status != GLEW_OK)
    {
        std::cerr << "GLEW failed to initialize: " << glewGetErrorString(status) << std::endl;
        return false;
    }
    is_init = true;
    return true;
}

void clear_screen(f32 r, f32 g, f32 b)
{
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cerr << "OpenGL Error: " << error << std::endl;
    }
}
} // namespace blaze::gfx