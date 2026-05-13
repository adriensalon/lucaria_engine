#pragma once

#include <lucaria/core/texture.hpp>

#include <lucaria/backend/opengl/renderbuffer_opengl.hpp>

namespace lucaria {
namespace detail {

    struct renderbuffer_implementation {
        renderbuffer_implementation() = delete;
        renderbuffer_implementation(const renderbuffer_implementation& other) = delete;
        renderbuffer_implementation& operator=(const renderbuffer_implementation& other) = delete;
        renderbuffer_implementation(renderbuffer_implementation&& other);
        renderbuffer_implementation& operator=(renderbuffer_implementation&& other);
        ~renderbuffer_implementation();

        renderbuffer_implementation(const uint32x2 size, const glm::uint internal_format, const glm::uint samples = 1);
        void resize(const uint32x2 new_size);

        renderbuffer_implementation_opengl implementation_opengl;

        uint32 sampling_count;
        uint32x2 size;
    };

}
}
