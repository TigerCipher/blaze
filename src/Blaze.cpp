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

#include "Core/Input.h"


namespace blaze
{

namespace
{
bool                                          running = false;
bool                                          is_init = false;
std::string                                   default_window{};
std::string                                   current_window{};
std::unordered_map<std::string, uptr<window>> window_map{};

std::function<void(f32)> render_function;
std::function<void(f32)> update_function;
camera*                  cam{};


bool process_events()
{
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
            return false;
        }
        mouse::process(event);
        keyboard::process(event);

        if (event.type == SDL_MOUSEMOTION)
        {
            f32 xoffset, yoffset;
            if (mouse::is_cursor_locked())
            {
                xoffset = (f32) event.motion.xrel;
                yoffset = -(f32) event.motion.yrel;
            } else
            {
                xoffset = mouse::position().x - mouse::previous_position().x;
                yoffset = mouse::previous_position().y - mouse::position().y;
            }

            cam->process_mouse_movement(xoffset, yoffset);
        }

        if (event.type == SDL_MOUSEWHEEL)
        {
            cam->process_mouse_scroll(mouse::scroll());
            cam->set_projection(cam->zoom(),
                                (f32) window_map[current_window]->width() / (f32) window_map[current_window]->height(), 0.1f,
                                100.0f);
        }
    }
    return true;
}


} // anonymous namespace

bool init(camera* pcam, const std::function<void(f32)>& render_func, const std::function<void(f32)>& update_func)
{
    if (is_init)
    {
        return false;
    }
    LOG_INFO("Initializing Blaze");
    if (!init_graphics())
    {
        return false;
    }
    cam             = pcam;
    render_function = render_func;
    update_function = update_func;
    is_init         = true;
    LOG_INFO("Blaze initialized");
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
    if (!cam)
    {
        LOG_FATAL("No camera set!");
        return;
    }
    if (!is_init || running)
    {
        return;
    }
    running = true;

    u32 frame_counter = 0;
    f32 fps_timer     = get_time();

    f32 delta_time;
    f32 last_frame = 0.0f;

    cam->set_projection(cam->zoom(), (f32) window_map[current_window]->width() / (f32) window_map[current_window]->height(), 0.1f,
                        100.0f);

    while (running)
    {
        f32 current_frame = get_time();
        delta_time        = current_frame - last_frame;
        last_frame        = current_frame;

        running = process_events();
        update_function(delta_time);
        if (window_map.size() > 1)
        {
            gfx::activate_window(default_window);
        }
        render_function(delta_time);
        ++frame_counter;

        if (get_time() - fps_timer > 1.0f)
        {
            LOG_TRACE("FPS: {}", frame_counter);
            frame_counter = 0;
            fps_timer     = get_time();
        }

        mouse::update();
        keyboard::update();

        if (window_map.size() == 1)
        {
            window_map[default_window]->swap();
        }
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

f32 get_time()
{
    return (f32) SDL_GetPerformanceCounter() / (f32) SDL_GetPerformanceFrequency();
}

void exit_now()
{
    running = false;
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
