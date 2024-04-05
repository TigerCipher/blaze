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
namespace blaze::gfx
{
bool init();

void clear_screen(f32 r, f32 g, f32 b);

// This also binds the buffer
void buffer_data(u32 vbo, i64 size, const void* data);

}

#endif //BLAZE_GLCORE_H
