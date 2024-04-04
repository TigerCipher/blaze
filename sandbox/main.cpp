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
#include "Graphics/Shader.h"
#include "Graphics/Texture.h"
#include "Graphics/Primitives.h"

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace blaze;

namespace
{
gfx::shader  test{ "coords" };
gfx::texture container{ "container.jpg" };
gfx::texture face{ "face.png" };
gfx::cube    box{ 1.0f };
glm::mat4    projection;


} // anonymous namespace

void render()
{
    blaze::gfx::clear_screen(0.2f, 0.f, 0.f);

    gfx::texture::activate_slot(0);
    container.bind();
    gfx::texture::activate_slot(1);
    face.bind();

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view  = glm::mat4(1.0f);
    model           = glm::rotate(model, get_time(), glm::vec3(0.5f, 1.0f, 0.0f));
    view            = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));


    test.bind();
    test.set_mat4("model", model);
    test.set_mat4("view", view);
    test.set_mat4("projection", projection);

    box.draw();

    //    blaze::gfx::activate_window("Test");
    //    blaze::gfx::clear_screen(0.f, 0.2f, 0.f);
    //
    //    blaze::gfx::activate_window("Test2");
    //    blaze::gfx::clear_screen(0.f, 0.f, 0.2f);
}

void init_sandbox()
{
    if (!test.load())
    {
        return;
    }

    if (!container.load(true) || !face.load(true))
    {
        return;
    }

    box.create(test);

    test.bind();
    test.set_int("texture1", 0);
    test.set_int("texture2", 1);

    // TODO: Projection based on current window size
    projection = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);
}

int main()
{
    LOG_INFO("Sandbox started");
    if (blaze::init())
    {
        std::cout << "Sandbox can run now" << std::endl;
    } else
    {
        std::cout << "Blaze failed to initialize!" << std::endl;
    }

    if (blaze::create_window("Sandbox", 1280, 720))
    {
        init_sandbox();
        blaze::set_render_function(render);
        blaze::run();
    }

    test.destroy();
    box.destroy();

    blaze::shutdown();
    LOG_INFO("Sandbox ended");

    return 0;
}