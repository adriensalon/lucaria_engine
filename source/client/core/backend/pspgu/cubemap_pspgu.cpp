#include <lucaria/core/cubemap.hpp>

namespace lucaria {
namespace detail {

    cubemap_implementation::cubemap_implementation(cubemap_implementation&& other)
    {
    }

    cubemap_implementation& cubemap_implementation::operator=(cubemap_implementation&& other)
    {
        return *this;
    }

    cubemap_implementation::~cubemap_implementation()
    {
    }

    cubemap_implementation::cubemap_implementation(const std::array<image_implementation, 6>& images)
    {
    }

}
}