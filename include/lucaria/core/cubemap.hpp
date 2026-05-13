#pragma once

#include <lucaria/core/image.hpp>
#include <lucaria/core/resource.hpp>

#include <lucaria/backend/opengl/cubemap_opengl.hpp>

namespace lucaria {
namespace detail {

    struct cubemap_implementation {
        LUCARIA_DELETE_DEFAULT(cubemap_implementation)
        cubemap_implementation(const cubemap_implementation& other) = delete;
        cubemap_implementation& operator=(const cubemap_implementation& other) = delete;
        cubemap_implementation(cubemap_implementation&& other);
        cubemap_implementation& operator=(cubemap_implementation&& other);
        ~cubemap_implementation();

        cubemap_implementation(const std::array<image_implementation, 6>& images);

        cubemap_implementation_opengl implementation_opengl;
    };

}

struct cubemap_object {
    cubemap_object() = default;
    cubemap_object(const cubemap_object& other) = default;
    cubemap_object& operator=(const cubemap_object& other) = default;
    cubemap_object(cubemap_object&& other) = default;
    cubemap_object& operator=(cubemap_object&& other) = default;

    /// TODO GO CONTEXT
    static cubemap_object create(const glm::uvec2 size);

    /// TODO GO CONTEXT
    static cubemap_object fetch(
        const std::array<std::filesystem::path, 6>& data_paths,
        const std::optional<std::array<std::filesystem::path, 6>>& etc2_paths = std::nullopt,
        const std::optional<std::array<std::filesystem::path, 6>>& s3tc_paths = std::nullopt);

    /// @brief Checks if the cubemap is ready to be used
    /// @return true if the cubemap is ready, false otherwise
    [[nodiscard]] bool has_value() const;

    /// @brief Conversion operator for the has_value member function
    [[nodiscard]] explicit operator bool() const;

private:
    detail::resource_container<detail::cubemap_implementation>* _resource = nullptr;
    explicit cubemap_object(detail::resource_container<detail::cubemap_implementation>* resource);
    friend struct rendering_system;
};

}
