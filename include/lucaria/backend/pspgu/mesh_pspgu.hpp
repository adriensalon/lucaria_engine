#pragma once

#include <unordered_map>

#include <lucaria/core/math.hpp>

#include <lucaria/backend/pspgu/backend_pspgu.hpp>

namespace lucaria {
namespace detail {

    enum struct mesh_attribute;

    struct mesh_implementation_pspgu {
        bool is_owning = false;
        void* vertices = nullptr;      // aligned CPU memory
		uint32 vertex_count = 0;
		int vertex_type = 0;           // GU_VERTEX_32BITF | GU_TEXTURE_32BITF...
		int primitive = GU_TRIANGLES;
    };

}
}