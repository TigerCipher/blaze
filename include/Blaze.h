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

#ifndef BLAZE_BLAZE_H
#define BLAZE_BLAZE_H

#include <unordered_map>
#include <functional>

#include "Core/Window.h"
#include "Core/Logger.h"

namespace blaze
{
bool init();
void shutdown();
void run();

const std::unordered_map<std::string, uptr<window>>& windows();

bool create_window(const std::string& title, i32 width, i32 height);
void destroy_window(const std::string& title);

void set_render_function(const std::function<void()>& render_function);

f32 get_time();

namespace gfx
{
void activate_window(const std::string& title);
} // namespace gfx
} // namespace blaze

#endif //BLAZE_BLAZE_H
