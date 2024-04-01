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
#include "Core/Window.h"

#include <SDL.h>

namespace blaze
{

namespace
{
bool is_init = false;
} // anonymous namespace

bool window::create(const std::string& title, i32 width, i32 height)
{
    if (!is_init)
    {
        return false;
    }

    m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
    if (!m_window)
    {
        // TODO: Log error
        return false;
    }

    m_context = SDL_GL_CreateContext(m_window);
    if (!m_context)
    {
        // TODO: Log error
        return false;
    }

    m_alive = true;
    return true;
}

void window::swap()
{
    if (!is_init)
    {
        return;
    }

    SDL_GL_SwapWindow(m_window);
}

void window::destroy()
{
    if (!is_init || !m_alive)
    {
        return;
    }

    SDL_GL_DeleteContext(m_context);
    SDL_DestroyWindow(m_window);
    m_alive = false;
}
SDL_Window* window::handle() const
{
    return m_window;
}


bool init_graphics()
{
    if (SDL_Init(SDL_INIT_VIDEO))
    {
        // TODO: Log error
        return false;
    }
    is_init = true;
    return true;
}

void shutdown_graphics()
{
    if (!is_init)
    {
        return;
    }
    SDL_Quit();
    is_init = false;
}
void temp_run() {
    init_graphics();
    bool running = true;
    window win;
    window win2;
    if (win.create("Sandbox", 1280, 720) && win2.create("Test", 400, 400))
    {
        while (running)
        {
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                if((event.type == SDL_WINDOWEVENT) && (event.window.event == SDL_WINDOWEVENT_CLOSE)){
                    if(SDL_GetWindowFromID(event.window.windowID) == win.handle()){
                        win.destroy();
                    } else if(SDL_GetWindowFromID(event.window.windowID) == win2.handle()){
                        win2.destroy();
                    }
                }
                if (event.type == SDL_QUIT)
                {
                    running = false;
                    break;
                }
            }

            win.swap();
            win2.swap();
        }

        win.destroy();
        win2.destroy();
    }
    shutdown_graphics();
}
} // namespace blaze