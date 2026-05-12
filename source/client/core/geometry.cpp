#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>

#include <lucaria/core/geometry.hpp>
#include <lucaria/core/database.hpp>

namespace lucaria {

extern void _load_bytes(const std::filesystem::path& file_path, const std::function<void(const std::vector<char>&)>& callback);
extern void _fetch_bytes(const std::filesystem::path& file_path, const std::function<void(const std::vector<char>&)>& callback, bool persist);

namespace {

    static void load_data_from_bytes(geometry_data& data, const std::vector<char>& data_bytes)
    {
        _detail::bytes_stream _stream(data_bytes);
#if LUCARIA_JSON
        cereal::JSONInputArchive _archive(_stream);
#else
        cereal::PortableBinaryInputArchive _archive(_stream);
#endif
        _archive(data);
    }

}

namespace detail {

    geometry_implementation::geometry_implementation(geometry_data&& data)
    {
        this->data = std::move(data);
    }

    geometry_implementation::geometry_implementation(const std::vector<char>& data_bytes)
    {
        load_data_from_bytes(data, data_bytes);
    }

    async_container<geometry_implementation> fetch_geometry_async(const std::filesystem::path& data_path)
    {
        std::shared_ptr<std::promise<geometry_implementation>> _promise = std::make_shared<std::promise<geometry_implementation>>();
        _fetch_bytes(data_path, [_promise](const std::vector<char>& _data_bytes) {
        geometry_implementation _geometry(_data_bytes);
        _promise->set_value(std::move(_geometry)); }, true);

        return async_container<geometry_implementation>(_promise->get_future());
    }

	geometry_object geometry_manager::fetch(const std::filesystem::path& path)
    {
        resource_container<geometry_implementation>* cell = _resources.get_or_create_by_path(path, [&] {
            return fetch_geometry_async(path);
        });

        return geometry_object { cell };
    }

}

geometry_object geometry_object::fetch(const std::filesystem::path& path)
{
    return detail::engine_assets().geometries.fetch(path);
}

bool geometry_object::has_value() const
{
    return _cell && _cell->is_ready();
}

geometry_object::operator bool() const
{
    return has_value();
}

geometry_object::geometry_object(detail::resource_container<detail::geometry_implementation>* cell)
    : _cell(cell)
{
}

}