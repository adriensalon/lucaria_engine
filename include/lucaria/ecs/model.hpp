#pragma once

#include <lucaria/core/mesh.hpp>
#include <lucaria/core/program.hpp>
#include <lucaria/core/texture.hpp>

namespace lucaria {

/// @brief 
struct blockout_model_component {
    blockout_model_component() = default;
    blockout_model_component(const blockout_model_component& other) = delete;
    blockout_model_component& operator=(const blockout_model_component& other) = delete;
    blockout_model_component(blockout_model_component&& other) = default;
    blockout_model_component& operator=(blockout_model_component&& other) = default;

    blockout_model_component& use_mesh(const mesh_object mesh);
	
private:
    mesh_object _mesh = {};
    friend struct motion_system;
    friend struct rendering_system;
};

/// @brief 
struct unlit_model_component {
    unlit_model_component() = default;
    unlit_model_component(const unlit_model_component& other) = delete;
    unlit_model_component& operator=(const unlit_model_component& other) = delete;
    unlit_model_component(unlit_model_component&& other) = default;
    unlit_model_component& operator=(unlit_model_component&& other) = default;

	unlit_model_component& use_mesh(const mesh_object mesh);
	unlit_model_component& use_color(const texture_object color);

private:
    mesh_object _mesh = {};
    texture_object _color = {};
    friend struct motion_system;
    friend struct rendering_system;
};

}
