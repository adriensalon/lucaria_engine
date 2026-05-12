#pragma once

#include <lucaria/common/geometry_data.hpp>
#include <lucaria/core/fetch.hpp>
#include <lucaria/core/platform.hpp>
#include <lucaria/core/resource.hpp>

namespace lucaria {

struct geometry_object;

namespace detail {

    /// @brief Represents any geometry on the host
    struct geometry_implementation {
        LUCARIA_DELETE_DEFAULT(geometry_implementation)
        geometry_implementation(const geometry_implementation& other) = delete;
        geometry_implementation& operator=(const geometry_implementation& other) = delete;
        geometry_implementation(geometry_implementation&& other) = default;
        geometry_implementation& operator=(geometry_implementation&& other) = default;

        geometry_implementation(geometry_data&& data);
        geometry_implementation(const std::vector<char>& data_bytes);

        geometry_data data;
    };

	struct geometry_manager {
		geometry_object fetch(const std::filesystem::path& path);

    private:
        resource_manager<geometry_implementation> _resources = {};
    };
	
}

struct geometry_object {
    geometry_object() = default;
    geometry_object(const geometry_object& other) = default;
    geometry_object& operator=(const geometry_object& other) = default;
    geometry_object(geometry_object&& other) = default;
    geometry_object& operator=(geometry_object&& other) = default;

	static geometry_object fetch(const std::filesystem::path& path);


    /// @brief Checks if the image is ready to be used
    /// @return true if the image is ready, false otherwise
    [[nodiscard]] bool has_value() const;

    /// @brief Conversion operator for the has_value member function
    [[nodiscard]] explicit operator bool() const;
	
private:
    detail::resource_container<detail::geometry_implementation>* _cell = nullptr;
    explicit geometry_object(detail::resource_container<detail::geometry_implementation>* cell);
    friend struct detail::geometry_manager;
    friend struct spatial_interface_component;
    friend struct rendering_system;
};

}
