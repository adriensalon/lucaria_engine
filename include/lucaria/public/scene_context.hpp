#pragma once

namespace lucaria {

using scene_entity = int;

struct scene_context {

    /// @brief Creates a new entity in the scene and returns its handle
    /// @return the new entity handle
    [[nodiscard]] scene_entity emplace_entity();

    /// @brief Schedules an entity for erasure at the end of the current frame. The entity will be removed from the scene and all its components will be freed.
    /// @param entity
    void mark_erase_entity(const scene_entity entity);

    /// @brief Schedules the current scene for erasure at the end of the current frame. The scene will be removed from the engine and all its resources will be freed.
    void mark_erase_self();

    template <typename UserComponentType>
    UserComponentType& emplace_user_component(const scene_entity entity);

    template <typename... ComponentTypes>
    void each_view(const std::function<void(const scene_entity, ComponentTypes&&...)>& callback);

    template <typename... ComponentTypes>
    void each_view_self(const std::function<void(const scene_entity, ComponentTypes&&...)>& callback);

	//
	
    // animator_component& emplace_animator(const scene_entity entity);
    // get animator
    // erase animator
};

}