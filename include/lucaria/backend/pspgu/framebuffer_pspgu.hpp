#pragma once

#include <optional>

#include <lucaria/core/math.hpp>

#include <lucaria/backend/pspgu/backend_pspgu.hpp>

namespace lucaria {
namespace detail {

    struct framebuffer_implementation_opengl {
        bool is_owning = false;
        void* color = nullptr;
		void* depth = nullptr;
		uint32x2 size {};
		int psm = GU_PSM_8888;
		int fbw = 512;
		bool has_color = false;
		bool has_depth = false;
    };

}
}