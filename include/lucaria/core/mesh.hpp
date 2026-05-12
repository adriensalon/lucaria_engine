#pragma once

#include <unordered_map>

#include <lucaria/core/geometry.hpp>
#include <lucaria/core/resource.hpp>

#include <lucaria/backend/opengl/mesh_opengl.hpp>

namespace lucaria {

struct mesh_object;

namespace detail {

    enum struct mesh_attribute {
        position,
        color,
        normal,
        tangent,
        bitangent,
        texcoord,
        bones,
        weights
    };

    struct mesh_implementation {
        LUCARIA_DELETE_DEFAULT(mesh_implementation)
        mesh_implementation(const mesh_implementation& other) = delete;
        mesh_implementation& operator=(const mesh_implementation& other) = delete;
        mesh_implementation(mesh_implementation&& other);
        mesh_implementation& operator=(mesh_implementation&& other);
        ~mesh_implementation();

        mesh_implementation(const geometry_implementation& from);
        void update_attribute(const geometry_implementation& from, const mesh_attribute attribute, const glm::uint size, const glm::uint offset = 0);
        void update_indices(const geometry_implementation& from, const glm::uint size, const glm::uint offset = 0);
        [[nodiscard]] glm::uint get_size() const;
        [[nodiscard]] glm::uint get_array_handle() const;
        [[nodiscard]] glm::uint get_elements_handle() const;
        [[nodiscard]] const std::unordered_map<mesh_attribute, glm::uint>& get_attribute_handles() const;
        [[nodiscard]] const std::vector<glm::mat4>& get_invposes() const;

    private:
		mesh_implementation_opengl _opengl_impl;
    };

    struct mesh_manager {
        mesh_object create(const geometry_implementation& geometry);
        mesh_object fetch(const std::filesystem::path& path);

    private:
        resource_manager<mesh_implementation> _resources = {};
    };

}

struct mesh_object {
	mesh_object() = default;
	mesh_object(const mesh_object& other) = default;
	mesh_object& operator=(const mesh_object& other) = default;
	mesh_object(mesh_object&& other) = default;
	mesh_object& operator=(mesh_object&& other) = default;

    /// TODO GO CONTEXT
    static mesh_object create(const detail::geometry_implementation& geometry);

    /// TODO GO CONTEXT
    static mesh_object fetch(const std::filesystem::path& path);

    /// @brief Checks if the mesh is ready to be used
    /// @return true if the mesh is ready, false otherwise
    [[nodiscard]] bool has_value() const;
	
	/// @brief Conversion operator for the has_value member function
	[[nodiscard]] explicit operator bool() const;

private:
    detail::resource_container<detail::mesh_implementation>* _cell = nullptr;
    explicit mesh_object(detail::resource_container<detail::mesh_implementation>* cell);
    friend struct detail::mesh_manager;
    friend struct program;
    friend struct framebuffer;
    friend struct rendering_system;
};

// Internal definitions
namespace _detail {

#if LUCARIA_CONFIG_DEBUG

    struct guizmo_mesh {
        LUCARIA_DELETE_DEFAULT(guizmo_mesh)
        guizmo_mesh(const guizmo_mesh& other) = delete;
        guizmo_mesh& operator=(const guizmo_mesh& other) = delete;
        guizmo_mesh(guizmo_mesh&& other);
        guizmo_mesh& operator=(guizmo_mesh&& other);
        ~guizmo_mesh();

        guizmo_mesh(const detail::geometry_implementation& from);
        guizmo_mesh(const std::vector<glm::vec3>& positions, const std::vector<glm::uvec2>& indices);

        void update(const std::vector<glm::vec3>& positions, const std::vector<glm::uvec2>& indices);

        [[nodiscard]] glm::uint get_size() const;
        [[nodiscard]] glm::uint get_array_handle() const;
        [[nodiscard]] glm::uint get_elements_handle() const;
        [[nodiscard]] glm::uint get_positions_handle() const;

    private:
        bool _is_owning;
        glm::uint _size;
        glm::uint _array_handle;
        glm::uint _elements_handle;
        glm::uint _positions_handle;
    };

#endif

}
}
