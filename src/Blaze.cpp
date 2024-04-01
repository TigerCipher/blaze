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
#include "Blaze.h"

#include <SDL.h>

#include "Graphics/GLCore.h"

namespace blaze
{

namespace
{
bool                                          running = false;
bool                                          is_init = false;
std::string                                   default_window{};
std::string                                   current_window{};
std::unordered_map<std::string, uptr<window>> window_map{};

std::function<void()> render_function;
} // anonymous namespace

bool init()
{
    if (is_init)
    {
        return false;
    }
    if (!init_graphics())
    {
        return false;
    }
    is_init = true;
    return true;
}

void shutdown()
{
    if (!is_init)
    {
        return;
    }
    for (auto& [title, window] : window_map)
    {
        window->destroy();
    }

    shutdown_graphics();
    is_init = false;
}

void run()
{
    if (!is_init || running)
    {
        return;
    }
    running = true;
    while (running)
    {
        gfx::activate_window(default_window);
        render_function();

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if ((event.type == SDL_WINDOWEVENT) && (event.window.event == SDL_WINDOWEVENT_CLOSE))
            {
                for (auto& [title, window] : window_map)
                {
                    if (SDL_GetWindowFromID(event.window.windowID) == window->handle())
                    {
                        window->destroy();
                        break;
                    }
                }
            }
            if (event.type == SDL_QUIT)
            {
                running = false;
                break;
            }
        }
        //        for (auto& [title, window] : window_map)
        //        {
        //            window->swap();
        //        }
    }
}

const std::unordered_map<std::string, uptr<window>>& windows()
{
    return window_map;
}

bool create_window(const std::string& title, i32 width, i32 height)
{
    auto window = make_uptr<blaze::window>();
    if (window->create(title, width, height))
    {
        window_map.emplace(title, std::move(window));
        // First created window will be the default window to render to
        if (current_window.empty())
        {
            default_window = title;
            current_window = title;
        }
        return true;
    }
    return false;
}

void destroy_window(const std::string& title)
{
    auto it = window_map.find(title);
    if (it != window_map.end())
    {
        it->second->destroy();
        window_map.erase(it);
    }
}
void set_render_function(const std::function<void()>& rf)
{
    render_function = rf;
}


void gfx::activate_window(const std::string& title)
{
    auto it = window_map.find(title);
    if (it != window_map.end())
    {
        if (current_window != title)
        {
            window_map[current_window]->swap();
            current_window = title;
        }
        it->second->activate();
    }
}
} // namespace blaze