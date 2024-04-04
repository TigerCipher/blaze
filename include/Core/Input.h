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

#ifndef BLAZE_INPUT_H
#define BLAZE_INPUT_H

#include "Types.h"

union SDL_Event;

namespace blaze
{

namespace mouse
{

struct point
{
    f32 x;
    f32 y;
};

enum buttons
{
    button_left,
    button_right,
    button_middle,

    button_count
};

void update();
void process(SDL_Event& event);

bool is_button_down(buttons button);
bool is_button_up(buttons button);
bool was_button_down(buttons button);
bool was_button_up(buttons button);
f32  scroll();

point position();
point previous_position();


} // namespace mouse

struct key
{
    // From SDL's scancodes. Added here in case a change is made to glfw or some other library for input
    enum code : u16
    {
        a                     = 4,
        b                     = 5,
        c                     = 6,
        d                     = 7,
        e                     = 8,
        f                     = 9,
        g                     = 10,
        h                     = 11,
        i                     = 12,
        j                     = 13,
        k                     = 14,
        l                     = 15,
        m                     = 16,
        n                     = 17,
        o                     = 18,
        p                     = 19,
        q                     = 20,
        r                     = 21,
        s                     = 22,
        t                     = 23,
        u                     = 24,
        v                     = 25,
        w                     = 26,
        x                     = 27,
        y                     = 28,
        z                     = 29,
        one                   = 30,
        two                   = 31,
        three                 = 32,
        four                  = 33,
        five                  = 34,
        six                   = 35,
        seven                 = 36,
        eight                 = 37,
        nine                  = 38,
        zero                  = 39,
        enter                 = 40,
        escape                = 41,
        backspace             = 42,
        tab                   = 43,
        space                 = 44,
        minus                 = 45,
        equals                = 46,
        left_bracket          = 47,
        right_bracket         = 48,
        backslash             = 49,
        non_us_hash           = 50,
        semicolon             = 51,
        apostrophe            = 52,
        grave                 = 53,
        comma                 = 54,
        period                = 55,
        slash                 = 56,
        caps_lock             = 57,
        f1                    = 58,
        f2                    = 59,
        f3                    = 60,
        f4                    = 61,
        f5                    = 62,
        f6                    = 63,
        f7                    = 64,
        f8                    = 65,
        f9                    = 66,
        f10                   = 67,
        f11                   = 68,
        f12                   = 69,
        print_screen          = 70,
        scroll_lock           = 71,
        pause                 = 72,
        insert                = 73,
        home                  = 74,
        page_up               = 75,
        del                   = 76,
        end                   = 77,
        page_down             = 78,
        right                 = 79,
        left                  = 80,
        down                  = 81,
        up                    = 82,
        num_lock              = 83, // "clear" on mac keyboards
        keypad_divide         = 84,
        keypad_multiply       = 85,
        keypad_minus          = 86,
        keypad_plus           = 87,
        keypad_enter          = 88,
        keypad_one            = 89,
        keypad_two            = 90,
        keypad_three          = 91,
        keypad_four           = 92,
        keypad_five           = 93,
        keypad_six            = 94,
        keypad_seven          = 95,
        keypad_eight          = 96,
        keypad_nine           = 97,
        keypad_zero           = 98,
        keypad_period         = 99,
        non_us_backslash      = 100,
        application           = 101, // windows context menu
        power                 = 102, // status flag, but according to usb documentation some mac keyboards have a power key
        keypad_equals         = 103,
        f13                   = 104,
        f14                   = 105,
        f15                   = 106,
        f16                   = 107,
        f17                   = 108,
        f18                   = 109,
        f19                   = 110,
        f20                   = 111,
        f21                   = 112,
        f22                   = 113,
        f23                   = 114,
        f24                   = 115,
        execute               = 116,
        help                  = 117,
        menu                  = 118,
        select                = 119,
        stop                  = 120,
        again                 = 121,
        undo                  = 122,
        cut                   = 123,
        copy                  = 124,
        paste                 = 125,
        find                  = 126,
        mute                  = 127,
        volumeup              = 128,
        volumedown            = 129,
        keypad_comma          = 133,
        keypad_equalsas400    = 134,
        international1        = 135,
        international2        = 136,
        international3        = 137,
        international4        = 138,
        international5        = 139,
        international6        = 140,
        international7        = 141,
        international8        = 142,
        international9        = 143,
        lang1                 = 144,
        lang2                 = 145,
        lang3                 = 146,
        lang4                 = 147,
        lang5                 = 148,
        lang6                 = 149,
        lang7                 = 150,
        lang8                 = 151,
        lang9                 = 152,
        alterase              = 153,
        sysreq                = 154,
        cancel                = 155,
        clear                 = 156,
        prior                 = 157,
        return2               = 158,
        separator             = 159,
        out                   = 160,
        oper                  = 161,
        clearagain            = 162,
        crsel                 = 163,
        exsel                 = 164,
        keypad_00             = 176,
        keypad_000            = 177,
        thousandsseparator    = 178,
        decimalseparator      = 179,
        currencyunit          = 180,
        currencysubunit       = 181,
        keypad_leftparen      = 182,
        keypad_rightparen     = 183,
        keypad_leftbrace      = 184,
        keypad_rightbrace     = 185,
        keypad_tab            = 186,
        keypad_backspace      = 187,
        keypad_a              = 188,
        keypad_b              = 189,
        keypad_c              = 190,
        keypad_d              = 191,
        keypad_e              = 192,
        keypad_f              = 193,
        keypad_xor            = 194,
        keypad_power          = 195,
        keypad_percent        = 196,
        keypad_less           = 197,
        keypad_greater        = 198,
        keypad_ampersand      = 199,
        keypad_dblampersand   = 200,
        keypad_verticalbar    = 201,
        keypad_dblverticalbar = 202,
        keypad_colon          = 203,
        keypad_hash           = 204,
        keypad_space          = 205,
        keypad_at             = 206,
        keypad_exclam         = 207,
        keypad_memstore       = 208,
        keypad_memrecall      = 209,
        keypad_memclear       = 210,
        keypad_memadd         = 211,
        keypad_memsubtract    = 212,
        keypad_memmultiply    = 213,
        keypad_memdivide      = 214,
        keypad_plusminus      = 215,
        keypad_clear          = 216,
        keypad_clearentry     = 217,
        keypad_binary         = 218,
        keypad_octal          = 219,
        keypad_decimal        = 220,
        keypad_hexadecimal    = 221,
        lctrl                 = 224,
        lshift                = 225,
        lalt                  = 226,
        lgui                  = 227, // windows, command (apple), meta
        rctrl                 = 228,
        rshift                = 229,
        ralt                  = 230,
        rgui                  = 231, // windows, command (apple), meta
        mode                  = 257,
        audionext             = 258,
        audioprev             = 259,
        audiostop             = 260,
        audioplay             = 261,
        audiomute             = 262,
        mediaselect           = 263,
        www                   = 264,
        mail                  = 265,
        calculator            = 266,
        computer              = 267,
        ac_search             = 268,
        ac_home               = 269,
        ac_back               = 270,
        ac_forward            = 271,
        ac_stop               = 272,
        ac_refresh            = 273,
        ac_bookmarks          = 274,
        brightnessdown        = 275,
        brightnessup          = 276,
        displayswitch         = 277,
        kbdillumtoggle        = 278,
        kbdillumdown          = 279,
        kbdillumup            = 280,
        eject                 = 281,
        sleep                 = 282,
        app1                  = 283,
        app2                  = 284,
        audiorewind           = 285,
        audiofastforward      = 286,
        softleft              = 287,
        softright             = 288,
        call                  = 289,
        endcall               = 290,

        count, // = 512
    };
};

namespace keyboard
{

void update();
void process(SDL_Event& event);

bool is_key_down(key::code key);
bool is_key_up(key::code key);
bool was_key_down(key::code key);
bool was_key_up(key::code key);


} // namespace keyboard


} // namespace blaze

#endif //BLAZE_INPUT_H
