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
#include "Core/Logger.h"

namespace blaze::logger::detail
{

namespace
{
std::string current_time_stamp()
{
    time_t now = time(nullptr);
    tm*    ltm = localtime(&now);
    return std::format("{:02}:{:02}:{:02}", ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
}
} // anonymous namespace
void output(log_level lvl, std::string_view msg)
{
    std::string str;
    std::string time_stamp = current_time_stamp();
    switch (lvl)
    {
    case log_level::trace: str = std::format("[{}][  TRACE  ]: {}\n", time_stamp, msg); break;
    case log_level::debug: str = std::format("[{}][  DEBUG  ]: {}\n", time_stamp, msg); break;
    case log_level::info: str = std::format("[{}][  INFO   ]: {}\n", time_stamp, msg); break;
    case log_level::warn: str = std::format("[{}][ WARNING ]: {}\n", time_stamp, msg); break;
    case log_level::error: str = std::format("[{}][  ERROR  ]: {}\n", time_stamp, msg); break;
    case log_level::fatal: str = std::format("[{}][  FATAL  ]: {}\n", time_stamp, msg); break;
    }

    printf("%s", str.c_str());
}
} // namespace blaze::logger::detail