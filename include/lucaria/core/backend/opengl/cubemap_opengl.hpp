#pragma once

#include <lucaria/core/math.hpp>

#include <lucaria/core/backend/opengl/backend_opengl.hpp>

namespace lucaria {
namespace detail {

    struct cubemap_implementation_opengl {
        bool is_owning = false;
        GLuint id = 0;
    };

}
}