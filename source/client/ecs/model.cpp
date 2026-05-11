#include <lucaria/ecs/model.hpp>

namespace lucaria {

blockout_model_component& blockout_model_component::use_mesh(mesh& from)
{
    _mesh.emplace(from);
    return *this;
}

blockout_model_component& blockout_model_component::use_mesh(detail::async_container<mesh>& from)
{
    _mesh.emplace(from);
    return *this;
}

unlit_model_component& unlit_model_component::use_mesh(mesh& from)
{
    _mesh.emplace(from);
    return *this;
}

unlit_model_component& unlit_model_component::use_mesh(detail::async_container<mesh>& from)
{
    _mesh.emplace(from);
    return *this;
}

}
