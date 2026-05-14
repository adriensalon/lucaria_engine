#pragma once

#include <string>
#include <unordered_map>

#include <lucaria/core/math.hpp>

#include <lucaria/core/backend/pspgu/mesh_pspgu.hpp>
#include <lucaria/core/backend/pspgu/texture_pspgu.hpp>

namespace lucaria {
namespace detail {

    struct program_implementation_pspgu {
        bool is_owning = false;
        const mesh_implementation_pspgu* mesh = nullptr;
    	const texture_implementation_pspgu* texture = nullptr;
		bool texture_enabled = false;
		bool lighting_enabled = false;
		bool depth_enabled = true;
    };

}
}