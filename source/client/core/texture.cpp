#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>

#include <lucaria/core/database.hpp>
#include <lucaria/core/error.hpp>
#include <lucaria/core/texture.hpp>

namespace lucaria {

extern const std::filesystem::path& _resolve_image_path(const std::filesystem::path& data_path, const std::optional<std::filesystem::path>& etc2_path, const std::optional<std::filesystem::path>& s3tc_path);
extern void _fetch_bytes(const std::filesystem::path& file_path, const std::function<void(const std::vector<char>&)>& callback, bool persist);

namespace detail {
    namespace {

        static async_container<texture_implementation> _fetch_texture_async(
            const std::filesystem::path& path,
            const std::optional<std::filesystem::path>& etc2_path,
            const std::optional<std::filesystem::path>& s3tc_path)
        {
            const std::filesystem::path& _image_path = _resolve_image_path(path, etc2_path, s3tc_path);
            std::shared_ptr<std::promise<image_implementation>> _image_promise = std::make_shared<std::promise<image_implementation>>();
            _fetch_bytes(_image_path, [_image_promise](const std::vector<char>& _data_bytes) {
        image_implementation _image(_data_bytes);
        _image_promise->set_value(std::move(_image)); }, true);

            // create texture on main thread
            return async_container<texture_implementation>(_image_promise->get_future(), [](const image_implementation& _from) {
                return texture_implementation(_from);
            });
        }

    }
}

texture_object texture_object::create(const glm::uvec2 size)
{
    return texture_object { detail::engine_assets().textures.create_cell(
        detail::async_container<detail::texture_implementation>(
            detail::texture_implementation(size))) };
}

texture_object texture_object::fetch(const std::filesystem::path& path, const std::optional<std::filesystem::path>& etc2_path, const std::optional<std::filesystem::path>& s3tc_path)
{
    detail::resource_container<detail::texture_implementation>* _resource = detail::engine_assets().textures.get_or_create_by_path(path, [&] {
        return detail::_fetch_texture_async(path, etc2_path, s3tc_path);
    });

    return texture_object { _resource };
}

bool texture_object::has_value() const
{
    return _resource && _resource->is_ready();
}

texture_object::operator bool() const
{
    return has_value();
}

void texture_object::resize(const glm::uvec2 size)
{
    if (_resource) {
        _resource->get().resize(size);
    }
}

// void texture_object::update_pixels(const image& from, const glm::uvec2 size, const glm::uvec2 offset)
// {
//     if (_cell) {
//         _cell->get().update_pixels(from, size, offset);
//     }
// }

glm::uvec2 texture_object::get_size() const
{
    if (_resource) {
        return _resource->get().get_size();
    }
    return glm::uvec2(0, 0);
}

texture_object::texture_object(detail::resource_container<detail::texture_implementation>* resource)
    : _resource(resource)
{
}

}
