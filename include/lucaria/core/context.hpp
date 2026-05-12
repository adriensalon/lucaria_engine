#pragma once

#include <lucaria/core/input.hpp>
#include <lucaria/core/scene.hpp>

namespace lucaria {

/// @brief Represents the context of the current game, which can be used to create scenes, get input state and so on.	
struct game_context {

	/// @brief Creates a new scene and returns its object
	/// @return the new scene object
	template <typename SceneType>
	[[nodiscard]] SceneType& emplace_scene();

    // input

    /// @brief Gets if the implementation provides keyboard events
    /// @return if the feature is supported
    [[nodiscard]] bool is_keyboard_supported();

    /// @brief Gets if the implementation provides mouse events
    /// @return if the feature is supported
    [[nodiscard]] bool is_mouse_supported();

    /// @brief Gets if the implementation provides touch events
    /// @return if the feature is supported
    [[nodiscard]] bool is_touch_supported();

    /// @brief Gets the state of the tracked keyboard keys
    /// @return state of the keys
    [[nodiscard]] std::unordered_map<button_key, button_event>& buttons();

    /// @brief Gets the state of the tracked mouse buttons
    /// @return state of the mouse buttons
    [[nodiscard]] std::unordered_map<glm::uint, pointer_event>& pointers();

    /// @brief Gets the current mouse position
    /// Syntactic sugar for calling get_pointers()[0].position
    /// @return current mouse position
    [[nodiscard]] glm::vec2 mouse_position();

    /// @brief Gets the current mouse position delta from previous frame
    /// Syntactic sugar for calling get_pointers()[0].delta
    /// @return current mouse position delta from previous frame
    [[nodiscard]] glm::vec2& mouse_position_delta();

    // dynamics

    /// @brief Raycasts shapes geometry
    /// @param from the position to raycast from
    /// @param to the position to raycast to
    [[nodiscard]] std::optional<collision> raycast_world(const glm::vec3& from, const glm::vec3& to);

    /// @brief Sets the global gravity
    /// @param newtons global gravity along -Y axis
    void set_world_gravity(const glm::float32 newtons);

    // mixer

    /// @brief Uses a transform component as the audio listener
    /// @param transform the transform component to use
    // void use_listener_transform(const transform_component transform);

    // rendering

    /// @brief
    /// @param enable
    void set_fxaa_enable(const bool enable);

    /// @brief
    /// @param contrast_threshold (default is 0.0312f)
    void set_fxaa_contrast_threshold(const glm::float32 contrast_threshold);

    /// @brief
    /// @param relative_threshold (default is 0.125f)
    void set_fxaa_relative_threshold(const glm::float32 relative_threshold);

    /// @brief
    /// @param edge_sharpness (default is 1.5f)
    void set_fxaa_edge_sharpness(const glm::float32 edge_sharpness);
};

/// @brief Represents the context of the current scene, which can be used to create entities, add components and so on.
struct scene_context {

    /// @brief Creates a new entity in the scene and returns its handle
    /// @return the new entity handle
    [[nodiscard]] scene_entity emplace_entity();

    /// @brief Schedules an entity for erasure at the end of the current frame. The entity will be removed from the scene and all its components will be freed.
    /// @param entity 
    void mark_erase_entity(const scene_entity entity);

	/// @brief Schedules the current scene for erasure at the end of the current frame. The scene will be removed from the engine and all its resources will be freed.
	void mark_erase_self();

    // animator_component& emplace_animator(const scene_entity entity);
    // get animator
    // erase animator

    // textures

    /// @brief Creates a new texture with the specified size
    /// @param size	the size of the texture to create
    /// @return a texture object with an empty texture of the specified size
    [[nodiscard]] texture_object emplace_texture(const glm::uvec2 size);

    /// @brief Loads an image from a file asynchronously and uploads directly to the device,
    /// lets the runtime choose the best format it can use without downloading the others
    /// @param path path to load uncompressed image version from
    /// @param etc2_path path to load ETC2 compressed image version from
    /// @param s3tc_path path to load S3TC compressed image version from
    [[nodiscard]] texture_object fetch_texture(
        const std::filesystem::path& path,
        const std::optional<std::filesystem::path>& etc2_path = std::nullopt,
        const std::optional<std::filesystem::path>& s3tc_path = std::nullopt);
};

}