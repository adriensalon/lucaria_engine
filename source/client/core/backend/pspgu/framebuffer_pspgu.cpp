#include <lucaria/core/framebuffer.hpp>

namespace lucaria {
namespace detail {

    framebuffer_implementation::framebuffer_implementation(framebuffer_implementation&& other)
    {
    }

    framebuffer_implementation& framebuffer_implementation::operator=(framebuffer_implementation&& other)
    {
        return *this;
    }

    framebuffer_implementation::~framebuffer_implementation()
    {
    }

    framebuffer_implementation::framebuffer_implementation()
    {
    }

    void framebuffer_implementation::use_default()
    {
    }

    void framebuffer_implementation::use()
    {
    }

    void framebuffer_implementation::bind_color(const texture_implementation& color)
    {
    }

    void framebuffer_implementation::bind_color(renderbuffer_implementation& color)
    {
    }

    void framebuffer_implementation::bind_depth(texture_implementation& depth)
    {
    }

    void framebuffer_implementation::bind_depth(renderbuffer_implementation& depth)
    {
    }

}
}