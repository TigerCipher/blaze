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

#include "Graphics/GLCore.h"
#include "Core/Logger.h"
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
    LOG_INFO("Creating window: {} ({}x{})", title, width, height);

    m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
    if (!m_window)
    {
        LOG_FATAL("Failed to create window. Error: {}", SDL_GetError());
        return false;
    }
    m_width = width;
    m_height = height;

    m_context = SDL_GL_CreateContext(m_window);
    if (!m_context)
    {
        LOG_FATAL("Failed to create OpenGL context. Error: {}", SDL_GetError());
        return false;
    }

    // for now disable vsync TODO: make this a setting
    if (SDL_GL_SetSwapInterval(0) < 0)
    {
        LOG_WARN("Failed to disable vsync. Error: {}", SDL_GetError());
    }

    gfx::init();

    m_alive = true;
    LOG_INFO("Window created successfully");
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
    LOG_INFO("Destroying window '{}'", SDL_GetWindowTitle(m_window));
    SDL_GL_DeleteContext(m_context);
    SDL_DestroyWindow(m_window);
    m_alive = false;
}
SDL_Window* window::handle() const
{
    return m_window;
}

void window::activate()
{
    SDL_GL_MakeCurrent(m_window, m_context);
}


bool init_graphics()
{
    LOG_INFO("Initializing SDL2");
    if (SDL_Init(SDL_INIT_VIDEO))
    {
        LOG_ERROR("Failed to initialize SDL2. Error: {}", SDL_GetError());
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

    // 8 bits per rgba channel
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    is_init = true;
    LOG_INFO("SDL2 initialized");
    return true;
}

void shutdown_graphics()
{
    if (!is_init)
    {
        return;
    }
    LOG_INFO("Shutting down SDL2");
    SDL_Quit();
    is_init = false;
}

} // namespace blaze