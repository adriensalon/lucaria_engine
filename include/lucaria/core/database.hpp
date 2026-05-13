#pragma once

#include <lucaria/core/animation.hpp>
#include <lucaria/core/cubemap.hpp>
#include <lucaria/core/event_track.hpp>
#include <lucaria/core/font.hpp>
#include <lucaria/core/mesh.hpp>
#include <lucaria/core/motion_track.hpp>
#include <lucaria/core/shape.hpp>
#include <lucaria/core/skeleton.hpp>
#include <lucaria/core/sound_track.hpp>
#include <lucaria/core/texture.hpp>

namespace lucaria {
namespace detail {

    struct asset_database {
        resource_manager<image_implementation> images = {};
        resource_manager<texture_implementation> textures = {};
        resource_manager<cubemap_implementation> cubemaps = {};
        resource_manager<geometry_implementation> geometries = {};
        resource_manager<shape_implementation> shapes = {};
        resource_manager<mesh_implementation> meshes = {};
        resource_manager<font_implementation> fonts = {};
        resource_manager<audio_implementation> audios = {};
        resource_manager<sound_track_implementation> sound_tracks = {};
        resource_manager<skeleton_implementation> skeletons = {};
        resource_manager<animation_implementation> animations = {};
        resource_manager<motion_track_implementation> motion_tracks = {};
        resource_manager<event_track_implementation> event_tracks = {};
    };

    asset_database& engine_assets();

}
}