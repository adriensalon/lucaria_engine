#pragma once

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