#pragma once

#include <LinearMath/btQuaternion.h>
#include <LinearMath/btTransform.h>
#include <LinearMath/btVector3.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <imgui.h>
#include <ozz/base/maths/simd_math.h>

namespace lucaria {

using int32 = glm::int32;
using uint32 = glm::uint32;
using float32 = glm::float32;
using float64 = glm::float64;

using int32x2 = glm::vec<2, int32>;
using int32x3 = glm::vec<3, int32>;
using int32x4 = glm::vec<4, int32>;
using uint32x2 = glm::vec<2, uint32>;
using uint32x3 = glm::vec<3, uint32>;
using uint32x4 = glm::vec<4, uint32>;
using float32x2 = glm::vec<2, float32>;
using float32x3 = glm::vec<3, float32>;
using float32x4 = glm::vec<4, float32>;
using float64x2 = glm::vec<2, float64>;
using float64x3 = glm::vec<3, float64>;
using float64x4 = glm::vec<4, float64>;

using int32x2x2 = glm::mat<2, 2, int32>;
using int32x3x3 = glm::mat<3, 3, int32>;
using int32x4x4 = glm::mat<4, 4, int32>;
using float32x2x2 = glm::mat<2, 2, float32>;
using float32x3x3 = glm::mat<3, 3, float32>;
using float32x4x4 = glm::mat<4, 4, float32>;
using float64x2x2 = glm::mat<2, 2, float64>;
using float64x3x3 = glm::mat<3, 3, float64>;
using float64x4x4 = glm::mat<4, 4, float64>;

/// @brief Projects a vector on a plane
/// @param vector the vector to project
/// @param normal the normal of the plane
/// @return the projected vector
[[nodiscard]] glm::vec3 project_on_plane(const glm::vec3& vector, const glm::vec3& normal);

/// @brief Converts to glm types
/// @param vector the bullet vector to convert
/// @return the converted vector
[[nodiscard]] glm::vec3 convert(const btVector3& vector);

/// @brief Converts to glm types
/// @param vector the ozz vector to convert
/// @return the converted vector
[[nodiscard]] glm::vec3 convert(const ozz::math::Float3& vector);

/// @brief Converts to glm types
/// @param vector the bullet transform to convert
/// @return the converted matrix
[[nodiscard]] glm::mat4 convert(const btTransform& transform);

/// @brief Converts to glm types
/// @param vector the ozz matrix to convert
/// @return the converted matrix
[[nodiscard]] glm::mat4 convert(const ozz::math::Float4x4& matrix);

/// @brief Converts to ozz types
/// @param vector the matrix to convert
/// @return the converted ozz matrix
[[nodiscard]] ozz::math::Float4x4 convert_ozz(const glm::mat4& matrix);

/// @brief Converts to ozz types
/// @param vector the matrix to convert
/// @return the converted ozz matrix
[[nodiscard]] btVector3 convert_bullet(const glm::vec3& vector);
[[nodiscard]] btQuaternion convert_bullet(const glm::quat& vector);
[[nodiscard]] btTransform convert_bullet(const glm::mat4& matrix);

[[nodiscard]] ImVec2 convert_imgui(const glm::vec2& vector);

}
