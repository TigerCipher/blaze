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
#include <deque>
#include <numeric>

#include "Core/Input.h"


namespace blaze
{

namespace
{
bool running = false;
bool is_init = false;

window win{};

std::function<void(f64)> render_function;
std::function<void(f64)> update_function;
camera*                  cam{};


bool process_events()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
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
            cam->set_projection(cam->zoom(), win.aspect_ratio(), 0.1f, 100.0f);
        }
    }
    return true;
}


} // anonymous namespace

bool init(camera* pcam, const std::function<void(f64)>& render_func, const std::function<void(f64)>& update_func)
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
    win.destroy();

    shutdown_graphics();
    is_init = false;
}

#define FPS_COUNTER 0

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
    f64 fps_timer     = get_time();

    f64 delta_time;
    f64 last_frame = 0.0;

    constexpr i32 average_frame_count = 5000;
    std::deque<f64> frame_times(average_frame_count, 0.0f);

    cam->set_projection(cam->zoom(), win.aspect_ratio(), 0.1f,
                        100.0f);

    while (running)
    {
        f64 current_frame = get_time();
        delta_time        = current_frame - last_frame;
        last_frame        = current_frame;

        frame_times.pop_front();
        frame_times.push_back(delta_time);
        f64 avg_delta = std::accumulate(frame_times.begin(), frame_times.end(), 0.0) / average_frame_count;

        running = process_events();
        update_function(avg_delta);

        render_function(avg_delta);

#if FPS_COUNTER
        ++frame_counter;

        if (get_time() - fps_timer > 1.0)
        {
            LOG_TRACE("FPS: {}", frame_counter);
            LOG_TRACE("Avg. delta: {}, Delta time: {}", avg_delta, delta_time);
            LOG_TRACE("Frame time: {}", 1.0f / frame_counter);
            frame_counter = 0;
            fps_timer     = get_time();
        }
#endif

        mouse::update();
        keyboard::update();

        win.swap();
    }
}

bool create_window(const std::string& title, i32 width, i32 height)
{
    return win.create(title, width, height);
}


f64 get_time()
{
    return (f64)SDL_GetPerformanceCounter() / (f64)SDL_GetPerformanceFrequency();
}

void exit_now()
{
    running = false;
}

} // namespace blaze
