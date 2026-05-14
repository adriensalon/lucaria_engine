#pragma once

#include <string>
#include <unordered_map>

#include <lucaria/core/math.hpp>

#include <lucaria/core/backend/opengl/backend_opengl.hpp>

namespace lucaria {
namespace detail {

    struct program_implementation_opengl {
        bool is_owning = false;
        GLuint id = 0;
        std::unordered_map<std::string, GLint> reflected_attributes = {};
        std::unordered_map<std::string, GLint> reflected_uniforms = {};
        GLuint bound_array_id = 0;
        GLuint bound_indices_count = 0;
    };

}
}