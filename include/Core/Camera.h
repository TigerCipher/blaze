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

#ifndef BLAZE_CAMERA_H
#define BLAZE_CAMERA_H

#include "Types.h"
#include <glm/glm.hpp>

namespace blaze
{

enum class direction
{
    forward,
    backward,
    left,
    right
};

class camera
{
public:
    camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), f32 yaw = default_yaw,
           f32 pitch = default_pitch);

    glm::mat4     view_matrix() const;
    constexpr f32 zoom() const { return m_zoom; }

    void process_keyboard(direction dir, f64 delta_time);
    void process_mouse_movement(f32 x_offset, f32 y_offset, bool constrain_pitch = true);
    void process_mouse_scroll(f32 y_offset);
    void set_projection(f32 fov, f32 aspect_ratio, f32 near, f32 far);

    void set_position(const glm::vec3& position) { m_position = position; }
    void set_movement_speed(f32 speed) { m_movement_speed = speed; }
    void set_mouse_sensitivity(f32 sensitivity) { m_mouse_sensitivity = sensitivity; }
    void set_zoom(f32 zoom) { m_zoom = zoom; }


    constexpr f32 yaw() const { return m_yaw; }
    constexpr f32 pitch() const { return m_pitch; }
    constexpr f32 movement_speed() const { return m_movement_speed; }
    constexpr f32 mouse_sensitivity() const { return m_mouse_sensitivity; }

    constexpr const glm::mat4& projection() const { return m_projection; }

    constexpr const glm::vec3& position() const { return m_position; }
    constexpr const glm::vec3& front() const { return m_front; }
    constexpr const glm::vec3& up() const { return m_up; }
    constexpr const glm::vec3& right() const { return m_right; }

private:
    constexpr static f32 default_yaw         = -90.0f;
    constexpr static f32 default_pitch       = 0.0f;
    constexpr static f32 default_speed       = 2.5f;
    constexpr static f32 default_sensitivity = 0.1f;
    constexpr static f32 default_zoom        = 45.0f;

    glm::vec3 m_position{ 0.0f, 0.0f, 0.0f };
    glm::vec3 m_front{ 0.0f, 0.0f, -1.0f };
    glm::vec3 m_up{ 0.0f, 1.0f, 0.0f };
    glm::vec3 m_right{ 1.0f, 0.0f, 0.0f };
    glm::vec3 m_world_up{ 0.0f, 1.0f, 0.0f };
    glm::mat4 m_projection{};

    f32 m_yaw               = default_yaw;
    f32 m_pitch             = default_pitch;
    f32 m_movement_speed    = default_speed;
    f32 m_mouse_sensitivity = default_sensitivity;
    f32 m_zoom              = default_zoom;

    bool update_needed = true;

    void update_vectors();
};

} // namespace blaze

#endif //BLAZE_CAMERA_H
