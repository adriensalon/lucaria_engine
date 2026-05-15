#pragma once

#include <optional>

#include <lucaria/core/collision.hpp>
#include <lucaria/core/math.hpp>

namespace lucaria {

[[nodiscard]] std::optional<collision> raycast(const glm::vec3& from, const glm::vec3& to);
void set_world_gravity(const glm::float32 newtons);

}
