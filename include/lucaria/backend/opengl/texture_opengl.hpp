#pragma once

#include <lucaria/core/math.hpp>

namespace lucaria {
namespace detail {

	struct texture_implementation_opengl {
		bool is_owning;
        glm::uvec2 size;
        glm::uint id;
	};

}
}