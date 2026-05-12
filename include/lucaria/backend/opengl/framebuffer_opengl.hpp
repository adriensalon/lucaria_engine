#pragma once

#include <optional>

#include <lucaria/core/math.hpp>

namespace lucaria {
namespace detail {

	struct framebuffer_implementation_opengl {
		bool is_owning = false;
        glm::uint id = 0;
		std::optional<glm::uint> texture_color_id = std::nullopt;
        std::optional<glm::uint> texture_depth_id = std::nullopt;
        std::optional<glm::uint> renderbuffer_color_id = std::nullopt;
        std::optional<glm::uint> renderbuffer_depth_id = std::nullopt;
	};

}
}