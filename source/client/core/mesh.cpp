#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>

#include <lucaria/core/database.hpp>
#include <lucaria/core/mesh.hpp>

namespace lucaria {

extern void _fetch_bytes(const std::filesystem::path& file_path, const std::function<void(const std::vector<char>&)>& callback, bool persist);

namespace detail {
    namespace {

        static async_container<mesh_implementation> _fetch_mesh_async(const std::filesystem::path& path)
        {
            std::shared_ptr<std::promise<geometry_implementation>> _geometry_promise = std::make_shared<std::promise<geometry_implementation>>();
            _fetch_bytes(path, [_geometry_promise](const std::vector<char>& _data_bytes) {
        geometry_implementation _geometry(_data_bytes);
        _geometry_promise->set_value(std::move(_geometry)); }, true);

            // create mesh on main thread
            return async_container<mesh_implementation>(_geometry_promise->get_future(), [](const geometry_implementation& _from) {
                return mesh_implementation(_from);
            });
        }
    }

}

mesh_object mesh_object::fetch(const std::filesystem::path& path)
{
    detail::resource_container<detail::mesh_implementation>* _resource = detail::engine_assets().meshes.get_or_create_by_path(path, [&] {
        return detail::_fetch_mesh_async(path);
    });

    return mesh_object { _resource };
}

bool mesh_object::has_value() const
{
    return _resource && _resource->is_ready();
}

mesh_object::operator bool() const
{
    return has_value();
}

mesh_object::mesh_object(detail::resource_container<detail::mesh_implementation>* resource)
    : _resource(resource)
{
}

}
