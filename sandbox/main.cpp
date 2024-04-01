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

#include <iostream>
#include "Blaze.h"
#include "Graphics/GLCore.h"

void render()
{
    blaze::gfx::clear_screen(0.2f, 0.f, 0.f);

    blaze::gfx::activate_window("Test");
    blaze::gfx::clear_screen(0.f, 0.2f, 0.f);

    blaze::gfx::activate_window("Test2");
    blaze::gfx::clear_screen(0.f, 0.f, 0.2f);
}

int main()
{
    std::cout << "Hello, World!" << std::endl;
    if (blaze::init())
    {
        std::cout << "Sandbox can run now" << std::endl;
    } else
    {
        std::cout << "Blaze failed to initialize!" << std::endl;
    }

    if (blaze::create_window("Sandbox", 1280, 720) && blaze::create_window("Test", 400, 400) &&
        blaze::create_window("Test2", 400, 400))
    {
        blaze::set_render_function(render);
        blaze::run();
    }

    blaze::shutdown();

    return 0;
}