#include <glm/gtc/matrix_transform.hpp>

#include <lucaria/core/database.hpp>
#include <lucaria/core/math.hpp>
#include <lucaria/core/shape.hpp>

namespace lucaria {

extern void _fetch_bytes(const std::filesystem::path& file_path, const std::function<void(const std::vector<char>&)>& callback, bool persist);

namespace detail {
    namespace {

        static void _make_convex_hull_shape(const detail::geometry_implementation& from, std::unique_ptr<btCollisionShape>& handle)
        {
            handle = std::make_unique<btConvexHullShape>();
            btConvexHullShape* _hull_shape = static_cast<btConvexHullShape*>(handle.get());
            for (const glm::vec3& _position : from.data.positions) {
                _hull_shape->addPoint(btVector3(_position.x, _position.y, _position.z));
            }
            _hull_shape->recalcLocalAabb();
        }

        static void _make_triangle_mesh_shape(const detail::geometry_implementation& from, std::unique_ptr<btCollisionShape>& handle, std::unique_ptr<btTriangleMesh>& triangle_handle)
        {
            btVector3 _vertex_1, _vertex_2, _vertex_3;
            triangle_handle = std::make_unique<btTriangleMesh>();
            for (const glm::uvec3& _index : from.data.indices) {
                _vertex_1 = btVector3(from.data.positions[_index.x].x, from.data.positions[_index.x].y, from.data.positions[_index.x].z);
                _vertex_2 = btVector3(from.data.positions[_index.y].x, from.data.positions[_index.y].y, from.data.positions[_index.y].z);
                _vertex_3 = btVector3(from.data.positions[_index.z].x, from.data.positions[_index.z].y, from.data.positions[_index.z].z);
                triangle_handle->addTriangle(_vertex_1, _vertex_2, _vertex_3);
            }
            handle = std::make_unique<btBvhTriangleMeshShape>(triangle_handle.get(), true);
        }

        // todo impact triangle mesh

        static async_container<shape_implementation> _fetch_shape_async(const std::filesystem::path& data_path, const shape_algorithm algorithm)
        {
            std::shared_ptr<std::promise<shape_implementation>> _promise = std::make_shared<std::promise<shape_implementation>>();
            _fetch_bytes(data_path, [_promise, algorithm](const std::vector<char>& _data_bytes) {
        geometry_implementation _geometry(_data_bytes);
        shape_implementation _shape(_geometry, algorithm);
        _promise->set_value(std::move(_shape)); }, true);

            // creating bullet collision shapes on worker thread is safe
            return async_container<shape_implementation>(_promise->get_future());
        }

    }

    shape_implementation::shape_implementation(const geometry_implementation& from, const shape_algorithm algorithm)
    {
        if (algorithm == shape_algorithm::convex_hull) {
            _make_convex_hull_shape(from, collision_shape);

        } else if (algorithm == shape_algorithm::triangle_mesh) {
            _make_triangle_mesh_shape(from, collision_shape, triangle_geometry);

        } else if (algorithm == shape_algorithm::impact_triangle_mesh) {
            LUCARIA_RUNTIME_ERROR("Impact triangle mesh not implemented") // TODO
        }

        glm::float32 _zdistance = 0.f; // lets compute ?
        feet_to_center = glm::translate(glm::mat4(1), glm::vec3(0, +_zdistance, 0));
        center_to_feet = glm::inverse(feet_to_center);
    }

    shape_implementation::shape_implementation(btCollisionShape* handle, const glm::float32 zdistance)
    {
        collision_shape = std::unique_ptr<btCollisionShape>(handle);
        feet_to_center = glm::translate(glm::mat4(1), glm::vec3(0, +zdistance, 0));
        center_to_feet = glm::translate(glm::mat4(1), glm::vec3(0, -zdistance, 0));
    }

}

shape_object shape_object::create(const geometry_object geometry, const detail::shape_algorithm algorithm)
{
    return shape_object { detail::engine_assets().shapes.create_cell(
        detail::async_container<detail::shape_implementation>(
            detail::shape_implementation(geometry._resource->get(), algorithm))) };
}

shape_object shape_object::create_box(const glm::vec3& half_extents)
{
    return shape_object { detail::engine_assets().shapes.create_cell(
        detail::async_container<detail::shape_implementation>(
            detail::shape_implementation(
                new btBoxShape(convert_bullet(half_extents)), half_extents.z))) };
}

shape_object shape_object::create_sphere(const glm::float32 radius)
{
    return shape_object { detail::engine_assets().shapes.create_cell(
        detail::async_container<detail::shape_implementation>(
            detail::shape_implementation(
                new btSphereShape(static_cast<btScalar>(radius)), radius))) };
}

shape_object shape_object::create_capsule(const glm::float32 radius, const glm::float32 height)
{
    return shape_object { detail::engine_assets().shapes.create_cell(
        detail::async_container<detail::shape_implementation>(
            detail::shape_implementation(
                new btCapsuleShape(static_cast<btScalar>(radius), static_cast<btScalar>(height)), radius + height * 0.5f))) };
}

shape_object shape_object::create_cone(const glm::float32 radius, const glm::float32 height)
{
    return shape_object { detail::engine_assets().shapes.create_cell(
        detail::async_container<detail::shape_implementation>(
            detail::shape_implementation(
                new btConeShape(static_cast<btScalar>(radius), static_cast<btScalar>(height)), height * 0.5f))) };
}

shape_object shape_object::fetch(const std::filesystem::path& path, const detail::shape_algorithm algorithm)
{
    detail::resource_container<detail::shape_implementation>* _resource = detail::engine_assets().shapes.get_or_create_by_path(path, [&] {
        return detail::_fetch_shape_async(path, algorithm);
    });

    return shape_object { _resource };
}

[[nodiscard]] bool shape_object::has_value() const
{
    return _resource && _resource->is_ready();
}

[[nodiscard]] shape_object::operator bool() const
{
    return has_value();
}

shape_object::shape_object(detail::resource_container<detail::shape_implementation>* resource)
    : _resource(resource)
{
}

}
