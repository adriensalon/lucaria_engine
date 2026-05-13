#pragma once

#include <unordered_map>

#include <lucaria/core/math.hpp>

#include <lucaria/backend/opengl/backend_opengl.hpp>

namespace lucaria {
namespace detail {

    enum struct mesh_attribute;

    struct mesh_implementation_opengl {
        bool is_owning = false;
        GLuint array_id = 0;
        GLuint elements_id = 0;
        std::unordered_map<mesh_attribute, GLuint> attribute_ids = {};
    };

}
}