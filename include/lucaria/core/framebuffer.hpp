#pragma once

#include <lucaria/core/renderbuffer.hpp>

#include <lucaria/backend/opengl/framebuffer_opengl.hpp>

namespace lucaria {
namespace detail {

    struct framebuffer_implementation {
        framebuffer_implementation(const framebuffer_implementation& other) = delete;
        framebuffer_implementation& operator=(const framebuffer_implementation& other) = delete;
        framebuffer_implementation(framebuffer_implementation&& other);
        framebuffer_implementation& operator=(framebuffer_implementation&& other);
        ~framebuffer_implementation();

        framebuffer_implementation();
        static void use_default();
        void use();
        void bind_color(const detail::texture_implementation& color);
        void bind_color(renderbuffer& color);
        void bind_depth(detail::texture_implementation& depth);
        void bind_depth(renderbuffer& depth);
		
		framebuffer_implementation_opengl implementation_opengl;
    };

}
}
