#pragma once

#include <optional>

#include <lucaria/core/collision.hpp>
#include <lucaria/core/math.hpp>

namespace lucaria {

[[nodiscard]] std::optional<collision> raycast(const glm::vec3& from, const glm::vec3& to);
void set_world_gravity(const glm::float32 newtons);

/// @brief Runtime API for dynamics system configuration and usage.
/// This context provides functionnality for raycasting and interacting with the
/// bullet dynamics world.
struct dynamics_context {

    /// @brief Raycasts shapes geometry
    /// @param from the position to raycast from
    /// @param to the position to raycast to
    [[nodiscard]] std::optional<collision> raycast(const float32x3& from, const float32x3& to);

    /// @brief Sets the global gravity
    /// @param newtons global gravity along -Y axis
    void set_world_gravity(const float32 newtons);
};

}
