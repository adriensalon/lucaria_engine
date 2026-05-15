#pragma once

#include <lucaria/core/cubemap.hpp>
#include <lucaria/entity/animator.hpp>
#include <lucaria/entity/transform.hpp>

namespace lucaria {

void use_skybox_cubemap(const cubemap_object cubemap);
void set_skybox_rotation(const glm::float32 rotation);
void use_camera_transform(transform_component& transform);
void use_camera_bone(animator_component& animator, const std::string& bone);
void set_camera_fov(const glm::float32 fov);
void set_camera_near(const glm::float32 near);
void set_camera_far(const glm::float32 far);
void set_camera_rotation(const glm::float32 yaw, const glm::float32 pitch);
void set_clear_color(const glm::vec4& color);
void set_clear_depth(const bool is_clearing);
void set_fxaa_enable(const bool enable);
void set_fxaa_contrast_threshold(const glm::float32 contrast_threshold);
void set_fxaa_relative_threshold(const glm::float32 relative_threshold);
void set_fxaa_edge_sharpness(const glm::float32 edge_sharpness);
void set_guizmos(const bool enabled);

}
