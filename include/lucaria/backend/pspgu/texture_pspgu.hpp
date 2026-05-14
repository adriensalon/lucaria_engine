#pragma once

#include <lucaria/core/math.hpp>

#include <lucaria/backend/pspgu/backend_pspgu.hpp>

namespace lucaria {
namespace detail {

	struct texture_implementation_pspgu {
		bool is_owning = false;
		void* pixels = nullptr;        // VRAM or memalign(16)
		int psm = GU_PSM_8888;
		int tbw = 0;                   // texture buffer width
		bool is_swizzled = false;
	};

}
}