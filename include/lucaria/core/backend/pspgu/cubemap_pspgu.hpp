#pragma once

#include <array>

#include <lucaria/core/backend/pspgu/texture_pspgu.hpp>

namespace lucaria {
namespace detail {

	struct cubemap_implementation_pspgu {
		bool is_owning = false;
		std::array<texture_implementation_pspgu, 6> faces = {};
	};

}
}