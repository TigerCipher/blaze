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
#include "Core/Input.h"
#include "Core/Camera.h"

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace blaze;

namespace
{
constexpr i32 window_width  = 1280;
constexpr i32 window_height = 720;

gfx::shader                            test{ "lighting" };
gfx::shader                            light_cube_shader{ "light_cube" };
gfx::texture                           container{ "container.jpg" };
gfx::texture                           face{ "face.png" };
gfx::cube<gfx::vertex_position_normal> box(1.0f); // TODO: Probably more efficient to use this for the lamp too and just ignore the normal
gfx::cube<gfx::vertex_position>        light_cube(0.2f);
camera                                 cam{};

} // anonymous namespace

void update(f32 delta)
{
    if (keyboard::is_key_down(key::escape))
    {
        blaze::exit_now();
    }
    if (keyboard::is_key_down(key::w))
    {
        cam.process_keyboard(direction::forward, delta);
    }
    if (keyboard::is_key_down(key::s))
    {
        cam.process_keyboard(direction::backward, delta);
    }
    if (keyboard::is_key_down(key::a))
    {
        cam.process_keyboard(direction::left, delta);
    }
    if (keyboard::is_key_down(key::d))
    {
        cam.process_keyboard(direction::right, delta);
    }
}

void render(f32 delta)
{
    blaze::gfx::clear_screen(0.2f, 0.f, 0.f);

    gfx::texture::activate_slot(0);
    container.bind();
    gfx::texture::activate_slot(1);
    face.bind();


    test.bind();
    test.set_mat4("view", cam.view_matrix());
    test.set_mat4("projection", cam.projection());
    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
    test.set_vec3("lightPos", lightPos);
    test.set_vec3("viewPos", cam.position());

    glm::mat4 model = glm::mat4(1.0f);
    test.set_mat4("model", model);
    box.draw();

    light_cube_shader.bind();
    light_cube_shader.set_mat4("view", cam.view_matrix());
    light_cube_shader.set_mat4("projection", cam.projection());
    model = glm::translate(model, lightPos);
    light_cube_shader.set_mat4("model", model);
    light_cube.draw();
}

void init_sandbox()
{
    if (!test.load() || !light_cube_shader.load())
    {
        return;
    }

    if (!container.load(true) || !face.load(true))
    {
        return;
    }
    cam.set_position({ 0.f, 0.f, 3.f });

    box.create(test);
    light_cube.create(light_cube_shader);

    test.bind();
    test.set_vec3("objectColor", glm::vec3(1.0f, 0.5f, 0.3f));
    test.set_vec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    //    test.set_int("texture1", 0);
    //    test.set_int("texture2", 1);
    gfx::shader::unbind();

    mouse::lock_cursor(true);
}

int main()
{
    LOG_INFO("Sandbox started");
    if (blaze::init(&cam, render, update))
    {
        std::cout << "Sandbox can run now" << std::endl;
    } else
    {
        std::cout << "Blaze failed to initialize!" << std::endl;
    }

    if (blaze::create_window("Sandbox", window_width, window_height))
    {
        init_sandbox();
        blaze::run();
    }

    test.destroy();
    box.destroy();

    blaze::shutdown();
    LOG_INFO("Sandbox ended");

    return 0;
}