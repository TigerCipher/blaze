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
gfx::cube<gfx::vertex_position_normal_texcoords>
    box(1.0f); // TODO: Probably more efficient to use this for the lamp too and just ignore the normal
gfx::cube<gfx::vertex_position>     light_cube(0.2f);
camera                              cam{};
gfx::light                          light{};
gfx::material                       box_material{};
std::vector<uptr<gfx::render_item>> render_items;

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

    container.bind();
    container_specular.bind(1);


    test.bind();
    test.set_mat4("view", cam.view_matrix());
    test.set_mat4("projection", cam.projection());

    test.set_vec3("viewPos", cam.position());
    light.diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    light.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    gfx::bind_light(test, light);
    //    gfx::bind_material(test, box_material);

    glm::mat4 model = glm::mat4(1.0f);
    render_items[0]->set_model(model);

    for (const auto& item : render_items)
    {
        item->draw(test);
    }

    light_cube_shader.bind();
    light_cube_shader.set_mat4("view", cam.view_matrix());
    light_cube_shader.set_mat4("projection", cam.projection());
    model = glm::translate(model, light.position);
    light_cube_shader.set_mat4("model", model);
    light_cube.draw();
}

void init_sandbox()
{
    if (!test.load() || !light_cube_shader.load())
    {
        return;
    }

    if (!container.load(true) || !face.load(true) || !container_specular.load(true))
    {
        return;
    }
    cam.set_position({ 0.f, 0.f, 3.f });

    box.create(test);
    light_cube.create(light_cube_shader);

    light.position = glm::vec3(1.2f, 1.0f, 2.0f);
    light.ambient  = glm::vec3(0.2f, 0.2f, 0.2f);
    light.diffuse  = glm::vec3(0.5f, 0.5f, 0.5f);
    light.specular = glm::vec3(1.0f, 1.0f, 1.0f);


    // TODO: Generally, diffuse, specular, etc textures will always be at the same slots
    // Instead, material should have a texture, and there can be multiple materials. I.e a wood material for the box
    // The bind function would activate the texture slots, bind the material data, and bind the textures
    // Something like:
    // void bind_material(const shader& shader, const material& mat)
    //    mat.diffuse_texture.bind();
    //    mat.specular_texture.bind(1);
    //    shader.bind();
    //    shader.set_int("material.diffuse", 0); // these really only need to be bound at init, since the data will never change
    //    shader.set_int("material.specular", 1);
    //    shader.set_float("material.shininess", mat.shininess);
    // In the long run, use would look something like:
    // material mat;
    // mat.diffuse_texture = container;
    // mat.specular_texture = container_specular;
    // mat.shininess = 32.0f;
    // render_item item;
    // item.material = mat;
    // item.mesh = box;
    // item.transform = glm::mat4(1.0f);
    // render_items.push_back(item);
    // Then in the render loop:
    // for (const auto& item : render_items)
    // {
    //     bind_material(item.material);
    //     item.mesh.draw();
    // }
    // TODO: The above, or something similar, should be considered
    box_material.diffuse   = &container;
    box_material.specular  = &container_specular;
    box_material.shininess = 32.0f;

    test.bind();
    test.set_int("material.diffuse", 0);
    test.set_int("material.specular", 1);
    // unbind here since we change gl state when we clear the screen (otherwise we get performance warnings)
    gfx::shader::unbind();


    render_items.push_back(make_uptr<gfx::render_item>(&box, box_material));

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
    box.destroy();
    render_items.clear();

    blaze::shutdown();
    LOG_INFO("Sandbox ended");

    return 0;
}