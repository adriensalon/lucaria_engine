#pragma once

#include <lucaria/core/texture.hpp>
#include <lucaria/core/mesh.hpp>
#include <lucaria/core/skeleton.hpp>
#include <lucaria/core/animation.hpp>

namespace lucaria {
namespace detail {

struct asset_database {
	image_manager images = {};
	geometry_manager geometries = {};

    texture_manager textures = {};
    mesh_manager meshes = {};
    // skeleton_manager skeletons;
    // animation_manager animations;
};

asset_database& engine_assets();

}
}