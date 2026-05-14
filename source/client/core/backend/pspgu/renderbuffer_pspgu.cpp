#include <lucaria/core/renderbuffer.hpp>

namespace lucaria {
namespace detail {

    renderbuffer_implementation::renderbuffer_implementation(renderbuffer_implementation&& other)
    {
    }

    renderbuffer_implementation& renderbuffer_implementation::operator=(renderbuffer_implementation&& other)
    {
        return *this;
    }

    renderbuffer_implementation::~renderbuffer_implementation()
    {
    }

    renderbuffer_implementation::renderbuffer_implementation(const uint32x2 size, const glm::uint internal_format, const glm::uint samples)
        : size(size)
    {
    }

    void renderbuffer_implementation::resize(const uint32x2 new_size)
    {
    }

}
}