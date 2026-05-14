#pragma once

#include <lucaria/core/animation.hpp>
#include <lucaria/core/audio.hpp>
#include <lucaria/core/collision.hpp>
#include <lucaria/core/cubemap.hpp>
#include <lucaria/core/database.hpp>
#include <lucaria/core/fetch.hpp>
#include <lucaria/core/font.hpp>
#include <lucaria/core/framebuffer.hpp>
#include <lucaria/core/geometry.hpp>
#include <lucaria/core/image.hpp>
#include <lucaria/core/input.hpp>
#include <lucaria/core/math.hpp>
#include <lucaria/core/mesh.hpp>
#include <lucaria/core/platform.hpp>
#include <lucaria/core/program.hpp>
#include <lucaria/core/renderbuffer.hpp>
#include <lucaria/core/run.hpp>
#include <lucaria/core/shader.hpp>
#include <lucaria/core/shape.hpp>
#include <lucaria/core/skeleton.hpp>
#include <lucaria/core/scene.hpp>
#include <lucaria/core/sound_track.hpp>
#include <lucaria/core/texture.hpp>

#include <lucaria/system/animator.hpp>
#include <lucaria/system/dynamics.hpp>
#include <lucaria/system/interface.hpp>
#include <lucaria/system/mixer.hpp>
#include <lucaria/system/model.hpp>
#include <lucaria/system/rendering.hpp>
#include <lucaria/system/rigidbody.hpp>
#include <lucaria/system/speaker.hpp>
#include <lucaria/system/transform.hpp>

namespace lucaria {

/// @brief Represents the context of the current scene, which can be used to create entities, add components and so on.
struct game_context {

    /// @brief Creates a new scene and returns its object
    /// @return the new scene object
    template <typename SceneType>
    SceneType& emplace_scene();

    /// @brief
    input_context input;

    /// @brief
    fetch_context fetch;

    /// @brief
    object_context object;

    /// @brief
    scene_context scene;

    /// @brief
    dynamics_context dynamics;

    /// @brief
    mixer_context mixer;

    /// @brief
    rendering_context rendering;
};

}