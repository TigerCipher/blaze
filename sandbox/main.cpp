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
#include "Graphics/Material.h"
#include "Graphics/Light.h"
#include "Graphics/RenderItem.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace blaze;

namespace
{
constexpr i32 window_width  = 1280;
constexpr i32 window_height = 720;

gfx::shader  test{ "lighting" };
gfx::shader  light_cube_shader{ "light_cube" };
gfx::texture container{ "container2.png" };
gfx::texture container_specular{ "container2_specular.png" };
gfx::texture face{ "face.png" };
gfx::texture matrix{ "matrix.jpg" };

camera                        cam{};
gfx::directional_light        light{};
gfx::spot_light               spotlight{};
std::vector<gfx::point_light> point_lights{};
gfx::material                 box_material{};
gfx::material                 no_material{};

uptr<gfx::render_item> box_item{};
uptr<gfx::render_item> light_item{};

} // anonymous namespace

void update(f64 delta)
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
    if (keyboard::is_key_down(key::space))
    {
        cam.set_position({ 0.f, 0.f, 3.f });
    }
    if (keyboard::is_key_down(key::lshift))
    {
        cam.set_movement_speed(8.0f);
    } else
    {
        cam.set_movement_speed(2.5f);
    }
    if (keyboard::was_key_up(key::f))
    {
        if (spotlight.diffuse == glm::vec3(0.f, 0.f, 0.f))
        {
            spotlight.diffuse = glm::vec3(1.f, 1.f, 1.f);
        } else
        {
            spotlight.diffuse = { 0.f, 0.f, 0.f };
        }
    }
}

void render(f64 delta)
{
    blaze::gfx::clear_screen(0.f, 0.f, 0.f);

    test.bind();
    test.set_mat4("view", cam.view_matrix());
    test.set_mat4("projection", cam.projection());

    test.set_vec3("viewPos", cam.position());

    gfx::bind_light(test, light);
    spotlight.position  = cam.position();
    spotlight.direction = cam.front();
    gfx::bind_light(test, spotlight);
    for (u32 i = 0; i < point_lights.size(); ++i)
    {
        gfx::bind_light(test, point_lights[i], i);
    }
    //    gfx::bind_material(test, box_material);

    //    glm::mat4 model = glm::mat4(1.0f);
    //    model = glm::rotate(model, get_time(), glm::vec3(1.0f, 0.0f, 1.0f));
    //    box_item->set_model(model);
    //    box_item->draw(test);

    static glm::vec3 positions[] = { glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
                                     glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
                                     glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
                                     glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
                                     glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f) };

    for (u32 i = 0; i < 10; ++i)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model           = glm::translate(model, positions[i]);
        f32 angle       = 20.0f * (f32) i;
        model           = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        box_item->set_model(model);
        box_item->draw(test);
    }

    glm::mat4 model = glm::mat4(1.0f);
    light_cube_shader.bind();
    light_cube_shader.set_mat4("view", cam.view_matrix());
    light_cube_shader.set_mat4("projection", cam.projection());
    for (const auto& pl : point_lights)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, pl.position);
        model = glm::scale(model, glm::vec3(0.2f));
        light_cube_shader.set_vec3("color", pl.ambient);
        light_item->set_model(model);
        light_item->draw(light_cube_shader);
    }
}

void init_sandbox()
{
    if (!test.load() || !light_cube_shader.load())
    {
        return;
    }

    if (!container.load(true) || !container_specular.load(true))
    {
        return;
    }
    cam.set_position({ 0.f, 0.f, 3.f });

    sptr<gfx::cube<gfx::vertex_position_normal_texcoords>> box =
        make_sptr<gfx::cube<gfx::vertex_position_normal_texcoords>>(1.0f);
    box->create();

    // Bit ugly, but this is a way to have light_cube share the vbo with box, which is more efficient
    sptr<gfx::cube<gfx::vertex_position>> light_cube = make_sptr<gfx::cube<gfx::vertex_position>>(box);
    light_cube->create();
    light_item = make_uptr<gfx::render_item>(light_cube, no_material);

    // directional light
    light.direction = glm::vec3(-0.2f, -1.0f, -0.3f);
    light.ambient   = glm::vec3(0.1f, 0.1f, 0.1f);
    light.diffuse   = glm::vec3(0.5f, 0.5f, 0.5f);
    light.specular  = glm::vec3(1.0f, 1.0f, 1.0f);

    // spotlight
    spotlight.position     = cam.position();
    spotlight.direction    = cam.front();
    spotlight.ambient      = glm::vec3(0.0f, 0.0f, 0.0f);
    spotlight.diffuse      = glm::vec3(1.0f, 1.0f, 1.0f);
    spotlight.specular     = glm::vec3(1.0f, 1.0f, 1.0f);
    spotlight.constant     = 1.0f;
    spotlight.linear       = 0.09f;
    spotlight.quadratic    = 0.032f;
    spotlight.cutoff       = glm::cos(glm::radians(12.5f));
    spotlight.outer_cutoff = glm::cos(glm::radians(17.5f));

    // point lights
    gfx::point_light pl, pl2, pl3, pl4;
    pl.position  = glm::vec3(0.7f, 0.2f, 2.0f);
    pl.ambient   = glm::vec3(0.05f, 0.75f, 0.05f);
    pl.diffuse   = glm::vec3(0.8f, 0.8f, 0.8f);
    pl.specular  = glm::vec3(1.0f, 1.0f, 1.0f);
    pl.constant  = 1.0f;
    pl.linear    = 0.09f;
    pl.quadratic = 0.032f;
    point_lights.push_back(pl);
    pl2.position  = glm::vec3(2.3f, -3.3f, -4.0f);
    pl2.ambient   = glm::vec3(0.75f, 0.05f, 0.75f);
    pl2.diffuse   = glm::vec3(0.8f, 0.8f, 0.8f);
    pl2.specular  = glm::vec3(1.0f, 1.0f, 1.0f);
    pl2.constant  = 1.0f;
    pl2.linear    = 0.09f;
    pl2.quadratic = 0.032f;
    point_lights.push_back(pl2);
    pl3.position  = glm::vec3(-4.0f, 2.0f, -12.0f);
    pl3.ambient   = glm::vec3(0.05f, 0.95f, 0.55f);
    pl3.diffuse   = glm::vec3(0.8f, 0.8f, 0.8f);
    pl3.specular  = glm::vec3(1.0f, 1.0f, 1.0f);
    pl3.constant  = 1.0f;
    pl3.linear    = 0.09f;
    pl3.quadratic = 0.032f;
    point_lights.push_back(pl3);
    pl4.position  = glm::vec3(0.0f, 0.0f, -3.0f);
    pl4.ambient   = glm::vec3(0.95f, 0.05f, 0.05f);
    pl4.diffuse   = glm::vec3(0.8f, 0.8f, 0.8f);
    pl4.specular  = glm::vec3(1.0f, 1.0f, 1.0f);
    pl4.constant  = 1.0f;
    pl4.linear    = 0.09f;
    pl4.quadratic = 0.032f;
    point_lights.push_back(pl4);


    box_material.diffuse   = &container;
    box_material.specular  = &container_specular;
    box_material.shininess = 32.0f;

    test.bind();
    test.set_int("material.diffuse", 0);
    test.set_int("material.specular", 1);
    // unbind here since we change gl state when we clear the screen (otherwise we get performance warnings)
    gfx::shader::unbind();


    box_item = make_uptr<gfx::render_item>(box, box_material);

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
    light_cube_shader.destroy();
    light_item->destroy();
    box_item->destroy();

    blaze::shutdown();
    LOG_INFO("Sandbox ended");

    return 0;
}