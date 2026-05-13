#pragma once

#include <lucaria/core/math.hpp>

#include <lucaria/backend/opengl/backend_opengl.hpp>

namespace lucaria {
namespace detail {

    struct renderbuffer_implementation_opengl {
        bool is_owning = false;
        GLuint id = 0;
        GLuint internal_format = 0;
    };

}
}