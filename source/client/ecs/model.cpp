#include <lucaria/ecs/model.hpp>

namespace lucaria {

blockout_model_component& blockout_model_component::use_mesh(const mesh_object mesh)
{
    _mesh = mesh;
    return *this;
}

unlit_model_component& unlit_model_component::use_mesh(const mesh_object mesh)
{
    _mesh = mesh;
    return *this;
}

unlit_model_component& unlit_model_component::use_color(const texture_object color)
{
    _color = color;
    return *this;
}

}
