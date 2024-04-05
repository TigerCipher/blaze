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

#ifndef BLAZE_WINDOW_H
#define BLAZE_WINDOW_H

#include <string>

#include "Types.h"

struct SDL_Window;

namespace blaze
{

// TODO: Make this class generic so it can be used for other windowing libraries
class window
{
public:
    bool create(const std::string& title, i32 width, i32 height);
    void swap();
    void destroy();

    constexpr i32 width() const { return m_width; }
    constexpr i32 height() const { return m_height; }

    SDL_Window* handle() const;

    constexpr f32 aspect_ratio() const { return (f32) m_width / (f32) m_height; }

private:
    i32         m_width{};
    i32         m_height{};
    bool        m_alive{ false };
    SDL_Window* m_window{ nullptr };
    void*       m_context{ nullptr };
};


bool init_graphics();
void shutdown_graphics();

} // namespace blaze

#endif //BLAZE_WINDOW_H
