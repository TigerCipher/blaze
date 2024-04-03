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
#include "Graphics/GLCore.h"
#include "Graphics/Shader.h"
#include "Graphics/Texture.h"

#include <gl/glew.h>
#include <iostream>

// a bit useless with the error callback, but can be helpful for tracking down errors
#define GL_CALL(x)                                                                                                               \
    x;                                                                                                                           \
    {                                                                                                                            \
        GLenum error = glGetError();                                                                                             \
        if (error != GL_NO_ERROR)                                                                                                \
        {                                                                                                                        \
            std::cerr << "OpenGL Error: " << error << ", " << get_error_string(error) << std::endl;                              \
            std::cerr << "---- in file: " << __FILE__ << " at line " << __LINE__ << std::endl;                                   \
        }                                                                                                                        \
    }

namespace blaze::gfx
{

namespace
{
bool is_init = false;
shader test{"texture"};
texture container{"container.jpg"};
texture face{"face.png"};
u32 vao;

const char* get_error_string(GLenum error)
{
    switch (error)
    {
    case GL_INVALID_ENUM: return "GL_INVALID_ENUM";
    case GL_INVALID_VALUE: return "GL_INVALID_VALUE";
    case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";
    case GL_STACK_OVERFLOW: return "GL_STACK_OVERFLOW";
    case GL_STACK_UNDERFLOW: return "GL_STACK_UNDERFLOW";
    case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY";
    case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
    default: return "Unknown error";
    }
}


void GLAPIENTRY error_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message,
                               const void* user_param)
{
    return;
    std::cerr << "---------------------opengl-error-callback-start------------" << std::endl;
    std::cerr << "Severity: " << severity << std::endl;
    std::cerr << "OpenGL Error: " << message << std::endl;
    std::cerr << "Use GL_CALL macro to help locate the error" << std::endl;
    std::cerr << "---------------------opengl-error-callback-end--------------" << std::endl;
}

} // anonymous namespace

bool init()
{
    if (is_init)
    {
        return false;
    }
    glewExperimental = true;
    u32 status       = glewInit();
    if (status != GLEW_OK)
    {
        std::cerr << "GLEW failed to initialize: " << glewGetErrorString(status) << std::endl;
        return false;
    }

    glGetError(); // Clear any errors that glewInit may have caused
#ifdef _DEBUG
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(error_callback, nullptr);
#endif

    if(!test.load())
    {
        return false;
    }

    if(!container.load(true) || !face.load(true))
    {
        return false;
    }

    u32 vbo, ebo;
    f32 vertices[] = {
        // positions          // colors           // texture coords
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };

    u32 indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3 // second triangle
    };
    GL_CALL(glGenVertexArrays(1, &vao));
    GL_CALL(glGenBuffers(1, &vbo));
    GL_CALL(glGenBuffers(1, &ebo));

    GL_CALL(glBindVertexArray(vao));

    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));
    GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

    GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)nullptr));
    GL_CALL(glEnableVertexAttribArray(0));

    GL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)(3 * sizeof(f32))));
    GL_CALL(glEnableVertexAttribArray(1));

    GL_CALL(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)(6 * sizeof(f32))));
    GL_CALL(glEnableVertexAttribArray(2));

    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GL_CALL(glBindVertexArray(0));

    test.bind();
    test.set_int("texture1", 0);
    test.set_int("texture2", 1);

    is_init = true;
    return true;
}

void clear_screen(f32 r, f32 g, f32 b)
{
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void test_shader() {
    texture::activate_slot(0);
    container.bind();
    texture::activate_slot(1);
    face.bind();

    test.bind();
//    test.set_float("red", 0.5f);
//    test.set_float("green", 0.2f);
//    test.set_float("blue", 0.8f);
    GL_CALL(glBindVertexArray(vao));
//    GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 3));
    GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
}

} // namespace blaze::gfx