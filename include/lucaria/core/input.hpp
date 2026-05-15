#pragma once

#include <unordered_map>

#include <lucaria/core/key.hpp>
#include <lucaria/core/math.hpp>

namespace lucaria {
namespace detail {

    struct key_event {
        bool state = false;
        bool is_down = false;
        bool is_up = false;
    };

    struct pointer_event {
        float32x2 position = float32x2(0);
        float32x2 delta = float32x2(0);
        bool is_down = false;
        bool is_up = false;
    };

    [[nodiscard]] bool get_is_keyboard_supported();
    [[nodiscard]] bool get_is_mouse_supported();
    [[nodiscard]] bool get_is_touch_supported();
    [[nodiscard]] std::unordered_map<input_key, key_event>& get_buttons();
    [[nodiscard]] std::unordered_map<glm::uint, pointer_event>& get_pointers();
    [[nodiscard]] float32x2 get_mouse_position();
    [[nodiscard]] float32x2& get_mouse_position_delta();

}
}
