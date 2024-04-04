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

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace blaze;

namespace
{
gfx::shader  test{ "transform" };
gfx::texture container{ "container.jpg" };
gfx::texture face{ "face.png" };
u32          vao;

struct vertex
{
    glm::vec3 position;
    glm::vec2 tex_coords;
};

} // anonymous namespace

void render()
{
    blaze::gfx::clear_screen(0.2f, 0.f, 0.f);

    gfx::texture::activate_slot(0);
    container.bind();
    gfx::texture::activate_slot(1);
    face.bind();

    glm::mat4 transform = glm::mat4(1.0f);
    transform           = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
    transform           = glm::rotate(transform, get_time(), glm::vec3(0.0f, 0.0f, 1.0f));


    test.bind();
    test.set_mat4("transform", transform);

    glBindVertexArray(vao);
    //    GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 3));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

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

    u32    vbo, ebo;
    vertex vertices[] = {
            // positions            // texture coords
        {   { 0.5f, 0.5f, 0.0f }, { 1.0f, 1.0f } }, // top right
        {  { 0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f } }, // bottom right
        { { -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f } }, // bottom left
        {  { -0.5f, 0.5f, 0.0f }, { 0.0f, 1.0f } }  // top left
    };

    u32 indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // TODO: Automate the pointer somehow. Maybe store the offset in the attribute struct, and use the previous attribute's offset to get the pointer
    test.bind_attribute("aPos", (void*) nullptr);
    test.bind_attribute("aTexCoord", (void*) offsetof(vertex, tex_coords));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //    glBindVertexArray(0); -> causes medium severity debug msg: "Program/shader state performance warning: Vertex shader in program 3 is being recompiled based on GL state"

    test.bind();
    test.set_int("texture1", 0);
    test.set_int("texture2", 1);
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

    blaze::shutdown();
    LOG_INFO("Sandbox ended");

    return 0;
}