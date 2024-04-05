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
#include "Core/Camera.h"
#include "glm/ext/matrix_clip_space.hpp"
#include <glm/ext/matrix_transform.hpp>

namespace blaze
{

camera::camera(glm::vec3 position, glm::vec3 up, f32 yaw, f32 pitch) :
    m_position(position), m_world_up(up), m_yaw(yaw), m_pitch(pitch)
{
    update_vectors();
}

glm::mat4 camera::view_matrix() const
{
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

void camera::process_keyboard(direction dir, f32 delta_time)
{
    f32 velocity = m_movement_speed * delta_time;
    if (dir == direction::forward)
    {
        m_position += m_front * velocity;
    }
    if (dir == direction::backward)
    {
        m_position -= m_front * velocity;
    }
    if (dir == direction::left)
    {
        m_position -= m_right * velocity;
    }
    if (dir == direction::right)
    {
        m_position += m_right * velocity;
    }
}

void camera::process_mouse_movement(f32 x_offset, f32 y_offset, bool constrain_pitch)
{
    x_offset *= m_mouse_sensitivity;
    y_offset *= m_mouse_sensitivity;

    m_yaw += x_offset;
    m_pitch += y_offset;

    if (constrain_pitch)
    {
        if (m_pitch > 89.0f)
        {
            m_pitch = 89.0f;
        }
        if (m_pitch < -89.0f)
        {
            m_pitch = -89.0f;
        }
    } else
    {
        if (m_pitch > 360.0f)
        {
            m_pitch -= 360.0f;
        }
        if (m_pitch < -360.0f)
        {
            m_pitch += 360.0f;
        }
    }

    if (m_yaw > 360.0f)
    {
        m_yaw -= 360.0f;
    }
    if (m_yaw < -360.0f)
    {
        m_yaw += 360.0f;
    }

    update_vectors();
}

void camera::process_mouse_scroll(f32 y_offset)
{
    constexpr f32 max_zoom = 45.0f;
    f32           old_zoom = m_zoom;
    m_zoom -= y_offset;
    if (m_zoom < 1.0f)
    {
        m_zoom = 1.0f;
    }
    if (m_zoom > max_zoom)
    {
        m_zoom = max_zoom;
    }
}

void camera::update_vectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(front);
    m_right = glm::normalize(glm::cross(m_front, m_world_up));
    m_up    = glm::normalize(glm::cross(m_right, m_front));
}

void camera::set_projection(f32 fov, f32 aspect_ratio, f32 near, f32 far)
{
    m_projection = glm::perspective(glm::radians(fov), aspect_ratio, near, far);
}


} // namespace blaze