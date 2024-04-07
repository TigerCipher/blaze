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
#include "Core/Camera.h"

namespace blaze
{


bool init(camera* pcam, const std::function<void(f64)>& render_function, const std::function<void(f64)>& update_function);
void shutdown();
void run();


bool create_window(const std::string& title, i32 width, i32 height);

void exit_now();

f64 get_time();

} // namespace blaze

#endif //BLAZE_BLAZE_H
