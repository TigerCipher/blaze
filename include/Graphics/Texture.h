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

#ifndef BLAZE_TEXTURE_H
#define BLAZE_TEXTURE_H

#include <utility>

#include "Types.h"

namespace blaze::gfx
{
class texture
{
public:
    texture(std::string  name) : m_name(std::move(name)) {}
    ~texture();

    bool load(bool flip = true);
    void unload();

    void bind() const;

    constexpr u32 id() const { return m_id; }
    constexpr i32 width() const { return m_width; }
    constexpr i32 height() const { return m_height; }

    static void activate_slot(u32 slot);

private:
    u32 m_id{ u32_invalid_id };
    i32 m_width{};
    i32 m_height{};
    std::string m_name;
};
} // namespace blaze::gfx

#endif //BLAZE_TEXTURE_H
