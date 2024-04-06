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

#ifndef BLAZE_GLCORE_H
#define BLAZE_GLCORE_H

#include "Types.h"


// a bit useless with the error callback, but can be helpful for tracking down errors
// glew must be included before this can be used
#ifdef _DEBUG
    #include "Core/Logger.h"
    #include <sstream>
    #define GL_CALL(x)                                                                                                           \
        x;                                                                                                                       \
        {                                                                                                                        \
            GLenum error = glGetError();                                                                                         \
            if (error != GL_NO_ERROR)                                                                                            \
            {                                                                                                                    \
                std::stringstream ss;                                                                                            \
                ss << glewGetErrorString(error);                                                                                 \
                LOG_ERROR("[{}][{}] OpenGL Error: {}, {}", __FILE__, __LINE__, error, ss.str());                                 \
            }                                                                                                                    \
        }
#else
    #define GL_CALL(x) x
#endif


namespace blaze::gfx
{
bool init();

void clear_screen(f32 r, f32 g, f32 b);

// This also binds the buffer
void buffer_data(u32 vbo, i64 size, const void* data);

} // namespace blaze::gfx

#endif //BLAZE_GLCORE_H
