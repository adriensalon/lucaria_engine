#pragma once

#include <lucaria/core/platform/glfw/key_glfw.hpp>

namespace lucaria {
namespace detail {

    struct window_implementation_glfw {
        GLFWwindow* window = nullptr;
        bool is_mouse_locked = false;
        bool must_install_imgui_callbacks = true;
    };

}
}