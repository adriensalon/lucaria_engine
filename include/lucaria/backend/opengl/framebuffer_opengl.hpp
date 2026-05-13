#pragma once

#include <optional>

#include <lucaria/core/math.hpp>

#include <lucaria/backend/opengl/backend_opengl.hpp>

namespace lucaria {
namespace detail {

    struct framebuffer_implementation_opengl {
        bool is_owning = false;
        GLuint id = 0;
        std::optional<GLuint> texture_color_id = std::nullopt;
        std::optional<GLuint> texture_depth_id = std::nullopt;
        std::optional<GLuint> renderbuffer_color_id = std::nullopt;
        std::optional<GLuint> renderbuffer_depth_id = std::nullopt;
    };

}
}