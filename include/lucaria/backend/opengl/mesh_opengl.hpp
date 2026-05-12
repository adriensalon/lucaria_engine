#pragma once

#include <lucaria/core/math.hpp>

namespace lucaria {
namespace detail {

    enum struct mesh_attribute;

    struct mesh_implementation_opengl {
        bool is_owning;
        glm::uint size;
        glm::uint array_id;
        glm::uint elements_id;
        std::unordered_map<mesh_attribute, glm::uint> attribute_ids;
        std::vector<glm::mat4> invposes;
    };

}
}