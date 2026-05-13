#include <lucaria/core/error.hpp>
#include <lucaria/core/program.hpp>

namespace lucaria {

void _fetch_bytes(const std::vector<std::filesystem::path>& file_paths, const std::function<void(const std::vector<std::vector<char>>&)>& callback, bool persist);

namespace detail {
    namespace {

        detail::async_container<program_implementation> fetch_program(const std::filesystem::path& vertex_data_path, const std::filesystem::path& fragment_data_path)
        {
            std::vector<std::filesystem::path> _shaders_paths = { vertex_data_path, fragment_data_path };
            std::shared_ptr<std::promise<std::pair<shader, shader>>> _shaders_promise = std::make_shared<std::promise<std::pair<shader, shader>>>();
            _fetch_bytes(_shaders_paths, [_shaders_promise](const std::vector<std::vector<char>>& _data_bytes) {
        std::pair<shader, shader> _shaders = {
            shader(_data_bytes[0]),
            shader(_data_bytes[1])
        };
        _shaders_promise->set_value(std::move(_shaders)); }, true);

            return detail::async_container<program_implementation>(_shaders_promise->get_future(), [](const std::pair<shader, shader>& _from) {
                return program_implementation(_from.first, _from.second);
            });
        }

    }

}

}
