#pragma once

#include <lucaria/core/math.hpp>

#include <lucaria/backend/pspgu/backend_pspgu.hpp>

namespace lucaria {
namespace detail {

    struct renderbuffer_implementation_pspgu {
        bool is_owning = false;
        void* pixels = nullptr;
		int psm = GU_PSM_8888;
		int fbw = 512;
    };

}
}