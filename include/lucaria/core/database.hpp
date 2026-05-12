#pragma once

#include <lucaria/core/animation.hpp>
#include <lucaria/core/texture.hpp>
#include <lucaria/core/mesh.hpp>
#include <lucaria/core/skeleton.hpp>
#include <lucaria/core/animation.hpp>
#include <lucaria/core/shape.hpp>

namespace lucaria {
namespace detail {

struct asset_database {
	resource_manager<image_implementation> images = {};
    resource_manager<texture_implementation> textures = {};
	resource_manager<geometry_implementation> geometries = {};
	resource_manager<shape_implementation> shapes = {};
    resource_manager<mesh_implementation> meshes = {};
	// fonts
	// audios
	// sounds
	resource_manager<skeleton_implementation> skeletons = {};
	resource_manager<animation_implementation> animations = {};
	resource_manager<motion_track_implementation> motion_tracks = {};
	resource_manager<event_track_implementation> event_tracks = {};
};

asset_database& engine_assets();

}
}