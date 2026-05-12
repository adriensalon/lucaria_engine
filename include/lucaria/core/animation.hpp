#pragma once

#include <ozz/animation/runtime/animation.h>
#include <ozz/animation/runtime/track.h>

#include <lucaria/common/event_track_data.hpp>
#include <lucaria/core/platform.hpp>
#include <lucaria/core/resource.hpp>

namespace lucaria {

struct animation_object;
struct motion_track_object;
struct event_track_object;

namespace detail {

    struct animation_implementation {
        LUCARIA_DELETE_DEFAULT(animation_implementation)
        animation_implementation(const animation_implementation& other) = delete;
        animation_implementation& operator=(const animation_implementation& other) = delete;
        animation_implementation(animation_implementation&& other) = default;
        animation_implementation& operator=(animation_implementation&& other) = default;

        animation_implementation(const std::vector<char>& bytes);
        animation_implementation(ozz::animation::Animation&& animation);

        ozz::animation::Animation animation;
    };

    struct motion_track_implementation {
        LUCARIA_DELETE_DEFAULT(motion_track_implementation)
        motion_track_implementation(const motion_track_implementation& other) = delete;
        motion_track_implementation& operator=(const motion_track_implementation& other) = delete;
        motion_track_implementation(motion_track_implementation&& other) = default;
        motion_track_implementation& operator=(motion_track_implementation&& other) = default;

        motion_track_implementation(const std::vector<char>& bytes);
        motion_track_implementation(ozz::animation::Float3Track&& translation_track, ozz::animation::QuaternionTrack&& rotation_track);
        [[nodiscard]] glm::vec3 get_total_translation() const;

        ozz::animation::Float3Track translation_track;
        ozz::animation::QuaternionTrack rotation_track;
    };

    struct event_track_implementation {
        LUCARIA_DELETE_DEFAULT(event_track_implementation)
        event_track_implementation(const event_track_implementation& other) = delete;
        event_track_implementation& operator=(const event_track_implementation& other) = delete;
        event_track_implementation(event_track_implementation&& other) = default;
        event_track_implementation& operator=(event_track_implementation&& other) = default;

        event_track_implementation(const std::vector<char>& bytes);
        event_track_implementation(event_track_data&& data);

        event_track_data data;
    };

}

/// @brief Represents an animation on the host. Can be created from an animation file or from empty data.
struct animation_object {
    animation_object() = default;
    animation_object(const animation_object& other) = default;
    animation_object& operator=(const animation_object& other) = default;
    animation_object(animation_object&& other) = default;
    animation_object& operator=(animation_object&& other) = default;

    static animation_object fetch(const std::filesystem::path& path);

    /// @brief Checks if the texture is ready to be used
    /// @return true if the texture is ready, false otherwise
    [[nodiscard]] bool has_value() const;

    /// @brief Conversion operator for the has_value member function
    [[nodiscard]] explicit operator bool() const;

private:
    detail::resource_container<detail::animation_implementation>* _resource = nullptr;
    explicit animation_object(detail::resource_container<detail::animation_implementation>* cell);
    friend struct animator_component;
    friend struct motion_system;
};

/// @brief Represents a motion track on the host. Can be created from a motion track file or from empty data.
struct motion_track_object {
    motion_track_object() = default;
    motion_track_object(const motion_track_object& other) = default;
    motion_track_object& operator=(const motion_track_object& other) = default;
    motion_track_object(motion_track_object&& other) = default;
    motion_track_object& operator=(motion_track_object&& other) = default;

    static motion_track_object fetch(const std::filesystem::path& path);

    /// @brief Checks if the texture is ready to be used
    /// @return true if the texture is ready, false otherwise
    [[nodiscard]] bool has_value() const;

    /// @brief Conversion operator for the has_value member function
    [[nodiscard]] explicit operator bool() const;

private:
    detail::resource_container<detail::motion_track_implementation>* _resource = nullptr;
    explicit motion_track_object(detail::resource_container<detail::motion_track_implementation>* resource);
    friend struct animator_component;
    friend struct motion_system;
};

/// @brief Represents an event track on the host. Can be created from an event track file or from empty data.
struct event_track_object {
    event_track_object() = default;
    event_track_object(const event_track_object& other) = default;
    event_track_object& operator=(const event_track_object& other) = default;
    event_track_object(event_track_object&& other) = default;
    event_track_object& operator=(event_track_object&& other) = default;

    static event_track_object fetch(const std::filesystem::path& path);

    /// @brief Checks if the texture is ready to be used
    /// @return true if the texture is ready, false otherwise
    [[nodiscard]] bool has_value() const;

    /// @brief Conversion operator for the has_value member function
    [[nodiscard]] explicit operator bool() const;

private:
    detail::resource_container<detail::event_track_implementation>* _resource = nullptr;
    explicit event_track_object(detail::resource_container<detail::event_track_implementation>* resource);
    friend struct animator_component;
    friend struct motion_system;
};

}
