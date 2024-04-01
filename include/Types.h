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

#ifndef BLAZE_TYPES_H
#define BLAZE_TYPES_H

#include <cstdint>
#include <memory>
#include <string>


using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using f32 = float;
using f64 = double;

// may go unused, we'll see
using string_hash = std::hash<std::string>;

// Useful concepts
template<typename T, typename... Args>
concept constructible_from_args = std::constructible_from<T, Args...>;

template<typename T>
concept default_constructible = std::default_initializable<T>;

template<typename T>
concept primitive_type = std::is_arithmetic_v<T>;

// smart pointers. For now just convenience aliases and functions to avoid std:: everywhere. Might use custom smart pointers later.
template<typename T>
using uptr = std::unique_ptr<T>;

template<typename T>
using sptr = std::shared_ptr<T>;

template<typename T, typename... Args>
    requires constructible_from_args<T, Args...>
constexpr uptr<T> make_uptr(Args&&... args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T, typename... Args>
    requires constructible_from_args<T, Args...>
constexpr sptr<T> make_sptr(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}


#endif //BLAZE_TYPES_H
