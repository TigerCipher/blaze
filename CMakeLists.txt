cmake_minimum_required(VERSION 3.27)
project(blaze)

set(CMAKE_CXX_STANDARD 20)

add_library(blaze STATIC
        include/Blaze.h
        src/Blaze.cpp
        include/Core/Window.h
        include/Types.h
        src/Core/Window.cpp
        include/Graphics/GLCore.h
        src/Graphics/GLCore.cpp
        include/Core/Logger.h
        src/Core/Logger.cpp
        include/Graphics/Shader.h
        src/Graphics/Shader.cpp
        include/Graphics/Texture.h
        src/Graphics/Texture.cpp
)

target_include_directories(blaze PUBLIC include)

find_package(GLEW REQUIRED)
target_link_libraries(blaze PRIVATE GLEW::GLEW)

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(blaze
        PRIVATE
        $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)

find_package(soil2 CONFIG REQUIRED)
target_link_libraries(blaze PRIVATE soil2)

find_package(glm CONFIG REQUIRED)
target_link_libraries(blaze PUBLIC glm::glm)

add_subdirectory(sandbox)