#pragma once

#include <lucaria/system/transform.hpp>

namespace lucaria {

void use_listener_transform(transform_component& transform);

struct mixer_context {

    /// @brief Uses a transform component as the audio listener
    /// @param transform the transform component to use
    void use_listener_transform(transform_component& transform);

	/// @brief 
	/// @param volume 
	void set_listener_volume(const float32 volume);
};

}
