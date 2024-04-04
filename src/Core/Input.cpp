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
#include "Core/Input.h"

#include <SDL.h>

namespace blaze
{

namespace mouse
{

namespace
{
struct mouse_state
{
    bool buttons[button_count];
    f32  x;
    f32  y;
    f32  scroll;
};

struct input_state
{
    mouse_state current{};
    mouse_state previous{};
};

input_state state{};

} // anonymous namespace

void update()
{
    state.previous = state.current;
}

void process(SDL_Event& event)
{
    if (event.type == SDL_MOUSEBUTTONDOWN)
    {
        if (event.button.button == SDL_BUTTON_LEFT)
        {
            state.current.buttons[button_left] = true;
        }
        if (event.button.button == SDL_BUTTON_RIGHT)
        {
            state.current.buttons[button_right] = true;
        }
        if (event.button.button == SDL_BUTTON_MIDDLE)
        {
            state.current.buttons[button_middle] = true;
        }
    }
    if (event.type == SDL_MOUSEBUTTONUP)
    {
        if (event.button.button == SDL_BUTTON_LEFT)
        {
            state.current.buttons[button_left] = false;
        }
        if (event.button.button == SDL_BUTTON_RIGHT)
        {
            state.current.buttons[button_right] = false;
        }
        if (event.button.button == SDL_BUTTON_MIDDLE)
        {
            state.current.buttons[button_middle] = false;
        }
    }

    if (event.type == SDL_MOUSEMOTION)
    {
        state.current.x = (f32) event.motion.x;
        state.current.y = (f32) event.motion.y;
    }

    if (event.type == SDL_MOUSEWHEEL)
    {
        state.current.scroll = (f32) event.wheel.y;
    }
}

bool is_button_down(buttons button)
{
    return state.current.buttons[button];
}

bool is_button_up(buttons button)
{
    return !state.current.buttons[button];
}

bool was_button_down(buttons button)
{
    return !state.current.buttons[button] && state.previous.buttons[button];
}

bool was_button_up(buttons button)
{
    return state.current.buttons[button] && !state.previous.buttons[button];
}

f32 scroll()
{
    return state.current.scroll;
}

point position()
{
    return { state.current.x, state.current.y };
}

point previous_position()
{
    return { state.previous.x, state.previous.y };
}


} // namespace mouse

namespace keyboard
{

namespace
{
struct keyboard_state
{
    bool keys[key::count];
};

struct input_state
{
    keyboard_state current{};
    keyboard_state previous{};
};

input_state state{};

} // anonymous namespace

void update()
{
    state.previous = state.current;
}

void process(SDL_Event& event)
{
    if (event.type == SDL_KEYDOWN)
    {
        state.current.keys[event.key.keysym.scancode] = true;
    }
    if (event.type == SDL_KEYUP)
    {
        state.current.keys[event.key.keysym.scancode] = false;
    }
}

bool is_key_down(key::code key)
{
    return state.current.keys[key];
}
bool is_key_up(key::code key)
{
    return !state.current.keys[key];
}
bool was_key_down(key::code key)
{
    return !state.current.keys[key] && state.previous.keys[key];
}
bool was_key_up(key::code key)
{
    return state.current.keys[key] && !state.previous.keys[key];
}
} // namespace keyboard

} // namespace blaze