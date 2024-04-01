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

namespace blaze::window
{

namespace
{
SDL_Window*   window  = nullptr;
SDL_GLContext context = nullptr;
bool          is_init = false;
} // anonymous namespace

bool create(const std::string& title, i32 width, i32 height)
{
    if (is_init)
    {
        // TODO: Log error
        return false;
    }

    if (SDL_Init(SDL_INIT_VIDEO))
    {
        // TODO: Log error
        return false;
    }

    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
    if (!window)
    {
        // TODO: Log error
        return false;
    }

    context = SDL_GL_CreateContext(window);
    if (!context)
    {
        // TODO: Log error
        return false;
    }

    is_init = true;
    return true;
}

void show()
{
    if (!is_init)
    {
        return;
    }

    while (true)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                return;
            }
        }

        SDL_GL_SwapWindow(window);
    }
}

void destroy()
{
    if (!is_init)
    {
        return;
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    is_init = false;
}

} // namespace blaze::window